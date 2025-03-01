#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

#define LLOG(x)   // DLOG(x)

Vector<Event<> >  Ctrl::hotkey;
Vector<dword>     Ctrl::keyhot;
Vector<dword>     Ctrl::modhot;

Vector<Ctrl::Win> Ctrl::wins;

Ptr<Ctrl>         Ctrl::activeCtrl;

bool              Ctrl::invalids;

int Ctrl::FindGtkId(int id)
{
	for(int i = 0; i < wins.GetCount(); i++)
		if(wins[i].id == id)
			return i;
	return -1;
}

int Ctrl::FindCtrl(Ctrl *ctrl)
{
	for(int i = 0; i < wins.GetCount(); i++)
		if(wins[i].ctrl == ctrl)
			return i;
	return -1;
}

int Ctrl::FindGtkWindow(GtkWidget *gtk)
{
	for(int i = 0; i < wins.GetCount(); i++)
		if(wins[i].gtk == gtk)
			return i;
	return -1;
}

int Ctrl::FindGdkWindow(GdkWindow *gdk)
{
	for(int i = 0; i < wins.GetCount(); i++)
		if(wins[i].gdk == gdk)
			return i;
	return -1;
}

bool Ctrl::IsAlphaSupported()
{
	return false;
}

bool Ctrl::IsCompositedGui()
{
	return true; // limits some GUI effects that do not play well with advanced desktops
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	GuiLock __;
	Vector<Ctrl *> h;
	for(int i = 0; i < wins.GetCount(); i++)
		h.Add(wins[i].ctrl);
	return h;
}

cairo_surface_t *CreateCairoSurface(const Image& img);

void  Ctrl::SetMouseCursor(const Image& image)
{
	LLOG("SetMouseCursor");
	GuiLock __;
	int64 id = image.GetSerialId();
	Ctrl *topctrl = NULL;
	Top *top = NULL;
	if(mouseCtrl)
		topctrl = mouseCtrl->GetTopCtrl();
	else
		topctrl = GetActiveCtrl();
	if(topctrl)
		top = topctrl->GetTop();
	if(top && id != top->cursor_id) {
		top->cursor_id = id;
		int64 aux = image.GetAuxData();
		GdkCursor *c = NULL;
		if(aux)
			c = gdk_cursor_new_for_display(gdk_display_get_default(), (GdkCursorType)(aux - 1));
		else
		if(IsNull(image))
			c = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_BLANK_CURSOR);
		else {
			Point p = image.GetHotSpot();

#if GTK_CHECK_VERSION(3, 10, 0)
			cairo_surface_t *surface = CreateCairoSurface(image);
			double scale = SCL(1);
			cairo_surface_set_device_scale(surface, scale, scale);
			c = gdk_cursor_new_from_surface(gdk_display_get_default(), surface, p.x / scale, p.y / scale);
			cairo_surface_destroy(surface);
#else
			ImageGdk m;
			m.Set(image);
			GdkPixbuf *pb = m;
			if(pb)
				c = gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pb, p.x, p.y);
#endif
		}
		if(c && topctrl->IsOpen()) {
			gdk_window_set_cursor(topctrl->gdk(), c);
			g_object_unref(c);
			if(IsXWayland()) // xwayland is broken, need some paint to change the cursor...
				topctrl->Refresh(0, 0, 1, 1);
			gdk_display_flush(gdk_display_get_default()); // Make it visible immediately
		}
	}
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	Top *top = GetTop();
	return IsOpen() ? top->owner : NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	if(focusCtrl)
		return focusCtrl->GetTopCtrl();
	return activeCtrl;
}

// Vector<Event<> > Ctrl::hotkey;

#ifndef GDK_WINDOWING_X11

// There is no generic support in GTK for HotKey

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
{
	return -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
}

#endif

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;

	if(!IsOpen())
		return Null;

	gint x, y;
	gint width, height;

	if(IsWayland()) {
		if(top && utop->csd.IsEnabled()) {
			gdk_window_get_origin(gtk_widget_get_window(utop->drawing_area), &x, &y);
			width = gtk_widget_get_allocated_width(utop->drawing_area);
			height = gtk_widget_get_allocated_height(utop->drawing_area);
		}
		else
			gdk_window_get_geometry(gdk(), &x, &y, &width, &height);
	}
	else {
		gdk_window_get_position(gdk(), &x, &y);
		width = gdk_window_get_width(gdk());
		height = gdk_window_get_height(gdk());
	}

	return SCL(x, y, width, height);
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
	LLOG("WndShow " << Name() << ", " << b);
	Top *top = GetTop();
	if(IsOpen() && top) {
		if(b)
			gtk_widget_show_now(top->window);
		else
			gtk_widget_hide(top->window);
		StateH(SHOW);
	}
}

