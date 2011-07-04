#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x)

ImageBuffer    Ctrl::framebuffer;
Vector<Rect>   Ctrl::invalid;

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;

Point          Ctrl::fbCursorPos = Null;
Image          Ctrl::fbCursorImage;
Point          Ctrl::fbCursorBakPos = Null;
Image          Ctrl::fbCursorBak;
Rect           Ctrl::fbCaretRect;
Image          Ctrl::fbCaretBak;
int            Ctrl::fbCaretTm;
int            Ctrl::renderingMode = MODE_ANTIALIASED;

void Ctrl::SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetRect(framebuffer.GetSize());
	desktop->SetOpen(true);
	desktop->NewTop();
	invalid.Add(framebuffer.GetSize());
}

void Ctrl::SetRenderingMode(int mode)
{
	renderingMode = mode;
	invalid.Add(framebuffer.GetSize());
}

void Ctrl::InitFB()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();
	framebuffer.Create(1, 1);
}

void Ctrl::SetFramebufferSize(Size sz)
{
	framebuffer.Create(sz);
	if(desktop)
		desktop->SetRect(sz);
	invalid.Add(sz);
}

int Ctrl::FindTopCtrl() const
{
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(this == topctrl[i])
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

Vector<Ctrl *> Ctrl::GetTopCtrls()
{
	Vector<Ctrl *> ctrl;
	if(desktop)
		ctrl.Add(desktop);
	for(int i = 0; i < topctrl.GetCount(); i++)
		ctrl.Add(topctrl[i]);
	return ctrl;
}

Ctrl *Ctrl::GetOwner()
{
	GuiLock __;
	int q = FindTopCtrl();
	if(q > 0 && topctrl[q]->top)
		return topctrl[q]->top->owner_window;
	return NULL;
}

Ctrl *Ctrl::GetActiveCtrl()
{
	GuiLock __;
	return desktop;
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
	return FBIsWaitingEvent();
}

void Ctrl::AddInvalid(const Rect& rect)
{
	AddRefreshRect(invalid, rect);
}

void Ctrl::SyncTopWindows()
{
	for(int i = 0; i < topctrl.GetCount(); i++) {
		TopWindow *w = dynamic_cast<TopWindow *>(topctrl[i]);
		if(w)
			w->SyncRect();
	}
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	if(DoCall()) {
		SyncTopWindows();
		return false;
	}
	if(FBEndSession()) {
		if(quit) *quit = true;
		return false;
	}
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	if(FBProcessEvent(quit)) {
		SyncTopWindows();
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

Rect Ctrl::GetClipBound(const Vector<Rect>& inv, const Rect& r)
{
	Rect ri = Null;
	for(int j = 0; j < inv.GetCount(); j++) {
		Rect rr = inv[j] & r;
		if(!rr.IsEmpty())
			ri = IsNull(ri) ? rr : rr | ri;
	}
	return ri;
}

void Ctrl::DoPaint(const Vector<Rect>& invalid)
{
	if(invalid.GetCount() && desktop) {
		RemoveCursor();
		RemoveCaret();
		SystemDraw painter(framebuffer, invalid, renderingMode);
		painter.Begin();
		for(int i = 0; i < invalid.GetCount(); i++)
			painter.RectPath(invalid[i]);
		painter.Painter::Clip();
		Vector<Rect> inv;
		inv <<= invalid;
		for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
			Rect r = topctrl[i]->GetRect();
			Rect ri = GetClipBound(inv, r);
			if(!IsNull(ri)) {
				painter.Offset(r.TopLeft());
				topctrl[i]->UpdateArea(painter, ri - r.TopLeft());
				painter.End();
				Subtract(inv, r);
				painter.ExcludeClip(r);
			}
		}
		Rect ri = GetClipBound(inv, framebuffer.GetSize());
		if(!IsNull(ri))
			desktop->UpdateArea(painter, ri);
	}
	CursorSync();
	for(int i = 0; i < invalid.GetCount(); i++)
		FBUpdate(invalid[i]);
}

bool Ctrl::ProcessEvents(bool *quit)
{
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()) && !FBEndSession());
	TimerProc(GetTickCount());
	SweepMkImageCache();
	DoPaint(invalid);
	invalid.Clear();
	return false;
}

