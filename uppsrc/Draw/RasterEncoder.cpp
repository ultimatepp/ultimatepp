#include "Draw.h"

namespace Upp {

RasterEncoder::RasterEncoder()
{
	size = Size(0, 0);
	dots = Size(0, 0);
	hotspot = Point(0, 0);
	format.SetRGBA();
}

void RasterEncoder::SetLine(RGBA *_line)
{
	line = _line;
	h.Clear();
}

void RasterEncoder::WriteLine(const RGBA *s)
{
	if(format.IsRGBA())
		WriteLineRaw((byte *)s);
	else {
		if(!scanline)
			scanline.Alloc(format.GetByteCount(size.cx));
		format.Write(scanline, s, size.cx, GetPaletteCv());
		WriteLineRaw(scanline);
	}
}

void RasterEncoder::WriteLine()
{
	WriteLine(line);
}

void RasterEncoder::Create(Size sz)
{
	size = sz;
	h.Alloc(sz.cx);
	line = h;
	Start(sz);
	scanline.Clear();
	line_bytes = format.GetByteCount(size.cx);
}

RGBA *RasterEncoder::Pal()
{
	if(!palette)
		palette.Alloc(256);
	return palette;
}

const RGBA *RasterEncoder::GetPalette()
{
	if(!palette) {
		palette.Alloc(256);
		RGBA *p = palette;
		int n = GetPaletteCount();
		for(int i = 0; i < n; i++)
			p[i].r = p[i].g = p[i].b = p[i].a = 255 * i * (n - 1) / (n - 1);
	}
	return palette;
}

void RasterEncoder::SetPalette(const RGBA *pal)
{
	int n = GetPaletteCount();
	if(n) {
		memcpy(Pal(), pal, n * sizeof(RGBA));
		palette_cv.Create();
		CreatePaletteCv(Pal(), n, *palette_cv);
	}
}

void RasterEncoder::SetPalette(Raster& raster)
{
	int n = GetPaletteCount();
	if(n) {
		if(raster.GetPaletteCount() == n)
			memcpy(Pal(), raster.GetPalette(), n * sizeof(RGBA));
		else
			CreatePalette(raster, Pal(), n);
		palette_cv.Create();
		CreatePaletteCv(Pal(), n, *palette_cv);
	}
}

void RasterEncoder::Create(Size sz, const RGBA *palette)
{
	SetPalette(palette);
	Create(sz);
}

void RasterEncoder::Create(int cx, int cy, const RGBA *palette)
{
	SetPalette(palette);
	Create(cx, cy);
}

void RasterEncoder::Create(Size sz, Raster& pal_raster)
{
	SetPalette(pal_raster);
	Create(sz);
}

void RasterEncoder::Create(int cx, int cy, Raster& pal_raster)
{
	SetPalette(pal_raster);
	Create(cx, cy);
}

int RasterEncoder::GetPaletteCount()
{
	return 0;
}

RasterEncoder::~RasterEncoder() {}

void ImageEncoder::Start(Size sz)
{
	ii = 0;
	ib.Create(sz);
	if(sz.cy)
		SetLine(ib[0]);
}

void ImageEncoder::WriteLineRaw(const byte *data)
{
	if((RGBA *)data != ~*this)
		memcpy(~*this, data, GetSize().cx * sizeof(RGBA));
	if(++ii < GetHeight())
		SetLine(ib[ii]);
}

void StreamRasterEncoder::Save(Stream& s, Raster& raster)
{
	if(raster.GetWidth() == 0 || raster.GetHeight() == 0)
		return;
	SetStream(s);
	Size sz = raster.GetSize();
	SetDots(raster.GetInfo().dots);
	SetHotSpot(raster.GetInfo().hotspot);
	Create(sz, raster);
	for(int i = 0; i < sz.cy; i++)
		WriteLine(raster[i]);
}

bool StreamRasterEncoder::SaveFile(const char *fn, Raster& raster)
{
	FileOut out(fn);
	Save(out, raster);
	return !out.IsError();
}

String StreamRasterEncoder::SaveString(Raster& raster)
{
	StringStream ss;
	Save(ss, raster);
	return ss;
}

void StreamRasterEncoder::Save(Stream& s, const Image& img)
{
	ImageRaster r(img);
	Save(s, r);
}

bool StreamRasterEncoder::SaveFile(const char *fn, const Image& img)
{
	ImageRaster r(img);
	return SaveFile(fn, r);
}

String StreamRasterEncoder::SaveString(const Image& img)
{
	ImageRaster r(img);
	return SaveString(r);
}

}
