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
	LLOG("SysDrawImageOp " << img.GetSerialId() << ' ' << x << ", " << y << ", "<< img.GetSize());
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

Draw& ImageDraw::Alpha()
{
	if(!alpha_surface) {
		alpha_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
		alpha.cr = cairo_create(alpha_surface);
	}
	return alpha;
}

void CairoGet(ImageBuffer& b, Size isz, cairo_surface_t *surface, cairo_surface_t *alpha_surface)
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

void ImageDraw::FetchStraight(ImageBuffer& b) const
{
	CairoGet(b, isz, surface, alpha_surface);
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

void ImageDraw::Init(Size sz)
{
	isz = sz;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cr = cairo_create(surface);
	alpha_surface = NULL;
	del = true;
}

ImageDraw::ImageDraw(Size sz)
{
	Init(sz);
}

ImageDraw::ImageDraw(int cx, int cy)
{
	Init(Size(cx, cy));
}

ImageDraw::ImageDraw(cairo_t *cr_, Size sz)
{
	isz = sz;
	cr = cr_;
	surface = cairo_get_target(cr);
	alpha_surface = NULL;
	del = false;
}

ImageDraw::~ImageDraw()
{
	if(del) {
		cairo_destroy(cr);
		cairo_surface_destroy(surface);
		if(alpha_surface) {
			cairo_destroy(alpha.cr);
			cairo_surface_destroy(alpha_surface);
		}
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

#define FCURSOR_(x) { static Image h; ONCELOCK { h = CreateImage(Size(1, 1), Black); h.SetAuxData(x + 1); } return h; }

Image Image::Arrow() FCURSOR_(GDK_LEFT_PTR)
Image Image::Wait() FCURSOR_(GDK_WATCH)
Image Image::IBeam() FCURSOR_(GDK_XTERM)
Image Image::No() FCURSOR_(GDK_CIRCLE)
Image Image::SizeAll() FCURSOR_(GDK_FLEUR)
Image Image::SizeHorz() FCURSOR_(GDK_SB_H_DOUBLE_ARROW)
Image Image::SizeVert() FCURSOR_(GDK_DOUBLE_ARROW)
Image Image::SizeTopLeft() FCURSOR_(GDK_TOP_LEFT_CORNER)
Image Image::SizeTop() FCURSOR_(GDK_TOP_SIDE)
Image Image::SizeTopRight() FCURSOR_(GDK_TOP_RIGHT_CORNER)
Image Image::SizeLeft() FCURSOR_(GDK_LEFT_SIDE)
Image Image::SizeRight() FCURSOR_(GDK_RIGHT_SIDE)
Image Image::SizeBottomLeft() FCURSOR_(GDK_BOTTOM_LEFT_CORNER)
Image Image::SizeBottom() FCURSOR_(GDK_BOTTOM_SIDE)
Image Image::SizeBottomRight()  FCURSOR_(GDK_BOTTOM_RIGHT_CORNER)
Image Image::Cross() FCURSOR_(GDK_CROSSHAIR)
Image Image::Hand() FCURSOR_(GDK_HAND1)

END_UPP_NAMESPACE

#endif
