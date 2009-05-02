#include "Draw.h"

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

int ImageBuffer::ScanKind() const
{
	bool a255 = false;
	bool a0 = false;
	const RGBA *s = pixels;
	const RGBA *e = s + GetLength();
	while(s < e) {
		if(s->a == 0)
			a0 = true;
		else
		if(s->a == 255)
			a255 = true;
		else
			return IMAGE_ALPHA;
		s++;
	}
	return a255 ? a0 ? IMAGE_MASK : IMAGE_OPAQUE : IMAGE_EMPTY;
}

void ImageBuffer::Create(int cx, int cy)
{
	ASSERT(cx >= 0 && cy >= 0);
	size.cx = cx;
	size.cy = cy;
	pixels.Alloc(GetLength());
#ifdef _DEBUG
	RGBA *s = pixels;
	RGBA *e = pixels + GetLength();
	byte  a = 0;
	while(s < e) {
		s->a = a;
		a = ~a;
		s->r = 255;
		s->g = s->b = 0;
		s++;
	}
#endif
	kind = IMAGE_UNKNOWN;
	spot2 = hotspot = Point(0, 0);
	dots = Size(0, 0);
}

void ImageBuffer::DeepCopy(const ImageBuffer& img)
{
	Create(img.GetSize());
	SetHotSpot(img.GetHotSpot());
	Set2ndSpot(img.Get2ndSpot());
	SetDots(img.GetDots());
	memcpy(pixels, img.pixels, GetLength() * sizeof(RGBA));
}

void ImageBuffer::Set(Image& img)
{
	if(img.data)
		if(img.data->refcount == 1) {
			size = img.GetSize();
			kind = IMAGE_UNKNOWN;
			hotspot = img.GetHotSpot();
			spot2 = img.Get2ndSpot();
			dots = img.GetDots();
			pixels = img.data->buffer.pixels;
			img.Clear();
		}
		else {
			DeepCopy(img.data->buffer);
			kind = IMAGE_UNKNOWN;
			img.Clear();
		}
	else
		Create(0, 0);
}


void ImageBuffer::operator=(Image& img)
{
	Clear();
	Set(img);
}

void ImageBuffer::operator=(ImageBuffer& img)
{
	Clear();
	Image m = img;
	Set(m);
}

ImageBuffer::ImageBuffer(Image& img)
{
	Set(img);
}

ImageBuffer::ImageBuffer(ImageDraw& w)
{
	Image img = w;
	Set(img);
}

ImageBuffer::ImageBuffer(ImageBuffer& b)
{
	kind = b.kind;
	size = b.size;
	dots = b.dots;
	pixels = b.pixels;
	hotspot = b.hotspot;
	spot2 = b.spot2;
}

void Image::Set(ImageBuffer& b)
{
	if(b.GetWidth() == 0 || b.GetHeight() == 0)
		data = NULL;
	else
		data = new Data(b);
}

void Image::Clear()
{
	if(data)
		data->Release();
	data = NULL;
}

Image& Image::operator=(ImageBuffer& img)
{
	if(data)
		data->Release();
	Set(img);
	return *this;
}

Image& Image::operator=(const Image& img)
{
	Data *d = data;
	data = img.data;
	if(data)
		data->Retain();
	if(d)
		d->Release();
	return *this;
}

const RGBA* Image::operator~() const
{
	return data ? ~data->buffer : NULL;
}

Image::operator const RGBA*() const
{
	return data ? ~data->buffer : NULL;
}

const RGBA* Image::operator[](int i) const
{
	ASSERT(data);
	return data->buffer[i];
}

Size Image::GetSize() const
{
	return data ? data->buffer.GetSize() : Size(0, 0);
}

int Image::GetLength() const
{
	return data ? data->buffer.GetLength() : 0;
}

Point Image::GetHotSpot() const
{
	return data ? data->buffer.GetHotSpot() : Point(0, 0);
}

Point Image::Get2ndSpot() const
{
	return data ? data->buffer.Get2ndSpot() : Point(0, 0);
}

