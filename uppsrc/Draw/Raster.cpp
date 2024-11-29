#include "Draw.h"

namespace Upp {

Raster::Info::Info()
{
	bpp = 24;
	colors = 1 << 24;
	dots = Size(0, 0);
	hotspot = Point(0, 0);
	kind = IMAGE_OPAQUE;
	orientation = FLIP_NONE;
}

const RasterFormat *Raster::GetFormat()
{
	return NULL;
}

void Raster::SeekPage(int page)
{
	ASSERT(page == 0);
}

int Raster::GetActivePage() const
{
	return 0;
}

int Raster::GetPageCount()
{
	return 1;
}

int Raster::GetPageAspect(int page)
{
	return 0;
}

int Raster::GetPageDelay(int page)
{
	return 0;
}

Rect Raster::GetPageRect(int n)
{
	Size sz = GetSize();
	return Rect(0, 0, sz.cx, sz.cy);
}

int Raster::GetPageDisposal(int n)
{
	return 0;	
}

Value Raster::GetMetaData(String id)
{
	return Value();
}

void Raster::EnumMetaData(Vector<String>& id_list)
{
	id_list.Clear();
}

void Raster::Line::Pick(Line&& b)
{
	data = b.data;
	fmtdata = b.fmtdata;
	raster = b.raster;
	free = b.free;
	fmtfree = b.fmtfree;
	const_cast<Line *>(&b)->free = const_cast<Line *>(&b)->fmtfree = false;
#ifdef _DEBUG
	const_cast<Line *>(&b)->data = NULL;
	const_cast<Line *>(&b)->fmtdata = NULL;
#endif
}

void Raster::Line::MakeRGBA() const
{
	ASSERT(fmtdata && raster);
	int cx = raster->GetWidth();
	const RasterFormat *f = raster->GetFormat();
	if(f) {
		RGBA *rgba = new RGBA[cx];
		free = true;
		f->Read(rgba, fmtdata, cx, raster->GetPalette());
		data = rgba;
	}
	else
		data = (const RGBA *)fmtdata;
}

Raster::Info Raster::GetInfo()
{
	Info f;
	f.bpp = 32;
	f.colors = 256*256*256;
	f.dots = Size(0, 0);
	f.hotspot = Point(0, 0);
	f.kind = IMAGE_ALPHA;
	return f;
}

bool Raster::Create() { return true; }

bool Raster::IsError() { return false; }

int   Raster::GetPaletteCount() { return 0; }

const RGBA *Raster::GetPalette() { return NULL; }

Image Raster::GetImage(int x, int y, int cx, int cy, const Gate<int, int> progress)
{
	Size size = GetSize();
	y = minmax(y, 0, size.cy);
	int yy = minmax(y + cy, y, size.cy);
	x = minmax(x, 0, size.cx);
	cx = minmax(x + cx, x, size.cx) - x;
	ImageBuffer b(cx, yy - y);
	RGBA* t = b;
	int y0 = y;
	while(y < yy) {
		if(progress(y - y0, yy - y0))
			return Null;
		memcpy_t(t, ~GetLine(y) + x, cx);
		t += cx;
		y++;
	}
	Info f = GetInfo();
	b.SetHotSpot(f.hotspot - Point(x, y0));
	if(size.cx && size.cy)
		b.SetDots(Size(f.dots.cx * cx / size.cx, f.dots.cy * cy / size.cy));
	b.SetKind(f.kind);

	if(IsError())
		return Image();
	
	Image img = b;
	return FlipImage(img, f.orientation);
}

Image Raster::GetImage(const Gate<int, int> progress)
{
	Size sz = GetSize();
	return GetImage(0, 0, sz.cx, sz.cy, progress);
}

Raster::~Raster() {}

Raster::Line ImageRaster::GetLine(int line)
{
	return Line(img[line], false);
}

Size ImageRaster::GetSize()
{
	return img.GetSize();
}

Raster::Info ImageRaster::GetInfo()
{
	Raster::Info f = Raster::GetInfo();
	f.dots = img.GetDots();
	f.hotspot = img.GetHotSpot();
	f.kind = img.GetKind();
	return f;
}

MemoryRaster::MemoryRaster()
: size(0, 0)
{
}

void MemoryRaster::Load(Raster& raster)
{
	info = raster.GetInfo();
	size = raster.GetSize();
	palette.SetCount(raster.GetPaletteCount());
	if(!palette.IsEmpty())
		memcpy_t(palette.begin(), raster.GetPalette(), palette.GetCount());
	lines.SetCount(size.cy);
	if(const RasterFormat *fmt = raster.GetFormat()) {
		format = *fmt;
		int rowbytes = format.GetByteCount(size.cx);
		for(int i = 0; i < size.cy; i++) {
			lines[i].Alloc(rowbytes);
			memcpy_t(~lines[i], raster.GetLine(i).GetRawData(), rowbytes);
		}
	}
	else {
		format.SetRGBA();
		int rowbytes = sizeof(RGBA) * size.cx;
		for(int i = 0; i < size.cy; i++) {
			lines[i].Alloc(rowbytes);
			memcpy_t((RGBA *)~lines[i], raster.GetLine(i).GetRGBA(), rowbytes);
		}
	}
}

Raster::Line MemoryRaster::GetLine(int line)
{
	if(format.IsRGBA())
		return Line((const RGBA *)~lines[line], false);
	else
		return Line(~lines[line], this, false);
}

int MemoryRaster::GetLength() const
{
	return size.cy * (format.IsRGBA()
		? size.cx * sizeof(RGBA)
		: ((size.cx * info.bpp + 31) >> 5) * 4);
}

bool StreamRaster::Open(Stream& _s)
{
	s = &_s;
	error = !Create();
	return !error;
}

bool StreamRaster::IsError()
{
	return error || !s || s->IsError();
}

Image StreamRaster::Load(Stream& s, const Gate<int, int> progress)
{
	if(Open(s)) {
		Image img = GetImage(progress);
		if(!IsError())
			return img;
	}
	return Image();
}

Image StreamRaster::LoadFile(const char *fn, const Gate<int, int> progress)
{
	FileIn in(fn);
	return in ? Load(in, progress) : Image();
}

Image StreamRaster::LoadString(const String& s, const Gate<int, int> progress)
{
	StringStream ss(s);
	return Load(ss, progress);
}

static StaticCriticalSection sAnyRaster;

Vector<void *>& StreamRaster::Map()
{
	static Vector<void *> x;
	return x;
}

void StreamRaster::AddFormat(RasterFactory factory)
{
	INTERLOCKED_(sAnyRaster)
		Map().Add((void *)factory);
}

One<StreamRaster> StreamRaster::OpenAny(Stream& s)
{
	INTERLOCKED_(sAnyRaster)
		for(int i = 0; i < Map().GetCount(); i++) {
			int64 p = s.GetPos();
			One<StreamRaster> raster = (*RasterFactory(Map()[i]))();
			s.ClearError();
			if(raster->Open(s))
				return raster;
			s.ClearError();
			s.Seek(p);
		}
	return NULL;
}

Image StreamRaster::LoadAny(Stream& s, Gate<int, int> progress)
{
	One<StreamRaster> r = OpenAny(s);
	return r ? r->GetImage(progress) : Image();
}

Image StreamRaster::LoadFileAny(const char *fn, Gate<int, int> progress)
{
	FileIn in(fn);
	if(in)
		return LoadAny(in, progress);
	return Null;
}

Image StreamRaster::LoadStringAny(const String& s, Gate<int, int> progress)
{
	StringStream ss(s);
	return LoadAny(ss, progress);
}

}
