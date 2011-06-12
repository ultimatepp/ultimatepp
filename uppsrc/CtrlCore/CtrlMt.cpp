#include "CtrlCore.h"

#define LLOG(x)    // if(!IsMainThread()) DLOG((int)GetCurrentThreadId() << " " << x)

NAMESPACE_UPP

#ifdef _MULTITHREADED

static int         NonMain;
static StaticMutex NonMainLock; 

void EnterGuiMutex()
{
	LLOG("Thread " << IsMainThread() << " trying to lock" << ", nonmain: " << NonMain);
	bool nonmain = !IsMainThread();
	if(nonmain)
		NonMainLock.Enter();
	EnterGMutex();
	if(nonmain)
		NonMain++;
	LLOG("Thread " << IsMainThread() << " LOCK" << ", nonmain: " << NonMain);
}

void LeaveGuiMutex()
{
	LLOG("Thread " << IsMainThread() << " trying to unlock" << ", nonmain: " << NonMain);
	bool nonmain = !IsMainThread();
	if(nonmain)
		NonMain--;
	LeaveGMutex();
	if(nonmain)
		NonMainLock.Leave();
	LLOG("Thread " << IsMainThread() << " UNLOCK" << ", nonmain: " << NonMain);
}

struct Ctrl::CallBox {
	Semaphore sem;
	Callback  cb;
};

void Ctrl::PerformCall(Ctrl::CallBox *cbox)
{
	cbox->cb();
	LLOG("Sem release");
	cbox->sem.Release();
}

Callback    Ctrl::CtrlCall;
static Mutex CtrlCallMutex;

void WakeUpGuiThread();

bool Ctrl::DoCall()
{
	LLOG("DoCall");
	GuiLock __;
	{
		Mutex::Lock __(CtrlCallMutex);
		CtrlCall();
		CtrlCall.Clear();
	}
	LLOG("--- DoCall, nonmain: " << NonMain);
	return NonMain;
}

void Ctrl::ICall(Callback cb)
{
	LLOG("Ctrl::Call " << IsMainThread() << ", nonmain: " << NonMain);
	if(IsMainThread())
		cb();
	else {
		GuiLock __;
		CallBox cbox;
		cbox.cb = cb;
		{
			Mutex::Lock __(CtrlCallMutex);
			CtrlCall = callback1(PerformCall, &cbox);
		}
		int level = LeaveGMutexAll();
		WakeUpGuiThread();
		LLOG("Waiting for semaphore");
		if(!Thread::IsShutdownThreads())
			cbox.sem.Wait();
		EnterGMutex(level);
	}
	LLOG("-- Ctrl::Call " << IsMainThread());
}

void Ctrl::Call(Callback cb)
{
	if(IsMainThread())
		cb();
	else {
		GuiLock __;
		CallBox cbox;
		cbox.cb = cb;
		UPP::PostCallback(callback1(PerformCall, &cbox));
		int n = NonMain;
		int nn = n;
		NonMain = 0;
		for(int i = 0; i < n; i++)
			NonMainLock.Leave();
		int level = LeaveGMutexAll();
		WakeUpGuiThread();
		if(!Thread::IsShutdownThreads())
			cbox.sem.Wait();
		for(int i = 0; i < n; i++)
			NonMainLock.Enter();
		EnterGMutex(level);
		NonMain = n;
	}
}

#else

bool Ctrl::DoCall()
{
	return false;
}

void Ctrl::ICall(Callback cb)
{
	cb();
}

void Ctrl::Call(Callback cb)
{
	cb();
}
#endif

void Ctrl::GuiSleep(int ms)
{
	Call(callback1(&Ctrl::GuiSleep0, ms));
}

void Ctrl::WndDestroy()
{
	ICall(callback(this, &Ctrl::WndDestroy0));
}

void Ctrl::WndShow(bool b)
{
	ICall(THISBACK1(WndShow0, b));
}

void Ctrl::WndUpdate()
{
	ICall(THISBACK(WndUpdate0));
}

void Ctrl::SetWndForeground()
{
	ICall(THISBACK(SetWndForeground0));
}

bool Ctrl::WndEnable(bool b)
{
	ICall(THISBACK1(WndEnable0, &b));
	return b;
}

bool Ctrl::SetWndFocus()
{
	bool b;
	ICall(THISBACK1(SetWndFocus0, &b));
	return b;
}

void Ctrl::WndInvalidateRect(const Rect& r)
{
	ICall(THISBACK1(WndInvalidateRect0, r));
}

void Ctrl::WndSetPos(const Rect& rect)
{
	ICall(THISBACK1(WndSetPos0, rect));
}

void Ctrl::WndUpdate(const Rect& r)
{
	ICall(THISBACK1(WndUpdate0r, r));
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	ICall(THISBACK3(WndScrollView0, r, dx, dy));
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	Call(callback1(&Ctrl::EventLoop0, ctrl));
}

END_UPP_NAMESPACE