Size Image::GetDots() const
{
	return data ? data->buffer.GetDots() : Size(0, 0);
}

int Image::GetKindNoScan() const
{
	return data ? data->buffer.GetKind() : IMAGE_EMPTY;
}

int Image::Data::GetKind()
{
	int k = buffer.GetKind();
	if(k != IMAGE_UNKNOWN)
		return k;
	k = buffer.ScanKind();
	buffer.SetKind(k);
	return k;
}

int Image::GetKind() const
{
	return data ? data->GetKind() : IMAGE_EMPTY;
}

void Image::PaintImage(Draw& w, int x, int y, const Rect& src, Color c) const
{
	if(data)
		data->Paint(w, x, y, src, c);
}

void Image::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	Size sz = GetSize();
	Point p = GetHotSpot();
	Size dots = GetDots();
	s % sz % p % dots;
	int len = sz.cx * sz.cy;
	if(s.IsLoading())
		if(len) {
			ImageBuffer b(sz);
			if(!s.GetAll(~b, len * sizeof(RGBA)))
				s.SetError();
			b.SetDots(dots);
			b.SetHotSpot(p);
			*this = b;
		}
		else
			Clear();
	else
		s.Put(~*this, len * sizeof(RGBA));
}

INITBLOCK {
	RichValue<Image>::Register();
}

bool Image::operator==(const Image& img) const
{
	if(GetLength() != img.GetLength())
		return false;
	return memcmp(~*this, ~img, GetLength() * sizeof(RGBA)) == 0;
}

bool Image::operator!=(const Image& img) const
{
	return !operator==(img);
}

dword Image::GetHashValue() const
{
	return memhash(~*this, GetLength() * sizeof(RGBA));
}

Image::Image(const Image& img)
{
	data = img.data;
	if(data)
		data->Retain();
}

Image::Image(Image (*fn)())
{
	data = NULL;
	*this = (*fn)();
}

Image::Image(const Value& src)
{
	data = NULL;
	if(!IsNull(src))
		*this = RawValue<Image>::Extract(src);
}

Image::Image(ImageBuffer& b)
{
	Set(b);
}

Image::~Image()
{
	if(data)
		data->Release();
}

Image::Image(const Init& init)
{
	ASSERT(init.info[0] >= 1);
	Size sz;
	sz.cx = Peek32le(init.info + 1);
	sz.cy = Peek32le(init.info + 5);
	ImageBuffer b(sz);
	int i = 0;
	while(i < init.scan_count) {
		UnpackRLE(b[i], (const byte *)init.scans[i], sz.cx);
		i++;
	}
	while(i < sz.cy)
		memset(b[i++], 0, sizeof(RGBA) * sz.cx);
	b.SetHotSpot(Point(Peek32le(init.info + 9), Peek32le(init.info + 13)));
	Set(b);
}

String Image::ToString() const
{
	return String("Image ").Cat() << GetSize();
}

Link<Image::Data>     Image::Data::ResData[1];
int                   Image::Data::ResCount;

Image::Data::Data(ImageBuffer& b)
:	buffer(b)
{
	paintcount = 0;
	paintonly = false;
	refcount = 1;
	INTERLOCKED {
		static int64 gserial;
		serial = ++gserial;
	}
	SysInit();
}

Image::Data::~Data()
{
	DrawLock __;
	SysRelease();
	Unlink();
}

void Image::Data::PaintOnlyShrink()
{
	if(paintonly) {
		LTIMING("PaintOnlyShrink");
		DrawLock __;
		DropPixels___(buffer);
		ResCount -= GetResCount();
		Unlink();
	}
}

Draw& ImageDraw::Alpha()
{
	if(!has_alpha) {
		alpha.DrawRect(size, GrayColor(0));
		has_alpha = true;
	}
	return alpha;
}

static void sMultiply(ImageBuffer& b, int (*op)(RGBA *t, const RGBA *s, int len))
{
	if(b.GetKind() != IMAGE_OPAQUE && b.GetKind() != IMAGE_EMPTY)
		(*op)(~b, ~b, b.GetLength());
}

