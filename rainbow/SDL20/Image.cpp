#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SDL20

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

void SetSurface(SystemDraw& w, int x, int y, int cx, int cy, const RGBA *pixels)
{
	GuiLock __;
	// Empty as CanSetSurface is false
}

void SetSurface(SystemDraw& w, const Rect& dest, const RGBA *pixels, Size psz, Point poff)
{
	GuiLock __;
	// Empty as CanSetSurface is false
}

#define IMAGECLASS FBImg
#define IMAGEFILE <Framebuffer/FB.iml>
#include <Draw/iml_source.h>

Image Image::Arrow() { return FBImg::arrow(); }
Image Image::Wait() { return FBImg::wait(); }
Image Image::IBeam() { return FBImg::ibeam(); }
Image Image::No() { return FBImg::no(); }
Image Image::SizeAll() { return FBImg::sizeall(); }
Image Image::SizeHorz() { return FBImg::sizehorz(); }
Image Image::SizeVert() { return FBImg::sizevert(); }
Image Image::SizeTopLeft() { return FBImg::sizetopleft(); }
Image Image::SizeTop() { return FBImg::sizetop(); }
Image Image::SizeTopRight() { return FBImg::sizetopright(); }
Image Image::SizeLeft() { return FBImg::sizeleft(); }
Image Image::SizeRight() { return FBImg::sizeright(); }
Image Image::SizeBottomLeft() { return FBImg::sizebottomleft(); }
Image Image::SizeBottom() { return FBImg::sizebottom(); }
Image Image::SizeBottomRight() { return FBImg::sizebottomright(); }
Image Image::Hand() { return FBImg::hand(); }

END_UPP_NAMESPACE

#endif
