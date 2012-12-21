#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)      DLOG(x)
#define LOGTIMING 0

#ifdef _DEBUG
#define LOGMESSAGES 0
#endif

#define ELOGW(x)  // RLOG(GetSysTime() << ": " << x) // Only activate in MT!
#define ELOG(x)   // RLOG(GetSysTime() << ": " << x)

Vector<Ctrl::Win> Ctrl::wins;
int       Ctrl::WndCaretTime;
bool      Ctrl::WndCaretVisible;

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
	return false;
}

void Ctrl::IMCommit(GtkIMContext *context, gchar *str, gpointer user_data)
{
	WString s = FromUtf8(str);
	for(int i = 0; i < s.GetCount(); i++)
		((Ctrl *)(user_data))->DispatchKey(s[i], 1);
}

void Ctrl::Create(Ctrl *owner, bool popup)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("Create " << Name() << " " << GetRect());
	ASSERT(!IsChild() && !IsOpen());
	LLOG("Ungrab1");

	top = new Top;
	top->window = gtk_window_new(popup ? GTK_WINDOW_POPUP : GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint(gtk(), popup ? GDK_WINDOW_TYPE_HINT_COMBO
	                                      : owner ? GDK_WINDOW_TYPE_HINT_DIALOG
	                                              : GDK_WINDOW_TYPE_HINT_NORMAL);

	top->cursor_id = -1;

	gtk_widget_set_events(top->window, 0xffffffff);
	g_signal_connect(top->window, "event", G_CALLBACK(GtkEvent), this);
	
	gtk_widget_realize(top->window);

	WndSetPos0(GetRect());
/*
	TopWindow *tw = dynamic_cast<TopWindow *>(this);
	gtk_window_set_resizable(gtk(), tw && tw->IsSizeable());
*/
	if(owner && owner->top)
		gtk_window_set_transient_for(gtk(), owner->gtk());
	gtk_widget_set_app_paintable(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
 	gtk_im_context_set_use_preedit(top->im_context, false);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), this);

	WndShow(IsShown());

	Win& w = wins.Add();
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = top->window->window;
	FocusSync();
	if(!popup)
		SetWndFocus();
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	DLOG("POPUP " << GetRect());
	Create(owner, true);
	if(activate)
		SetWndFocus();
}

void Ctrl::WndDestroy0()
{
	gtk_widget_destroy(top->window);
	g_object_unref(top->im_context);
	isopen = false;
	delete top;
	top = NULL;
	int q = FindCtrl(this);
	if(q >= 0)
		wins.Remove(q);
}

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> h;
	for(int i = 0; i < wins.GetCount(); i++)
		h.Add(wins[i].ctrl);
	return h;
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
	int64 id = image.GetSerialId();
	Ctrl *topctrl = NULL;
	Top *top = NULL;
	if(mouseCtrl)
		topctrl = mouseCtrl->GetTopCtrl();
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
		if(c) {
			gdk_window_set_cursor(topctrl->gdk(), c);
			gdk_cursor_unref(c);
		}
	}
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	return top ? top->owner : NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return focusCtrl ? focusCtrl->GetTopCtrl() : NULL;
}

// Vector<Callback> Ctrl::hotkey;

int Ctrl::RegisterSystemHotKey(dword key, Callback cb)
{
/*	ASSERT(key >= K_DELTA);
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = cb;
	dword mod = 0;
	if(key & K_ALT)
		mod |= MOD_ALT;
	if(key & K_SHIFT)
		mod |= MOD_SHIFT;
	if(key & K_CTRL)
		mod |= MOD_CONTROL;
	
	return RegisterHotKey(NULL, q, mod, key & 0xffff) ? q : -1;*/
	return -1;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
/*	if(id >= 0 && id < hotkey.GetCount()) {
		UnregisterHotKey(NULL, id);
		hotkey[id].Clear();
	}*/
}

bool Ctrl::IsWaitingEvent()
{
	return gtk_events_pending();
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	bool r = false;
	if(IsWaitingEvent()) {
		gtk_main_iteration();
		if(quit)
			*quit = IsEndSession();
		r = true;
	}
	TimerProc(GetTickCount());
	SyncCaret();
	AnimateCaret();
	return r;
}

void SweepMkImageCache();

bool Ctrl::ProcessEvents(bool *quit)
{
	bool r = false;
	while(IsWaitingEvent() && (!LoopCtrl || LoopCtrl->InLoop()))
		r = ProcessEvent(quit) || r;
	return r;
}

void Ctrl::SysEndLoop()
{
	gtk_main_quit();
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << LOG_BEGIN);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	while(!IsEndSession() && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
		if(IsEndSession()) break;
		DLOG(rmsecs() << " before gtk_main");
		gtk_main();
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
}

