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

gboolean Ctrl::GtkProc(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	bool b = ((Ctrl *)(user_data))->Proc(event);
	FocusSync();
	return b;
}

void Ctrl::IMCommit(GtkIMContext *context, gchar *str, gpointer user_data)
{
	WString s = FromUtf8(str);
	for(int i = 0; i < s.GetCount(); i++)
		((Ctrl *)(user_data))->Key(s[i], 1);
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
	gtk_widget_set_events(top->window, 0xffffffff);
	g_signal_connect(top->window, "event", G_CALLBACK(GtkProc), this);
	if(owner && owner->top)
		gtk_window_set_transient_for((GtkWindow *)top->window, (GtkWindow *)owner->top->window);
	gtk_widget_set_app_paintable(top->window, TRUE);
	isopen = true;

	top->im_context = gtk_im_multicontext_new();
	gtk_im_context_set_client_window(top->im_context, gdk());
 	gtk_im_context_set_use_preedit(top->im_context, false);
	g_signal_connect(top->im_context, "commit", G_CALLBACK(IMCommit), this);

	WndShow(IsShown());
	WndSetPos0(GetRect());

	Win& w = wins.Add();
	w.ctrl = this;
	w.gtk = top->window;
	w.gdk = top->window->window;
	FocusSync();
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
	return h;
}

void  Ctrl::SetMouseCursor(const Image& image)
{
	GuiLock __;
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
	if(IsWaitingEvent()) {
		gtk_main_iteration();
		if(quit)
			*quit = IsEndSession();
		return true;
	}	
	return false;
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

	DDUMP(IsEndSession());
	DDUMP(ctrl->InLoop());
	while(!IsEndSession() && (ctrl ? true/* ctrl->IsOpen()*/ && ctrl->InLoop() : true /*GetTopCtrls().GetCount()*/)) // Fix this
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		if(IsEndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		DLOG("Before gtk_main");
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

void Ctrl::WndUpdate0()
{
	GuiLock __;
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
	return Rect();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
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

Rect Ctrl::GetWorkArea(Point pt)
{
	Array<Rect> rc;
	GetWorkArea(rc);
	for(int i = 0; i < rc.GetCount(); i++)
		if(rc[i].Contains(pt))
			return rc[i];
	return GetPrimaryWorkArea();
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	return Rect();
}

Rect Ctrl::GetPrimaryWorkArea()
{
	Rect r;
	return r;
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return Size();
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
	gtk_window_present((GtkWindow *)top->window);
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	return gtk_window_is_active((GtkWindow *)top->window);
}

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
}

void Ctrl::SetWndFocus0(bool *b)
{
	GuiLock __;
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	return false;
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return false;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return false;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return false;
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	GuiLock __;
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
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
}

Rect Ctrl::GetDefaultWindowRect() {
	return Rect(0, 0, 100, 100);
}

ViewDraw::ViewDraw(Ctrl *ctrl)
{
	EnterGuiMutex();
}

ViewDraw::~ViewDraw()
{
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
