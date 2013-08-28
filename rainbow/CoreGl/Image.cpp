#if defined(flagWINGL) || defined(flagLINUXGL)

#include <CtrlLib/CtrlLib.h>

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

#define IMAGECLASS CoreGlImg
#define IMAGEFILE <CoreGl/CoreGl.iml>
#include <Draw/iml_source.h>

Image Image::Arrow() { return CoreGlImg::arrow(); }
Image Image::Wait() { return CoreGlImg::wait(); }
Image Image::IBeam() { return CoreGlImg::ibeam(); }
Image Image::No() { return CoreGlImg::no(); }
Image Image::SizeAll() { return CoreGlImg::sizeall(); }
Image Image::SizeHorz() { return CoreGlImg::sizehorz(); }
Image Image::SizeVert() { return CoreGlImg::sizevert(); }
Image Image::SizeTopLeft() { return CoreGlImg::sizetopleft(); }
Image Image::SizeTop() { return CoreGlImg::sizetop(); }
Image Image::SizeTopRight() { return CoreGlImg::sizetopright(); }
Image Image::SizeLeft() { return CoreGlImg::sizeleft(); }
Image Image::SizeRight() { return CoreGlImg::sizeright(); }
Image Image::SizeBottomLeft() { return CoreGlImg::sizebottomleft(); }
Image Image::SizeBottom() { return CoreGlImg::sizebottom(); }
Image Image::SizeBottomRight() { return CoreGlImg::sizebottomright(); }
Image Image::Hand() { return CoreGlImg::hand(); }

END_UPP_NAMESPACE

#endif
