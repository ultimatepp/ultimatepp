#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

//#include <shellapi.h>

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)
#define LLOG(x)

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels);
void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff);

struct ImageSysData {
	Image            img;
	cairo_surface_t *surface;
	
	void Init(const Image& img);
	~ImageSysData();
};

void ImageSysData::Init(const Image& m)
{
	img = m;
	Size isz = img.GetSize();
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, isz.cx);
	for(int yy = 0; yy < isz.cy; yy++) {
		Copy((RGBA *)a, img[yy], isz.cx);
		a += stride;
	}
	cairo_surface_mark_dirty(surface);
	SysImageRealized(img);
}

ImageSysData::~ImageSysData()
{
	SysImageReleased(img);
	cairo_surface_destroy(surface);
}

struct ImageSysDataMaker : LRUCache<ImageSysData, int64>::Maker {
	Image img;

	virtual int64  Key() const                      { return img.GetSerialId(); }
	virtual int    Make(ImageSysData& object) const { object.Init(img); return img.GetLength(); }
};

void SystemDraw::SysDrawImageOp(int x, int y, const Image& img, Color color)
{
	GuiLock __;
	if(img.GetLength() == 0)
		return;
	DLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << x << ", " << y << ", "<< img.GetSize());
	ImageSysDataMaker m;
	static LRUCache<ImageSysData, int64> cache;
	LLOG("SysImage cache pixels " << cache.GetSize() << ", count " << cache.GetCount());
	m.img = img;
	ImageSysData& sd = cache.Get(m);
	if(!IsNull(color)) {
		SetColor(color);
		cairo_mask_surface(cr, sd.surface, x, y);
	}
	else {
		cairo_set_source_surface(cr, sd.surface, x, y);
		cairo_paint(cr);
	}
	cache.Shrink(4 * 1024 * 768, 1000); // Cache must be after Paint because of PaintOnly!
}

void ImageDraw::Init(Size sz)
{
	isz = sz;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cr = cairo_create(surface);
	alpha_surface = NULL;
}

Draw& ImageDraw::Alpha()
{
	if(!alpha_surface) {
		alpha_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
		alpha.cr = cairo_create(alpha_surface);
	}
	return alpha;
}

void ImageDraw::FetchStraight(ImageBuffer& b) const
{
	cairo_surface_flush(surface);
	byte *a = (byte *)cairo_image_surface_get_data(surface);
	int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, isz.cx);
	RGBA *t = b;
	byte *aa = NULL;
	if(alpha_surface) {
		cairo_surface_flush(alpha_surface);
		aa = (byte *)cairo_image_surface_get_data(alpha_surface);
	}
	for(int yy = 0; yy < isz.cy; yy++) {
		RGBA *s = (RGBA *)a;
		RGBA *e = s + isz.cx;
		if(aa) {
			RGBA *ss = (RGBA *)aa;
			while(s < e) {
				*t = *s++;
				(t++)->a = (ss++)->r;
			}
			aa += stride;
		}
		else
			while(s < e) {
				*t = *s++;
				(t++)->a = 255;
			}
		a += stride;
	}
}

ImageDraw::operator Image() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	Premultiply(img);
	return img;
}

Image ImageDraw::GetStraight() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	return img;
}

ImageDraw::ImageDraw(Size sz)
{
	Init(sz);
}

ImageDraw::ImageDraw(int cx, int cy)
{
	Init(Size(cx, cy));
}

ImageDraw::~ImageDraw()
{
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	if(alpha_surface) {
		cairo_destroy(alpha.cr);
		cairo_surface_destroy(alpha_surface);
	}
}

void BackDraw::Put(SystemDraw& w, int x, int y)
{
}

void BackDraw::Create(SystemDraw& w, int cx, int cy)
{
}

void BackDraw::Destroy()
{
}


Image Image::Arrow() { return Null; }
Image Image::Hand() { return Null; }
Image Image::Wait() { return Null; }
Image Image::IBeam() { return Null; }
Image Image::No() { return Null; }
Image Image::SizeAll() { return Null; }
Image Image::SizeHorz() { return Null; }
Image Image::SizeVert() { return Null; }
Image Image::SizeTopLeft() { return Null; }
Image Image::SizeTop() { return Null; }
Image Image::SizeTopRight() { return Null; }
Image Image::SizeLeft() { return Null; }
Image Image::SizeRight() { return Null; }
Image Image::SizeBottomLeft() { return Null; }
Image Image::SizeBottom() { return Null; }
Image Image::SizeBottomRight() { return Null; }

END_UPP_NAMESPACE

#endif
