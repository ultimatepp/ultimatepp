#include <CtrlCore/CtrlCore.h>

#ifdef GUI_TELPP

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
#define IMAGEFILE <Telpp/FB.iml>
#include <Draw/iml_source.h>

#define STD_CURSOR(name, sdl) \
Image Image::name() { static Image img; ONCELOCK { img = FBImg::name(); img.SetAuxData(sdl + 1); } return img; }

STD_CURSOR(Arrow, 1)
STD_CURSOR(Wait, 2)
STD_CURSOR(IBeam, 3)
STD_CURSOR(No, 4)
STD_CURSOR(SizeAll, 5)
STD_CURSOR(SizeHorz, 6)
STD_CURSOR(SizeVert, 7)
STD_CURSOR(SizeTopLeft, 8)
STD_CURSOR(SizeTop, 9)
STD_CURSOR(SizeTopRight, 10)
STD_CURSOR(SizeLeft, 11)
STD_CURSOR(SizeRight, 12)
STD_CURSOR(SizeBottomLeft, 13)
STD_CURSOR(SizeBottom, 14)
STD_CURSOR(SizeBottomRight, 15)
STD_CURSOR(Hand, 16)

END_UPP_NAMESPACE

#endif
