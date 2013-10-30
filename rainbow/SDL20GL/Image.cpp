#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SDL20GL

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
#define IMAGEFILE <SDL20GL/FB.iml>
#include <Draw/iml_source.h>

#define STD_CURSOR(name, sdl) \
Image Image::name() { static Image img; ONCELOCK { img = FBImg::name(); img.SetAuxData(sdl + 1); } return img; }

STD_CURSOR(Arrow, SDL_SYSTEM_CURSOR_ARROW)
STD_CURSOR(Wait, SDL_SYSTEM_CURSOR_WAIT)
STD_CURSOR(IBeam, SDL_SYSTEM_CURSOR_IBEAM)
STD_CURSOR(No, SDL_SYSTEM_CURSOR_NO)
STD_CURSOR(SizeAll, SDL_SYSTEM_CURSOR_SIZEALL)
STD_CURSOR(SizeHorz, SDL_SYSTEM_CURSOR_SIZEWE)
STD_CURSOR(SizeVert, SDL_SYSTEM_CURSOR_SIZENS)
STD_CURSOR(SizeTopLeft, SDL_SYSTEM_CURSOR_SIZENWSE)
STD_CURSOR(SizeTop, SDL_SYSTEM_CURSOR_SIZENS)
STD_CURSOR(SizeTopRight, SDL_SYSTEM_CURSOR_SIZENESW)
STD_CURSOR(SizeLeft, SDL_SYSTEM_CURSOR_SIZEWE)
STD_CURSOR(SizeRight, SDL_SYSTEM_CURSOR_SIZEWE)
STD_CURSOR(SizeBottomLeft, SDL_SYSTEM_CURSOR_SIZENWSE)
STD_CURSOR(SizeBottom, SDL_SYSTEM_CURSOR_SIZENS)
STD_CURSOR(SizeBottomRight, SDL_SYSTEM_CURSOR_SIZENESW)
STD_CURSOR(Hand, SDL_SYSTEM_CURSOR_HAND)

END_UPP_NAMESPACE

#endif
