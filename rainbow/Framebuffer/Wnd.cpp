#include <CtrlCore/CtrlCore.h>

#ifdef GUI_FB

NAMESPACE_UPP

#define LLOG(x)

Ptr<Ctrl>      desktop;
ImageBuffer    framebuffer;
Vector<Rect>   invalid;

void SetDesktop(Ctrl& q)
{
	desktop = &q;
	desktop->SetRect(framebuffer.GetSize());
	desktop->SetOpen(true);
	desktop->SetTop();
	invalid.Add(framebuffer.GetSize());
}

Ctrl *GetDesktop()
{
	return desktop;
}

void Ctrl::InitFB()
{
	Ctrl::GlobalBackBuffer();
	Ctrl::InitTimer();
	framebuffer.Create(1000, 600);
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
	return FBIsWaitingEvent();
}

bool Ctrl::ProcessEvent(bool *quit)
{
	ASSERT(IsMainThread());
	if(DoCall())
		return false;
	if(FBEndSession()) {
		if(quit) *quit = true;
		return false;
	}
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	if(FBProcessEvent(quit)) {
		DefferedFocusSync();
		SyncCaret();
		return true;
	}
	return false;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	if(!ProcessEvent(quit))
		return false;
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()) && !FBEndSession()); // LoopCtrl-MF 071008
	TimerProc(GetTickCount());
	SweepMkImageCache();
	if(invalid.GetCount()) {
		RemoveCursor();
		RemoveCaret();
	}
	for(int i = 0; i < invalid.GetCount(); i++) { _DBG_ // Optimize for single UpdateArea!
		SystemDraw painter(framebuffer);
		painter.Draw::Clip(invalid[i]);
		if(desktop)
			desktop->UpdateArea(painter, invalid[i]);
		painter.End();
		FBUpdate(invalid[i]);
	}
	invalid.Clear();
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

Rect Ctrl::GetWndUpdateRect() const
{
	GuiLock __;
	Rect r;
	return Rect(0, 0, 1000, 600);
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
	return this == desktop;
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
	if(top)
		delete top;
}

void Ctrl::SetWndForeground0()
{
	GuiLock __;
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
	AddRefreshRect(invalid, r);
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
