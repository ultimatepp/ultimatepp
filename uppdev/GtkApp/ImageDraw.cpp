#include "GtkApp.h"

void CairoImageDraw::Init(Size sz)
{
	isz = sz;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
	cr = cairo_create(surface);
	alpha_surface = NULL;
}

Draw& CairoImageDraw::Alpha()
{
	if(!alpha_surface) {
		alpha_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, isz.cx, isz.cy);
		alpha.cr = cairo_create(alpha_surface);
	}
	return alpha;
}

void CairoImageDraw::FetchStraight(ImageBuffer& b) const
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

CairoImageDraw::operator Image() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	Premultiply(img);
	return img;
}

Image CairoImageDraw::GetStraight() const
{
	ImageBuffer img(isz);
	FetchStraight(img);
	return img;
}

CairoImageDraw::CairoImageDraw(Size sz)
{
	Init(sz);
}

CairoImageDraw::CairoImageDraw(int cx, int cy)
{
	Init(Size(cx, cy));
}

CairoImageDraw::~CairoImageDraw()
{
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	if(alpha_surface) {
		cairo_destroy(alpha.cr);
		cairo_surface_destroy(alpha_surface);
	}
}
