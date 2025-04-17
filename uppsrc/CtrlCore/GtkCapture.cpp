#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

#define LLOG(x)   //   DLOG(x)

Ptr<Ctrl> Ctrl::grabwindow;
Ptr<Ctrl> Ctrl::grabpopup;

#if GTK_CHECK_VERSION(3, 20, 0)
GdkSeat *Ctrl::GetSeat()
{
	return gdk_display_get_default_seat(gdk_display_get_default());
}
#endif

GdkDevice *Ctrl::GetMouseDevice()
{
#if GTK_CHECK_VERSION(3, 20, 0)
	return gdk_seat_get_pointer(GetSeat());
#else
	return gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif
}

bool Ctrl::MouseIsGrabbed()
{
	return gdk_display_device_is_grabbed(gdk_display_get_default(), GetMouseDevice());
}

bool Ctrl::GrabMouse()
{
	return
#if GTK_CHECK_VERSION(3, 20, 0)
		gdk_seat_grab(GetSeat(), gdk(), GDK_SEAT_CAPABILITY_ALL_POINTING, true, NULL, NULL, NULL, 0)
#else
		gdk_device_grab(GetMouseDevice(), gdk(), GDK_OWNERSHIP_APPLICATION, true, (GdkEventMask)0, NULL, GDK_CURRENT_TIME)
#endif
	    == GDK_GRAB_SUCCESS;
}

void Ctrl::UngrabMouse()
{
#if GTK_CHECK_VERSION(3, 20, 0)
		gdk_seat_ungrab(GetSeat());
#else
		gdk_device_ungrab(GetMouseDevice(), GDK_CURRENT_TIME);
#endif
}

void Ctrl::StopGrabPopup()
{
	if(grabpopup && MouseIsGrabbed()) {
		UngrabMouse();
		grabpopup = NULL;
	}
}

void Ctrl::StartGrabPopup()
{
	if(activePopup.GetCount() && !grabpopup) {
		Ctrl *w = activePopup[0];
		if(w && w->IsOpen()) {
			ReleaseWndCapture0();
			// NOTE: On Wayland GrabMouse() is broken and shouldn't be used.
			if(IsWayland() || w->GrabMouse()) {
				grabpopup = w;
			}
		}
	}
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("SetWndCapture " << Name());
	if(!IsOpen())
		return false;
	StopGrabPopup();
	ReleaseWndCapture();
	if(GrabMouse()) {
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
		UngrabMouse();
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
	return this == grabwindow && grabwindow->IsOpen() && MouseIsGrabbed();
}

void Ctrl::CaptureSync()
{
	if(grabwindow && grabwindow->IsOpen() && !MouseIsGrabbed() && !grabwindow->GrabMouse())
		grabwindow = NULL;
}

};

#endif