void Premultiply(ImageBuffer& b)
{
	sMultiply(b, Premultiply);
}

void Unmultiply(ImageBuffer& b)
{
	sMultiply(b, Unmultiply);
}

static Image sMultiply(const Image& img, int (*op)(RGBA *t, const RGBA *s, int len))
{
	int k = img.GetKind();
	if(k == IMAGE_OPAQUE || k == IMAGE_EMPTY)
		return img;
	ImageBuffer ib(img.GetSize());
	ib.SetHotSpot(img.GetHotSpot());
	ib.Set2ndSpot(img.Get2ndSpot());
	ib.SetKind(Premultiply(~ib, ~img, ib.GetLength()));
	return ib;
}

Image Premultiply(const Image& img)
{
	return sMultiply(img, Premultiply);
}

Image Unmultiply(const Image& img)
{
	return sMultiply(img, Unmultiply);
}

void SetPaintOnly___(Image& m)
{
	if(m.data && m.data->refcount == 1)
		m.data->paintonly = true;
}

void Iml::Init(int n)
{
	for(int i = 0; i < n; i++)
		map.Add(name[i]);
}

void Iml::Reset()
{
	int n = map.GetCount();
	map.Clear();
	Init(n);
}

void Iml::Set(int i, const Image& img)
{
	map[i].image = img;
	map[i].loaded = true;
}

static StaticCriticalSection sImgImlLock;

Image Iml::Get(int i)
{
	IImage& m = map[i];
	if(!m.loaded) {
		INTERLOCKED_(sImgImlLock) {
			if(data.GetCount()) {
				int ii = 0;
				for(;;) {
					const Data& d = data[ii];
					if(i < d.count) {
						static const char   *cached_data;
						static Vector<Image> cached;
						if(cached_data != d.data) {
							cached_data = d.data;
							cached = UnpackImlData(String(d.data, d.len));
							if(premultiply)
								for(int i = 0; i < cached.GetCount(); i++)
									cached[i] = Premultiply(cached[i]);
						}
						m.image = cached[i];
						break;
					}
					i -= d.count;
					ii++;
				}
			}
			else
				m.image = Premultiply(Image(img_init[i]));
		}
		m.loaded = true;
	}
	return m.image;
}

#ifdef _DEBUG
int  Iml::GetBinSize() const
{
	int size = 0;
	for(int i = 0; i < map.GetCount(); i++) {
		const Image::Init& init = img_init[i];
		size += (int)strlen(name[i]) + 1 + 24;
		for(int q = 0; q < init.scan_count; q++)
			size += (int)strlen(init.scans[q]);
	}
	return size;
}
#endif

Iml::Iml(const Image::Init *img_init, const char **name, int n)
:	img_init(img_init),
	name(name)
{
#ifdef flagCHECKINIT
	RLOG("Constructing iml " << *name);
#endif
	premultiply = true;
	Init(n);
}

void Iml::AddData(const byte *_data, int len, int count)
{
	Data& d = data.Add();
	d.data = (const char *)_data;
	d.len = len;
	d.count = count;
	data.Shrink();
}

static StaticCriticalSection sImgMapLock;

static VectorMap<String, Iml *>& sImgMap()
{
	static VectorMap<String, Iml *> x;
	return x;
}

void Register(const char *imageclass, Iml& list)
{
#ifdef flagCHECKINIT
	RLOG("Registering iml " << imageclass);
#endif
	INTERLOCKED_(sImgMapLock)
		sImgMap().GetAdd(imageclass) = &list;
}

int GetImlCount()
{
	int q;
	INTERLOCKED_(sImgMapLock)
		q = sImgMap().GetCount();
	return q;
}

Iml& GetIml(int i)
{
	return *sImgMap()[i];
}

void Iml::Enter()
{
	sImgMapLock.Enter();
}

void Iml::Leave()
{
	sImgMapLock.Leave();
}

