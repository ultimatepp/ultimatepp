#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Ctrl::GuiPlatformConstruct()
{
}

void Ctrl::GuiPlatformDestruct()
{
}

void Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
}

bool Ctrl::GuiPlatformRefreshFrameSpecial(const Rect& r)
{
	return false;
}

bool Ctrl::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

void GuiPlatformAdjustDragImage(ImageBuffer&)
{
}

bool GuiPlatformHasSizeGrip()
{
	return true;
}

void GuiPlatformGripResize(TopWindow *q)
{
	if(q->IsOpen()) {
		Point p = GetMousePos();
		gtk_window_begin_resize_drag(q->gtk(), GDK_WINDOW_EDGE_SOUTH_EAST,
		                             1, p.x, p.y, Ctrl::CurrentTime);
	}
}

Color GuiPlatformGetScreenPixel(int x, int y)
{
	return Black;
}

void GuiPlatformAfterMenuPopUp()
{
}

String Ctrl::Name() const
{
	return Name0();
}

void Ctrl::InstallPanicBox()
{
}

END_UPP_NAMESPACE

#endif