void Ctrl::FocusSync()
{
	GuiLock __;
	Ctrl *focus = NULL;
	static Ctrl *ctrl;
	for(int i = 0; i < wins.GetCount(); i++)
		if(gtk_window_is_active((GtkWindow *)wins[i].gtk))
			focus = wins[i].ctrl;
	if(focus != ctrl) {
		if(ctrl)
			ctrl->KillFocusWnd();
		ctrl = focus;
		if(ctrl)
			ctrl->SetFocusWnd();
	}
}

void Ctrl::PaintCaret(SystemDraw& w)
{
	GuiLock __;
	// LLOG("PaintCaret " << this << ", caretCtrl: " << caretCtrl << ", WndCaretVisible: " << WndCaretVisible);
	if(this == caretCtrl && WndCaretVisible)
		w.DrawRect(caretx, carety, caretcx, caretcy, InvertColor);
}

void Ctrl::SetCaret(int x, int y, int cx, int cy)
{
	GuiLock __;
	if(this == caretCtrl)
		RefreshCaret();
	caretx = x;
	carety = y;
	caretcx = cx;
	caretcy = cy;
	WndCaretTime = GetTickCount();
	if(this == caretCtrl)
		RefreshCaret();
}

void Ctrl::SyncCaret() {
	GuiLock __;
	if(focusCtrl != caretCtrl) {
		RefreshCaret();
		caretCtrl = focusCtrl;
		RefreshCaret();
	}
}

void  Ctrl::AnimateCaret()
{
	GuiLock __;
	int v = !(((GetTickCount() - WndCaretTime) / 500) & 1);
	if(v != WndCaretVisible) {
		RefreshCaret();
		WndCaretVisible = v;
	}
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	Rect r;
	return r;
}

void Ctrl::WndShow0(bool b)
{
	GuiLock __;
	if(b)
		gtk_widget_show_all(top->window);
	else
		gtk_widget_hide_all(top->window);
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
	return GetWorkArea(GetRect().TopLeft());
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	GdkScreen *s = gdk_screen_get_default();
	int n = gdk_screen_get_n_monitors(s);
	rc.Clear();
	for(int i = 0; i < n; i++) {
		GdkRectangle r;
		gdk_screen_get_monitor_geometry(s, i, &r);  // TODO: Use WorkArea (for GTK 3.4)
		rc.Add(RectC(r.x, r.y, r.width, r.height));
	}
}

Rect Ctrl::GetWorkArea(Point pt)
{
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

Rect Ctrl::GetVirtualWorkArea()
{
	Rect out = GetPrimaryWorkArea();
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		out |= rc[i];
	return out;
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	gint x, y, width, height;
	gdk_window_get_geometry(gdk_screen_get_root_window(gdk_screen_get_default()),
	                        &x, &y, &width, &height, NULL);
	return RectC(x, y, width, height);
}

Rect Ctrl::GetPrimaryWorkArea()
{
	Array<Rect> rc;
	GetWorkArea(rc);
	return rc.GetCount() ? rc[0] : RectC(0, 0, 0, 0);
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

void Ctrl::SetWndForeground0()
{
	GuiLock __;
	if(top)
		gtk_window_present(gtk());
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	return top && gtk_window_is_active(gtk());
}

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
	if(top)
		gtk_widget_set_sensitive(top->window, *b);
}

void Ctrl::SetWndFocus0(bool *b)
{
	GuiLock __;
	if(top) {
		gtk_widget_grab_focus(top->window);
		*b = true;
	}
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	return top && gtk_widget_is_focus(top->window);
}

Ptr<Ctrl> Ctrl::grabwindow;

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	if(grabwindow)
		ReleaseWndCapture();
	if(gdk_pointer_grab(gdk(), FALSE,
					    GdkEventMask(GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_PRESS_MASK|GDK_POINTER_MOTION_MASK),
					    NULL, NULL, gtk_get_current_event_time()) == GDK_GRAB_SUCCESS) {
		grabwindow = this;
		return true;
	}
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	gdk_pointer_ungrab(gtk_get_current_event_time());
	grabwindow = NULL;
	return true;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return this == grabwindow && gdk_pointer_is_grabbed();
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
	LLOG("WndInvalidateRect " << rect);
	gdk_window_invalidate_rect(gdk(), GdkRect(r), true);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
	LLOG("WndSetPos0 " << rect);
	gdk_window_move_resize(gdk(), rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
}

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
	LLOG("WndUpdate0r " << r);
	gdk_window_process_updates(gdk(), TRUE);
}

void Ctrl::WndUpdate0()
{
	GuiLock __;
	LLOG("WndUpdate0");
	gdk_window_process_updates(gdk(), TRUE);
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
	gdk_window_invalidate_rect(gdk(), GdkRect(r), true);
}

Rect Ctrl::GetDefaultWindowRect() // ToDo!
{
	return Rect(0, 0, 100, 100);
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