String GetImlName(int i)
{
	String x;
	INTERLOCKED_(sImgMapLock)
		x = sImgMap().GetKey(i);
	return x;
}

int FindIml(const char *name)
{
	int q;
	INTERLOCKED_(sImgMapLock)
		q = sImgMap().Find(name);
	return q;
}

Image GetImlImage(const char *name)
{
	Image m;
	const char *w = strchr(name, ':');
	if(w) {
		int q = FindIml(String(name, w));
		if(q >= 0) {
			INTERLOCKED_(sImgMapLock) {
				Iml& iml = *sImgMap()[q];
				while(*w == ':')
					w++;
				q = iml.Find(w);
				if(q >= 0)
					m = iml.Get(q);
			}
		}
	}
	return m;
}

void SetImlImage(const char *name, const Image& m)
{
	const char *w = strchr(name, ':');
	if(w) {
		int q = FindIml(String(name, w));
		if(q >= 0) {
			INTERLOCKED_(sImgMapLock) {
				Iml& iml = *sImgMap()[q];
				while(*w == ':')
					w++;
				q = iml.Find(w);
				if(q >= 0)
					iml.Set(q, m);
			}
		}
	}
}

String StoreImageAsString(const Image& img)
{
	if(img.GetKind() == IMAGE_EMPTY)
		return Null;
	int type = img.GetKind() == IMAGE_OPAQUE ? 3 : 4;
	StringStream ss;
	ss.Put(type);
	Size sz = img.GetSize();
	ss.Put16le(sz.cx);
	ss.Put16le(sz.cy);
	Point p = img.GetHotSpot();
	ss.Put16le(p.x);
	ss.Put16le(p.y);
	Size dots = img.GetDots();
	ss.Put16le(dots.cx);
	ss.Put16le(dots.cy);
	const RGBA *s = img;
	const RGBA *e = s + img.GetLength();
	Buffer<byte> b(type * img.GetLength());
	byte *t = b;
	if(type == 3)
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			s++;
		}
	else
		while(s < e) {
			*t++ = s->r;
			*t++ = s->g;
			*t++ = s->b;
			*t++ = s->a;
			s++;
		}
	MemReadStream m(b, type * img.GetLength());
	ZCompress(ss, m);
	return ss;
}

Image  LoadImageFromString(const String& src)
{
	if(src.GetLength() < 13)
		return Null;
	StringStream ss(src);
	int type = ss.Get();
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	if(sz.cx < 0 || sz.cy < 0)
		return Null;
	Point p;
	p.x = ss.Get16le();
	p.y = ss.Get16le();
	if(p.x < 0 || p.y < 0)
		return Null;
	Size dots;
	dots.cx = ss.Get16le();
	dots.cy = ss.Get16le();
	if(dots.cx < 0 || dots.cy < 0)
		return Null;
	StringStream out;
	ZDecompress(out, ss);
	String data = out;
	if(data.GetLength() != type * sz.cx * sz.cy)
		return Image();
	ImageBuffer ib(sz);
	ib.SetHotSpot(p);
	ib.SetDots(dots);
	RGBA *t = ib;
	const RGBA *e = t + ib.GetLength();
	const byte *s = data;
	if(type == 3)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = 255;
			t++;
		}
	else
	if(type == 4)
		while(t < e) {
			t->r = *s++;
			t->g = *s++;
			t->b = *s++;
			t->a = *s++;
			t++;
		}
	else
		return Image();
	return ib;
}

Size GetImageStringSize(const String& src)
{
	if(src.GetLength() < 13)
		return Size(0, 0);
	StringStream ss(src);
	ss.Get();
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}

Size GetImageStringDots(const String& src)
{
	if(src.GetLength() < 13)
		return Size(0, 0);
	StringStream ss(src);
	ss.SeekCur(9);
	Size sz;
	sz.cx = ss.Get16le();
	sz.cy = ss.Get16le();
	return sz;
}

END_UPP_NAMESPACE