void Ctrl::EventLoop0(Ctrl *ctrl)
{
	GuiLock __;
	ASSERT(IsMainThread());
	ASSERT(LoopLevel == 0 || ctrl);
	LoopLevel++;
	LLOG("Entering event loop at level " << LoopLevel << BeginIndent);
	Ptr<Ctrl> ploop;
	if(ctrl) {
		ploop = LoopCtrl;
		LoopCtrl = ctrl;
		ctrl->inloop = true;
	}

	bool quit = false;
	ProcessEvents(&quit);
	while(!FBEndSession() && !quit && ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
		if(FBEndSession()) break;
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(EndIndent << "Leaving event loop ");
}

void Ctrl::GuiSleep0(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	if(FBEndSession())
		return;
	LLOG("GuiSleep 2");
	int level = LeaveGMutexAll();
	FBSleep(ms);
	EnterGMutex(level);
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	return GetRect();
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
	return FindTopCtrl() >= 0 || this == desktop;
}

void Ctrl::SetAlpha(byte alpha)
{
	GuiLock __;
}

Rect Ctrl::GetWorkArea() const
{
	GuiLock __;
	return framebuffer.GetSize();
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
	Array<Rect> r;
	r.Add(framebuffer.GetSize());
}

Rect Ctrl::GetVirtualWorkArea()
{
	return framebuffer.GetSize();
}

Rect Ctrl::GetWorkArea(Point pt)
{
	return framebuffer.GetSize();
}

Rect Ctrl::GetVirtualScreenArea()
{
	GuiLock __;
	return framebuffer.GetSize();
}

Rect Ctrl::GetPrimaryWorkArea()
{
	Rect r;
	return framebuffer.GetSize();
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return framebuffer.GetSize();
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

void Ctrl::WndDestroy0()
{
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this)
			topctrl[i]->WndDestroy0();
	int q = FindTopCtrl();
	if(q >= 0) {
		AddInvalid(GetRect());
		topctrl.Remove(q);
	}
	if(top) {
		delete top;
		top = NULL;
	}
	isopen = false;
	TopWindow *win = dynamic_cast<TopWindow *>(this);
	if(win)
		win->DestroyFrame();
}

void Ctrl::PutForeground()
{
	int q = FindTopCtrl();
	if(q >= 0) {
		AddInvalid(GetRect());
		topctrl.Remove(q);
		topctrl.Add(this);
	}
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this)
			topctrl[i]->PutForeground();
}

void Ctrl::SetWndForeground0()
{
	GuiLock __;
	ASSERT(IsOpen());
	if(top && top->owner_window && !IsWndForeground())
		top->owner_window->PutForeground();
	PutForeground();
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	for(int i = 0; i < topctrl.GetCount(); i++)
		if(topctrl[i]->top && topctrl[i]->top->owner_window == this && topctrl[i]->IsWndForeground())
			return true;
	return topctrl.GetCount() ? topctrl.Top() == this : this == desktop;
}

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
	*b = true;
}

void Ctrl::SetWndFocus0(bool *b)
{
	GuiLock __;
	*b = true;
}

bool Ctrl::HasWndFocus() const
{
	GuiLock __;
	return focusCtrl && focusCtrl->GetTopCtrl() == this;
}

bool Ctrl::SetWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return true;
}

bool Ctrl::ReleaseWndCapture()
{
	GuiLock __;
	ASSERT(IsMainThread());
	return true;
}

bool Ctrl::HasWndCapture() const
{
	GuiLock __;
	return captureCtrl && captureCtrl->GetTopCtrl() == this;
}

void Ctrl::WndInvalidateRect0(const Rect& r)
{
	GuiLock __;
	int q = FindTopCtrl();
	if(q >= 0)
		AddInvalid(r + topctrl[q]->GetRect().TopLeft());
	else
		AddInvalid(r);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
	TopWindow *w = dynamic_cast<TopWindow *>(this);
	if(w)
		w->SyncFrameRect(rect);
	invalid.Add(GetRect());
	SetWndRect(rect);
	invalid.Add(rect);
}

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
	Rect rr = r + GetRect().TopLeft();
	bool dummy;
	DoPaint(Intersect(invalid, rr, dummy));
	invalid = Subtract(invalid, rr, dummy);
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
	Refresh(r);
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	ASSERT(!IsChild() && !IsOpen() && FindTopCtrl() < 0);
	NewTop();
	if(owner) {
		Ctrl *owner_window = owner->GetTopCtrl(); _DBG_ // should perhaps be topwindow;
		ASSERT(owner_window->IsOpen());
		if(owner_window != desktop) {
			owner_window->SetForeground();
			top->owner_window = owner_window;
		}
	}
	topctrl.Add(this);
	popup = isopen = true;
	RefreshLayoutDeep();
	if(activate) SetFocusWnd();
	AddInvalid(GetRect());
}

Rect Ctrl::GetDefaultWindowRect() {
	return Rect(0, 0, 100, 100);
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
