#include "CtrlCore.h"

#define LLOG(x)    // DLOG(x)

NAMESPACE_UPP

#ifdef _MULTITHREADED

static StaticMutex sGLock;

static thread__ int sGLockLevel = 0;

void EnterGMutex()
{
	if(sGLockLevel++ == 0)
		sGLock.Enter();
	LLOG("EnterGMutex " << sGLockLevel);
}

void EnterGMutex(int n)
{
	if(n > 0) {
		if(sGLockLevel == 0)
			sGLock.Enter();
		sGLockLevel += n;
	}
	LLOG("EnterGMutex " << sGLockLevel);
}

void LeaveGMutex()
{
	ASSERT(sGLockLevel > 0);
	if(--sGLockLevel == 0)
		sGLock.Leave();
	LLOG("LeaveGMutex " << sGLockLevel);
}

int LeaveGMutexAll()
{
	int q = sGLockLevel;
	if(q) {
		sGLock.Leave();
		sGLockLevel = 0;
	}
	LLOG("LeaveGMutex all " << q);
	return q;
}

static StaticMutex NonMainLock;
// We need secondary lock for nonmain threads because we are unlocking with LeaveGMutexAll
// in the main thread waits for events to allow nonmain threads to play with GUI entities

void EnterGuiMutex()
{
	LLOG("Thread " << IsMainThread() << " trying to lock");
	if(!IsMainThread())
		NonMainLock.Enter();
	EnterGMutex();
	LLOG("Thread " << IsMainThread() << " LOCKED");
}

void LeaveGuiMutex()
{
//	LLOG("Thread " << IsMainThread() << " trying to unlock" << ", nonmain: " << NonMain);
	LeaveGMutex();
	if(!IsMainThread())
		NonMainLock.Leave();
//	LLOG("Thread " << IsMainThread() << " UNLOCK" << ", nonmain: " << NonMain);
}

static StaticMutex     CtrlCallMutex;
Callback               CtrlCall;
static StaticSemaphore CtrlCallSem; // Signals that CtrlCall was executed
static StaticMutex     ICallMutex;

void WakeUpGuiThread();

bool Ctrl::DoCall()
{
	Callback cb;
	{
		Mutex::Lock __(CtrlCallMutex);
		cb = CtrlCall;
	}
	if(CtrlCall) {
		GuiLock __;
		LLOG("DoCall");
		{
			Mutex::Lock __(CtrlCallMutex);
			CtrlCall.Clear();
		}
		cb();
		CtrlCallSem.Release();
		LLOG("--- DoCall");
		return true;
	}
	return false;
}

void Ctrl::ICall(Callback cb)
{
	if(IsMainThread())
		cb();
	else {
		LLOG("Ctrl::Call (nonmain)");
		Mutex::Lock __(ICallMutex); // Only single ICall allowed at all times
		{
			Mutex::Lock __(CtrlCallMutex);
			CtrlCall = cb;
		}
		int level = LeaveGMutexAll();
		WakeUpGuiThread();
		LLOG("Waiting for semaphore");
		if(!Thread::IsShutdownThreads())
			CtrlCallSem.Wait();
		EnterGMutex(level);
		LLOG("-- Ctrl::Call");
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
	ICall(callback1(&Ctrl::GuiSleep0, ms));
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
	ICall(callback1(&Ctrl::EventLoop0, ctrl));
}

END_UPP_NAMESPACE
