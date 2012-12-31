#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)   //   DLOG(x)

Ptr<Ctrl> Ctrl::grabwindow;
Ptr<Ctrl> Ctrl::grabpopup;

void Ctrl::StopGrabPopup()
{
	if(grabpopup && gdk_pointer_is_grabbed()) {
		gdk_pointer_ungrab(CurrentTime);
		grabpopup = NULL;
	}
}

void Ctrl::StartGrabPopup()
{
	if(activePopup.GetCount() && !grabpopup) {
		Ctrl *w = activePopup[0];
		if(w) {
			ReleaseWndCapture0();
			if(gdk_pointer_grab(w->gdk(), FALSE,
							    GdkEventMask(GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK|GDK_POINTER_MOTION_MASK),
							    NULL, NULL, CurrentTime) == GDK_GRAB_SUCCESS)
				grabpopup = w;
		}
	}
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("SetWndCapture " << Name());
	StopGrabPopup();
	ReleaseWndCapture();
	if(gdk_pointer_grab(gdk(), FALSE,
					    GdkEventMask(GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK|GDK_POINTER_MOTION_MASK),
					    NULL, NULL, CurrentTime) == GDK_GRAB_SUCCESS) {
		grabwindow = this;
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture0()
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("ReleaseWndCapture");
	if(grabwindow) {
		gdk_pointer_ungrab(CurrentTime);
		grabwindow = NULL;
		StartGrabPopup();
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	return ReleaseWndCapture0();
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return this == grabwindow && gdk_pointer_is_grabbed();
}

END_UPP_NAMESPACE

#endif
