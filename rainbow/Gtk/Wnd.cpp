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
	Ctrl *q = (Ctrl *)user_data;
	return q->Proc(event);
}

void Ctrl::Create(bool popup)
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
	gtk_widget_set_app_paintable(top->window, TRUE);
	gtk_widget_show_all(top->window);
	isopen = true;
}

void Ctrl::WndDestroy0()
{
	gtk_widget_destroy(top->window);
	isopen = false;
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
	return NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return NULL;
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
		
		DLOG("After gtk_main");
		if(ctrl) {
			DDUMP(ctrl->IsOpen());
			DDUMP(ctrl->InLoop());
		}
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

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	Rect r;
	return r;
}

void Ctrl::WndShow0(bool b)
{
	GuiLock __;
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
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	return true;
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
	DLOG("WndInvalidateRect " << r);
	GdkRect gr(r);
	DDUMP(gr.x);
	DDUMP(gr.y);
	DDUMP(gr.width);
	DDUMP(gr.height);
	gdk_window_invalidate_rect(win(), &gr, true);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
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
