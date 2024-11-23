#include "Local.h"

#ifdef VIRTUALGUI

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)
#define LDUMP(x) //DDUMP(x)

static Point fbmousepos;

Point GetMousePos() {
	return fbmousepos;
}

bool GetMouseLeft()   { return VirtualGuiPtr->GetMouseButtons() & (1<<0); }
bool GetMouseRight()  { return VirtualGuiPtr->GetMouseButtons() & (1<<1); }
bool GetMouseMiddle() { return VirtualGuiPtr->GetMouseButtons() & (1<<2); }
bool GetShift()       { return VirtualGuiPtr->GetModKeys() & KM_SHIFT; }
bool GetCtrl()        { return VirtualGuiPtr->GetModKeys() & KM_CTRL; }
bool GetAlt()         { return VirtualGuiPtr->GetModKeys() & KM_ALT; }
bool GetCapsLock()    { return VirtualGuiPtr->GetModKeys() & KM_CAPS; }

void Ctrl::MouseEventFB(Ptr<Ctrl> t, int event, Point p, int zdelta)
{
	if(!t->IsEnabled())
		return;
	Rect rr = t->GetRect();
	if((event & Ctrl::ACTION) == DOWN) {
		Ptr<Ctrl> q = t;
		TopWindowFrame *wf = dynamic_cast<TopWindowFrame *>(~t);
		if(wf)
			q = wf->window;
		if(q) q->ClickActivateWnd();
		if(q) q->SetForeground();
		if(ignoreclick)
			return;
	}
	if(t)
		t->DispatchMouse(event, p - rr.TopLeft(), zdelta);
	if(t)
		t->PostInput();
}

Ctrl *Ctrl::FindMouseTopCtrl()
{
	for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
		Ctrl *t = topctrl[i];
		if(t->GetRect().Contains(fbmousepos))
			return t->IsEnabled() ? t : NULL;
	}
	return desktop->IsEnabled() ? desktop : NULL;
}

void Ctrl::DoMouseFB(int event, Point p, int zdelta)
{
	fbmousepos = p;
	int a = event & Ctrl::ACTION;
	if(a == Ctrl::UP && Ctrl::ignoreclick) {
		EndIgnore();
		return;
	}
	else
	if(a == Ctrl::DOWN && ignoreclick)
		return;
	LLOG("### Mouse event: " << event << " position " << p << " zdelta " << zdelta << ", capture " << Upp::Name(captureCtrl));
	if(captureCtrl)
		MouseEventFB(captureCtrl->GetTopCtrl(), event, p, zdelta);
	else
		for(int i = topctrl.GetCount() - 1; i >= 0; i--) {
			Ptr<Ctrl> t = topctrl[i];
			Rect rr = t->GetRect();
			if(rr.Contains(p)) {
				MouseEventFB(t, event, p, zdelta);
				return;
			}
		}
	Ctrl *desktop = GetDesktop();
	if(desktop) {
		desktop->DispatchMouse(event, p, zdelta);
		desktop->PostInput();
	}
}

bool Ctrl::DoKeyFB(dword key, int cnt)
{
	LLOG("DoKeyFB " << GetKeyDesc(key) << ", " << cnt);
	
	bool b = DispatchKey(key, cnt);
	SyncCaret();
	Ctrl *desktop = GetDesktop();
	if(desktop)
		desktop->PostInput();
	return b;
}

void Ctrl::CursorSync()
{
	LLOG("@ CursorSync");
	Point p = GetMousePos() - fbCursorImage.GetHotSpot();
	if(fbCursorPos != p) {
		fbCursorPos = p;
		if(!(VirtualGuiPtr->GetOptions() & GUI_SETMOUSECURSOR))
			Invalidate();
	}

}

bool Ctrl::ProcessEvent(bool *quit)
{
	LLOG("@ ProcessEvent");
	ASSERT(IsMainThread());
	if(!GetMouseLeft() && !GetMouseRight() && !GetMouseMiddle())
		ReleaseCtrlCapture();
	bool ret = VirtualGuiPtr->ProcessEvent(quit);
	DefferedFocusSync();
	SyncCaret();
	SyncTopWindows();
	return ret;
}

bool Ctrl::ProcessEvents(bool *quit)
{
	//LOGBLOCK("@ ProcessEvents");
//	MemoryCheckDebug();
	bool ret = ProcessEvent(quit);
	while(ProcessEvent(quit) && (!LoopCtrl || LoopCtrl->InLoop()));
	TimeStop tm;
	LLOG("TimerProc invoked at " << msecs());
	TimerProc(msecs());
	LLOG("TimerProc elapsed: " << tm);
	AnimateCaret();
	SweepMkImageCache();
	DoPaint();
	return ret;
}

void Ctrl::EventLoop(Ctrl *ctrl)
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

	bool quit = false;
	int64 loopno = ++EventLoopNo;
	ProcessEvents(&quit);
	while(loopno > EndSessionLoopNo && !quit && (ctrl ? ctrl->IsOpen() && ctrl->InLoop() : GetTopCtrls().GetCount()))
	{
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / GuiSleep");
		SyncCaret();
		GuiSleep(20);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / ProcessEvents");
		ProcessEvents(&quit);
//		LLOG(GetSysTime() << " % " << (unsigned)msecs() % 10000 << ": EventLoop / after ProcessEvents");
		LDUMP(loopno);
		LDUMP(fbEndSessionLoop);
	}

	if(ctrl)
		LoopCtrl = ploop;
	LoopLevel--;
	LLOG(LOG_END << "Leaving event loop ");
}

void Ctrl::GuiSleep(int ms)
{
	GuiLock __;
	ASSERT(IsMainThread());
	LLOG("GuiSleep");
	int level = LeaveGuiMutexAll();
	VirtualGuiPtr->WaitEvent(ms);
	EnterGuiMutex(level);
}

void WakeUpGuiThread()
{
	VirtualGuiPtr->WakeUpGuiThread();
}

END_UPP_NAMESPACE

#endif