bool Ctrl::IsWndOpen() const {
	GuiLock __;
	const Top *top = GetTop();
	return top && top->window && gtk_widget_get_window(top->window);
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
}

Rect Ctrl::GetWorkArea() const
{
	return StdGetWorkArea();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	rc.Clear();
#if GTK_CHECK_VERSION(3, 22, 0)
	GdkDisplay *s = gdk_display_get_default();
	int n = gdk_display_get_n_monitors(s);
	Vector<int> netwa;
	for(int i = 0; i < n; i++) {
		GdkRectangle rr;
		auto *pMonitor = gdk_display_get_monitor(s, i);
		if(IsWayland())
			gdk_monitor_get_geometry(pMonitor, &rr);
		else
			gdk_monitor_get_workarea(pMonitor, &rr);
		rc.Add(SCL(rr.x, rr.y, rr.width, rr.height));
	}
#else
	GdkScreen *s = gdk_screen_get_default();
	int n = gdk_screen_get_n_monitors(s);
	Vector<int> netwa;
	for(int i = 0; i < n; i++) {
		GdkRectangle rr;
		Rect r;
		gdk_screen_get_monitor_workarea(s, i, &rr);
		r = RectC(rr.x, rr.y, rr.width, rr.height);
		rc.Add(r);
	}
#endif
}

Rect Ctrl::GetVirtualWorkArea()
{
	GuiLock __;
	Rect r = GetPrimaryWorkArea();
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		r |= rc[i];
	return r;
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	auto pRootWindow = gdk_screen_get_root_window(gdk_screen_get_default());
	if (!pRootWindow) {
		LLOG("Failed to obtain root window!");
		return Rect(0, 0, 1024, 768);
	}
#if GTK_CHECK_VERSION(3, 22, 0)
	if(IsWayland()) {
		GdkRectangle rr;
		auto *pDisplay = gdk_display_get_default();
		auto *pMonitor = gdk_display_get_monitor_at_window(pDisplay, pRootWindow);
		if (!pMonitor) {
			LLOG("Failed to obtain monitor!");
			return Rect(0, 0, 1024, 768);
		}
		gdk_monitor_get_geometry(pMonitor, &rr);
		return SCL(rr.x, rr.y, rr.width, rr.height);
	}
#endif
	if(IsWayland()) {
		LLOG("GTK Wayland backend not supported before 3.22 GTK version.");
		return Rect(0, 0, 1024, 768);
	}
	gint x, y, width, height;
	gdk_window_get_geometry(pRootWindow, &x, &y, &width, &height);
    return SCL(x, y, width, height);
}

Rect Ctrl::GetPrimaryWorkArea()
{
	GuiLock __;
#if GTK_CHECK_VERSION(3, 22, 0)
	if(IsWayland()) {
		// NOTE: WorkArea on Wayland is not available... Window manager decides where to put
		// windows.
		return GetVirtualScreenArea();
	}
	GdkRectangle rr;
	auto* display = gdk_display_get_default();
	gdk_monitor_get_workarea(gdk_display_get_primary_monitor(display), &rr);
	return SCL(rr.x, rr.y, rr.width, rr.height);
#else
	static Rect r;
	if (r.right == 0) {
		Array<Rect> rc;
		GetWorkArea(rc);
		int primary = gdk_screen_get_primary_monitor(gdk_screen_get_default());
		primary >= 0 && primary < rc.GetCount() ? r = rc[primary] : r = GetVirtualScreenArea();
	}
	return r;
#endif
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return GetPrimaryWorkArea();
}

int Ctrl::GetKbdDelay()
{
	GuiLock __;
	return 500;
}

int Ctrl::GetKbdSpeed()
{
	GuiLock __;
	return 1000 / 32;
}

