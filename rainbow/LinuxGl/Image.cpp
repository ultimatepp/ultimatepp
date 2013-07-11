#include <CtrlLib/CtrlLib.h>

#ifdef GUI_LINUXGL

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

#define IMAGECLASS LinuxGlImg
#define IMAGEFILE <LinuxGl/LinuxGl.iml>
#include <Draw/iml_source.h>

Image Image::Arrow() { return LinuxGlImg::arrow(); }
Image Image::Wait() { return LinuxGlImg::wait(); }
Image Image::IBeam() { return LinuxGlImg::ibeam(); }
Image Image::No() { return LinuxGlImg::no(); }
Image Image::SizeAll() { return LinuxGlImg::sizeall(); }
Image Image::SizeHorz() { return LinuxGlImg::sizehorz(); }
Image Image::SizeVert() { return LinuxGlImg::sizevert(); }
Image Image::SizeTopLeft() { return LinuxGlImg::sizetopleft(); }
Image Image::SizeTop() { return LinuxGlImg::sizetop(); }
Image Image::SizeTopRight() { return LinuxGlImg::sizetopright(); }
Image Image::SizeLeft() { return LinuxGlImg::sizeleft(); }
Image Image::SizeRight() { return LinuxGlImg::sizeright(); }
Image Image::SizeBottomLeft() { return LinuxGlImg::sizebottomleft(); }
Image Image::SizeBottom() { return LinuxGlImg::sizebottom(); }
Image Image::SizeBottomRight() { return LinuxGlImg::sizebottomright(); }
Image Image::Hand() { return LinuxGlImg::hand(); }

END_UPP_NAMESPACE

#endif
