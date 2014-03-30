#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)   // DLOG(x)

Vector<Callback>  Ctrl::hotkey;
Vector<dword>     Ctrl::keyhot;
Vector<dword>     Ctrl::modhot;

Vector<Ctrl::Win> Ctrl::wins;

Ptr<Ctrl>         Ctrl::activeCtrl;

int        Ctrl::WndCaretTime;
bool       Ctrl::WndCaretVisible;

int Ctrl::FindId(int id)
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
    GuiLock __;
    static bool b = gdk_display_supports_composite(gdk_display_get_default());
    return b;
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	GuiLock __;
	Vector<Ctrl *> h;
	for(int i = 0; i < wins.GetCount(); i++)
		h.Add(wins[i].ctrl);
	return h;
}

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
		top = topctrl->top;
	if(top && id != top->cursor_id) {
		top->cursor_id = id;
		int64 aux = image.GetAuxData();
		GdkCursor *c = NULL;
		if(aux)
			c = gdk_cursor_new((GdkCursorType)(aux - 1));
		else
		if(IsNull(image))
			c = gdk_cursor_new(GDK_BLANK_CURSOR);
		else {
			Point p = image.GetHotSpot();
			ImageGdk m;
			m.Set(image);
			GdkPixbuf *pb = m;
			if(pb)
				c = gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pb, p.x, p.y);
		}
		if(c && topctrl->IsOpen()) {
			gdk_window_set_cursor(topctrl->gdk(), c);
			gdk_cursor_unref(c);
			gdk_flush(); // Make it visible immediately
		}
	}
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	return IsOpen() ? top->owner : NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	if(focusCtrl)
		return focusCtrl->GetTopCtrl();
	return activeCtrl;
}

// Vector<Callback> Ctrl::hotkey;

#ifndef GDK_WINDOWING_X11

// There is no generic support in GTK for HotKey

int Ctrl::RegisterSystemHotKey(dword key, Callback cb)
{
	return -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
}

#endif

void  Ctrl::AnimateCaret()
{
	GuiLock __;
	int v = !(((GetTickCount() - WndCaretTime) / 500) & 1);
	if(v != WndCaretVisible) {
		WndCaretVisible = v;
		RefreshCaret();
	}
}

void Ctrl::PaintCaret(SystemDraw& w)
{
	GuiLock __;
	LLOG("PaintCaret " << Name() << ", caretCtrl: " << caretCtrl << ", WndCaretVisible: " << WndCaretVisible);
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	LLOG("SetCaret " << Name());
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	if(this == caretCtrl) {
		WndCaretTime = GetTickCount();
		RefreshCaret();
		AnimateCaret();
	}
}