void Ctrl::SetWndForeground()
{
	GuiLock __;
	if(IsOpen())
		gtk_window_present(gtk());
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	LLOG("IsWndForeground");
	return IsOpen() && gtk_window_is_active(gtk());
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	return IsOpen() && gtk_window_is_active(gtk());
}

#if 0
void Ctrl::FocusSync()
{
	GuiLock __;
	static Ptr<Ctrl> ctrl;
	if(focusCtrlWnd && focusCtrlWnd->IsOpen() && gtk_window_is_active(focusCtrlWnd->gtk())) {
		ctrl = focusCtrlWnd;
		return;
	}
	Ptr<Ctrl> focus = NULL;
	for(int i = 0; i < wins.GetCount(); i++)
		if(gtk_window_is_active((GtkWindow *)wins[i].gtk)) {
			focus = wins[i].ctrl;
			break;
		}
	if(focus != ctrl) {
		DLOG("FocusSync " << Upp::Name(ctrl) << ", focus: " << Upp::Name(focus));
		if(ctrl)
			ctrl->KillFocusWnd();
		ctrl = focus;
		if(ctrl)
			ctrl->SetFocusWnd();
		SyncCaret();
	}
}
#endif

void Ctrl::FocusSync()
{
	GuiLock __;
	static Ptr<Ctrl> ctrl;
	if(focusCtrlWnd && focusCtrlWnd->IsOpen() && gtk_window_is_active(focusCtrlWnd->gtk()))
		return;
	Ptr<Ctrl> focus = NULL;
	for(int i = 0; i < wins.GetCount(); i++)
		if(gtk_window_is_active((GtkWindow *)wins[i].gtk)) {
			focus = wins[i].ctrl;
			break;
		}
	if(focus != focusCtrlWnd) {
		LLOG("FocusSync " << Upp::Name(focusCtrlWnd) << " -> " << Upp::Name(focus));
		if(focusCtrlWnd && focusCtrlWnd->IsOpen())
			focusCtrlWnd->KillFocusWnd();
		if(focus)
			focus->SetFocusWnd();
		SyncCaret();
	}
}

bool Ctrl::SetWndFocus()
{
	GuiLock __;
	LLOG("SetWndFocus0 " << Upp::Name(this) << ", top: " << top);
	if(top) {
		LLOG("SetWndFocus0 DO gdk: " << gdk());
		SetWndForeground();
	#if 0 // this does not seem necessary
		int t0 = msecs();
		while(!gtk_window_is_active(gtk()) && msecs() - t0 < 500) // Wait up to 500ms for window to become active - not ideal, but only possibility
			FetchEvents(true);
	#endif
		FocusSync();
	}
	return true;
}

void Ctrl::DoCancelPreedit()
{
	if(!focusCtrl)
		return;
	Top *top = focusCtrl->GetTop();
	if(top)
		focusCtrl->HidePreedit();
	if(top && top->im_context) {
		gtk_im_context_reset(top->im_context);
		gtk_im_context_focus_out(top->im_context);
		gtk_im_context_focus_in(top->im_context);
	}
}

void WakeUpGuiThread();

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;

	Rect rr = r;
	if(scale > 1) {
		rr.left = r.left / 2;
		rr.top = r.top / 2;
		rr.right = (r.right + 1) / 2;
		rr.bottom = (r.bottom + 1) / 2;
	}

	if(IsWayland())
		rr.Inflate(2, 2); // TODO: This is temporary fix

	// as gtk3 dropped thread locking, we need to push invalid rectangles onto main loop
	for(Win& win : wins) {
		if(win.ctrl == this) {
			if(win.invalid.GetCount() && IsNull(win.invalid[0]))
				return;
			if(win.invalid.GetCount() > 40) { // keep things sane
				win.invalid.Clear();
				win.invalid.Add(Null);
			}
			else
				win.invalid.Add(rr);
			if(!invalids) {
				invalids = true;
				WakeUpGuiThread();
			}
		}
	}
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	GuiLock __;
	LLOG("ScrollView " << rect);
	WndInvalidateRect(r);
}

bool Ctrl::SweepConfigure(bool wait)
{
	Ptr<Ctrl> this_ = this;
	bool r = false;
	FetchEvents(wait);
	for(int i = 0; i < Events.GetCount() && this_; i++) {
		GEvent& e = Events[i];
		Top *top = GetTop();
		if(e.type == GDK_CONFIGURE && this_ && top && top->id == e.windowid) {
			Rect rect = e.value;
			LLOG("SweepConfigure " << rect);
			if(GetRect() != rect)
				SetWndRect(rect);
			r = true;
			e.type = EVENT_NONE;
		}
	}
	return r;
}

