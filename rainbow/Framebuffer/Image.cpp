#include <CtrlLib/CtrlLib.h>

#ifdef GUI_FB

//#include <shellapi.h>

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	GuiLock __;
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
	GuiLock __;
}

struct Image::Data::SystemData {
};

void Image::Data::SysInitImp()
{
	SystemData& sd = Sys();
}

void Image::Data::SysReleaseImp()
{
	SystemData& sd = Sys();
}

Image::Data::SystemData& Image::Data::Sys() const
{
	ASSERT(sizeof(system_buffer) >= sizeof(SystemData));
	return *(SystemData *)system_buffer;
}

int  Image::Data::GetResCountImp() const
{
	SystemData& sd = Sys();
	return 0;
}

void Image::Data::PaintImp(SystemDraw& w, int x, int y, const Rect& src, Color c)
{
	GuiLock __;
	SystemData& sd = Sys();
}

Image ImageDraw::Get(bool pm) const
{
	ImageBuffer result(image.GetSize());
	const RGBA *e = image.End();
	const RGBA *p = ~image;
	RGBA *t = ~result;
	if(has_alpha) {
		const RGBA *a = ~alpha;
		while(p < e) {
			*t = *p++;
			(t++)->a = (a++)->r;
		}
		if(pm)
			Premultiply(result);
		result.SetKind(IMAGE_ALPHA);
	}
	else {
		while(p < e) {
			*t = *p++;
			(t++)->a = 255;
		}
	}
	return result;
}

Draw& ImageDraw::Alpha()
{
	has_alpha = true;
	return alpha_painter;
}


ImageDraw::ImageDraw(Size sz)
:	ImageDraw__(sz.cx, sz.cy),
	BufferPainter(image),
	alpha_painter(alpha)
{
	has_alpha = false;
}

ImageDraw::ImageDraw(int cx, int cy)
:	ImageDraw__(cx, cy),
	BufferPainter(image),
	alpha_painter(alpha)
{
	has_alpha = false;
}

Image Image::Arrow() { return CtrlImg::arrow(); }
Image Image::Wait() { return CtrlImg::arrow(); }
Image Image::IBeam() { return CtrlImg::arrow(); }
Image Image::No() { return CtrlImg::arrow(); }
Image Image::SizeAll() { return CtrlImg::arrow(); }
Image Image::SizeHorz() { return CtrlImg::arrow(); }
Image Image::SizeVert() { return CtrlImg::arrow(); }
Image Image::SizeTopLeft() { return CtrlImg::arrow(); }
Image Image::SizeTop() { return CtrlImg::arrow(); }
Image Image::SizeTopRight() { return CtrlImg::arrow(); }
Image Image::SizeLeft() { return CtrlImg::arrow(); }
Image Image::SizeRight() { return CtrlImg::arrow(); }
Image Image::SizeBottomLeft() { return CtrlImg::arrow(); }
Image Image::SizeBottom() { return CtrlImg::arrow(); }
Image Image::SizeBottomRight() { return CtrlImg::arrow(); }

END_UPP_NAMESPACE

#endif