void Ctrl::SyncCaret() {
	GuiLock __;
	if(focusCtrl != caretCtrl) {
		LLOG("SyncCaret DO " << Upp::Name(caretCtrl) << " -> " << Upp::Name(focusCtrl));
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	if(IsOpen()) {
		gint x, y;
		gdk_window_get_position(gdk(), &x, &y);
	#if GTK_CHECK_VERSION(2, 24, 0)
		gint width = gdk_window_get_width(gdk());
		gint height = gdk_window_get_height(gdk());
	#else
		gint width, height;
		gdk_drawable_get_size(gdk(), &width, &height);
	#endif
		return RectC(x, y, width, height);
	}
	return Null;
}

void Ctrl::WndShow(bool b)
{
	GuiLock __;
	LLOG("WndShow " << Name() << ", " << b);
	if(IsOpen()) {
		if(b)
			gtk_widget_show_now(top->window);
		else
			gtk_widget_hide(top->window);
		StateH(SHOW);
	}
}

bool Ctrl::IsWndOpen() const {
	GuiLock __;
	return top && top->window && top->window->window;
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
}

Rect Ctrl::GetWorkArea() const
{
	GuiLock __;
	static Array<Rect> rc;
	if(rc.IsEmpty()) 
		GetWorkArea(rc);
	
	Point pt = GetScreenRect().TopLeft();
	for (int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	GdkScreen *s = gdk_screen_get_default();
	int n = gdk_screen_get_n_monitors(s);
	rc.Clear();
	Vector<int> netwa;
	for(int i = 0; i < n; i++) {
		GdkRectangle rr;
		Rect r;
#if GTK_CHECK_VERSION (3, 3, 5) // U++ does not work with gtk3 yet, but be prepared
		gdk_screen_get_monitor_workarea(s, i, &rr);
		r = RectC(r.x, r.y, r.width, r.height);
#else
		gdk_screen_get_monitor_geometry (s, i, &rr);
		r = RectC(rr.x, rr.y, rr.width, rr.height);
	#ifdef GDK_WINDOWING_X11
		if(i == 0)
			netwa = GetPropertyInts(gdk_screen_get_root_window(gdk_screen_get_default()),
			                        "_NET_WORKAREA");
		if(netwa.GetCount())
			r = r & RectC(netwa[0], netwa[1], netwa[2], netwa[3]);
	#endif
#endif
		rc.Add(r);
	}
}

Rect Ctrl::GetVirtualWorkArea()
{
	GuiLock __;
	static Rect r;
	if(r.right == 0) {
		r = GetPrimaryWorkArea();
		Array<Rect> rc;
		GetWorkArea(rc);
		for(int i = 0; i < rc.GetCount(); i++)
			r |= rc[i];
	}
	return r;
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	static Rect r;
	if(r.right == 0) {
		gint x, y, width, height;
		gdk_window_get_geometry(gdk_screen_get_root_window(gdk_screen_get_default()),
	    	                    &x, &y, &width, &height, NULL);
	    r = RectC(x, y, width, height);
	}
	return r;
}

Rect Ctrl::GetPrimaryWorkArea()
{
	GuiLock __;
	static Rect r;
	if (r.right == 0) {
		Array<Rect> rc;
		GetWorkArea(rc);
#if GTK_CHECK_VERSION(2, 20, 0)
		int primary = gdk_screen_get_primary_monitor(gdk_screen_get_default());
#else
		int primary = 0;
#endif
		primary >= 0 && primary < rc.GetCount() ? r = rc[primary] : r = GetVirtualScreenArea();
	}
	return r;
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

void Ctrl::FocusSync()
{
	GuiLock __;
	if(focusCtrlWnd && focusCtrlWnd->IsOpen() && gtk_window_is_active(focusCtrlWnd->gtk()))
		return;
	Ptr<Ctrl> focus = NULL;
	static Ptr<Ctrl> ctrl;
	for(int i = 0; i < wins.GetCount(); i++)
		if(gtk_window_is_active((GtkWindow *)wins[i].gtk)) {
			focus = wins[i].ctrl;
			break;
		}
	if(focus != ctrl) {
		if(ctrl)
			ctrl->KillFocusWnd();
		ctrl = focus;
		if(ctrl)
			ctrl->SetFocusWnd();
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
		int t0 = msecs();
		while(!gtk_window_is_active(gtk()) && msecs() - t0 < 500) // Wait up to 500ms for window to become active - not ideal, but only possibility
			FetchEvents(true);
		FocusSync();
	}
	return true;
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	LLOG("WndInvalidateRect " << r);
	gdk_window_invalidate_rect(gdk(), GdkRect(r), TRUE);
//	gtk_widget_queue_draw_area(top->window, r.left, r.top, r.GetWidth(), r.GetHeight());
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
		Event& e = Events[i];
		if(e.type == GDK_CONFIGURE && this_ && top->id == e.windowid) {
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
	LLOG("WndSetPos0 " << rect);
	GuiLock __;
	if(!IsOpen())
		return;
	Ptr<Ctrl> this_ = this;
	SweepConfigure(false); // Remove any previous GDK_CONFIGURE for this window
	if(!this_ || !IsOpen())
		return;
//	gtk_window_move(gtk(), rect.left, rect.top);
//	gtk_window_resize(gtk(), rect.GetWidth(), rect.GetHeight());
	gdk_window_move_resize(gdk(), rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
	int t0 = msecs();
	do { // Wait up to 500ms for corresponding GDK_CONFIGURE to arrive
		if(SweepConfigure(true))
			break;
	}
	while(msecs() - t0 < 500);
	LLOG("-- WndSetPos0 " << rect << " " << msecs() - t0);
}

void Ctrl::WndEnable(bool b)
{
	GuiLock __;
	if(IsOpen()) {
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
	gdk_window_process_updates(gdk(), TRUE);
	FetchEvents(FALSE); // Should pickup GDK_EXPOSE and repaint the window
	gdk_flush();
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

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	EnterGuiMutex();
	Ctrl *top = ctrl->GetTopCtrl();
	cr = gdk_cairo_create(top->gdk());
	Clipoff(ctrl->GetScreenView() - top->GetScreenRect().TopLeft());
}

ViewDraw::~ViewDraw()
{
	cairo_destroy(cr);
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

END_UPP_NAMESPACE

#endif
