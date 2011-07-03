#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x)

Ptr<Ctrl>      Ctrl::desktop;
Vector<Ctrl *> Ctrl::topctrl;
Rect           Ctrl::screenRect;

Point          Ctrl::fbCursorPos = Null;
Image          Ctrl::fbCursorImage;
Point          Ctrl::fbCursorBakPos = Null;
Image          Ctrl::fbCursorBak;
Rect           Ctrl::fbCaretRect;
Image          Ctrl::fbCaretBak;
int            Ctrl::fbCaretTm;

void Ctrl::SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetRect(screenRect);
	desktop->SetOpen(true);
	desktop->SetTop();
}

void Ctrl::SetWindowSize(Size sz)
{
	screenRect = sz;
	if(desktop)
		desktop->SetRect(screenRect);
}

void Ctrl::InitGl()
{
	Ctrl::InitTimer();
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
	return GlIsWaitingEvent();
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	if(DoCall())
		return false;
	if(GlEndSession()) {
		if(quit) *quit = true;
		return false;
	}
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	if(GlProcessEvent(quit)) {
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

void Ctrl::DrawScreen()
{
	if(desktop && !painting) {
		painting = true;
		RemoveCursor();		
		RemoveCaret();
		
		ActivateGlContext();
		//SyncLayout(1);
		//InitInfoPanel();
		Rect rect;
		Size csz = desktop->GetSize();
		Rect clip(csz);
		SystemDraw draw(hDC, csz);
		//draw.alpha = alpha;
		//draw.angle = angle;
		draw.FlatView();
		draw.Clear();
		//ApplyTransform(TS_BEFORE_PAINT);
		desktop->CtrlPaint(draw, clip);//, &infoPanel);
		//AnimateCaret();
		//ApplyTransform(TS_AFTER_PAINT);
		SwapBuffers(hDC);
		painting = false;
		LOGF("Fps %.2f\n", GetFps());
	}
}

bool Ctrl::ProcessEvents(bool *quit)
{
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()) && !GlEndSession()); // LoopCtrl-MF 071008
	TimerProc(GetTickCount());
	SweepMkImageCache();
	DrawScreen();
	CursorSync();
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
	while(!GlEndSession() && !quit && ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount())
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
		if(GlEndSession()) break;
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
	if(GlEndSession())
		return;
	LLOG("GuiSleep 2");
	int level = LeaveGMutexAll();
	GlSleep(ms);
	EnterGMutex(level);
}

Rect Ctrl::GetWndScreenRect() const
{
	GuiLock __;
	return screenRect;
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
	return screenRect;
}

void Ctrl::GetWorkArea(Array<Rect>& rc)
{
	GuiLock __;
}

Rect Ctrl::GetVirtualWorkArea()
{
	return screenRect;
}

Rect Ctrl::GetWorkArea(Point pt)
{
	return screenRect;
}

Rect Ctrl::GetVirtualScreenArea()
{
	return screenRect;
}

Rect Ctrl::GetPrimaryWorkArea()
{
	return screenRect;
}

Rect Ctrl::GetPrimaryScreenArea()
{
	return screenRect;
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
	int q = FindTopCtrl();
	if(q >= 0) {
		topctrl.Remove(q);
	}
	if(top) {
		delete top;
		top = NULL;
	}
	isopen = false;
}

void Ctrl::SetWndForeground0()
{
	GuiLock __;
	int q = FindTopCtrl();
	if(q >= 0) {
		topctrl.Remove(q);
		topctrl.Add(this);
	}
}

bool Ctrl::IsWndForeground() const
{
	GuiLock __;
	return false;
}

void Ctrl::WndEnable0(bool *b)
{
	GuiLock __;
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
	::InvalidateRect(glHwnd, NULL, false);
}

void Ctrl::WndSetPos0(const Rect& rect)
{
	GuiLock __;
	SetWndRect(rect);
}

void Ctrl::WndUpdate0r(const Rect& r)
{
	GuiLock __;
}

void  Ctrl::WndScrollView0(const Rect& r, int dx, int dy)
{
	GuiLock __;
	Refresh(r);
}

void Ctrl::PopUp(Ctrl *owner, bool savebits, bool activate, bool dropshadow, bool topmost)
{
	_DBG_ // Add owner management!!!!
	ASSERT(!IsChild() && !IsOpen() && FindTopCtrl() < 0);
	topctrl.Add(this);
	popup = isopen = true;
	SetTop();
	if(activate) SetFocus();
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