void Ctrl::WndSetPos(const Rect& rect)
{
	LLOG("WndSetPos " << UPP::Name(this) << " " << rect);
	GuiLock __;
	if(!IsOpen())
		return;
	Ptr<Ctrl> this_ = this;
	SweepConfigure(false); // Remove any previous GDK_CONFIGURE for this window
	if(!this_ || !IsOpen())
		return;

	Rect m(0, 0, 0, 0);
	if(dynamic_cast<TopWindow *>(this))
		m = GetFrameMargins();
	SetWndRect(rect);
	if(TopWindow *tw = dynamic_cast<TopWindow *>(this))
		tw->SyncSizeHints();
	gdk_window_move_resize(gdk(), LSC(rect.left - m.left), LSC(rect.top - m.top), LSC(rect.GetWidth()), LSC(rect.GetHeight()));
	LLOG("-- WndSetPos0 " << rect);
}

void Ctrl::WndEnable(bool b)
{
	GuiLock __;
	if(IsOpen()) {
		Top *top = GetTop();
		gtk_widget_set_sensitive(top->window, b);
		StateH(ENABLE);
	}
}

void Ctrl::WndUpdate(const Rect& r)
{
	GuiLock __;
	LLOG("WndUpdate0r " << r);
	WndUpdate(); // Not found a way how to update only part of window
}

void Ctrl::WndUpdate()
{
	GuiLock __;
	LLOG("WndUpdate0");
//	gdk_window_process_updates(gdk(), TRUE); // deprecated
	FetchEvents(FALSE); // Should pickup GDK_EXPOSE and repaint the window
	gdk_display_flush(gdk_display_get_default());
}

Rect Ctrl::GetDefaultWindowRect()
{
	GuiLock __;
	Rect r  = GetPrimaryWorkArea();
	Size sz = r.GetSize();

	static int pos = min(sz.cx / 10, 50);
	pos += 10;
	int cx = sz.cx * 2 / 3;
	int cy = sz.cy * 2 / 3;
	if(pos + cx + 50 > sz.cx || pos + cy + 50 > sz.cy)
		pos = 0;
	return RectC(r.left + pos + 20, r.top + pos + 20, cx, cy);
}

TopFrameDraw::TopFrameDraw(Ctrl *ctrl, const Rect& r)
{
	EnterGuiMutex();
	Ctrl *top = ctrl->GetTopCtrl();
#if GTK_CHECK_VERSION(3, 22, 0)
	cairo_rectangle_int_t rr;
	rr.x = Ctrl::LSC(r.left);
	rr.y = Ctrl::LSC(r.top);
	rr.width = Ctrl::LSC(r.GetWidth());
	rr.height = Ctrl::LSC(r.GetHeight());
	cairo_region_t *rg = cairo_region_create_rectangle(&rr);
	ctx = gdk_window_begin_draw_frame(top->gdk(), rg);
	cairo_region_destroy(rg);
	cr = gdk_drawing_context_get_cairo_context(ctx);
#else
	cr = gdk_cairo_create(top->gdk());
#endif
	cairo_scale(cr, Ctrl::LSC(1), Ctrl::LSC(1));
	Clipoff(r);
}

TopFrameDraw::~TopFrameDraw()
{
	FlushText();
#if GTK_CHECK_VERSION(3, 22, 0)
	gdk_window_end_draw_frame(gdk_drawing_context_get_window(ctx), ctx);
#else
	cairo_destroy(cr);
#endif
	LeaveGuiMutex();
}

Vector<WString> SplitCmdLine__(const char *cmd)
{
	Vector<WString> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else if(*cmd == '\"') {
			WString quoted;
			while(*++cmd && (*cmd != '\"' || *++cmd == '\"'))
				quoted.Cat(FromSystemCharset(String(cmd, 1)).ToWString());
			out.Add(quoted);
		}
		else {
			const char *begin = cmd;
			while((byte)*cmd > ' ')
				cmd++;
			out.Add(String(begin, cmd).ToWString());
		}
	return out;
}

}

#endif