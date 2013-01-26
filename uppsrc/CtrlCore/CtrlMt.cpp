#include "CtrlCore.h"

#define LLOG(x)   //  DLOG(x)

NAMESPACE_UPP

#ifdef _MULTITHREADED

static StaticMutex sGLock;

static thread__ int sGLockLevel = 0;

void EnterGuiMutex()
{
	LLOG(">EnterGuiMutex " << sGLockLevel << ' ' << IsMainThread());
	if(sGLockLevel++ == 0)
		sGLock.Enter();
	LLOG("EnterGuiMutex " << sGLockLevel << ' ' << IsMainThread());
}

void EnterGuiMutex(int n)
{
	LLOG(">EnterGuiMutex: " << n << ' ' << sGLockLevel << ' ' << IsMainThread());
	if(n > 0) {
		if(sGLockLevel == 0)
			sGLock.Enter();
		sGLockLevel += n;
	}
	LLOG("EnterGuiMutex " << sGLockLevel << ' ' << IsMainThread());
}

void LeaveGuiMutex()
{
	LLOG(">LeaveGuiMutex " << sGLockLevel << ' ' << IsMainThread());
	ASSERT(sGLockLevel > 0);
	if(--sGLockLevel == 0)
		sGLock.Leave();
	LLOG("LeaveGuiMutex " << sGLockLevel << ' ' << IsMainThread());
}

int LeaveGuiMutexAll()
{
	LLOG(">LeaveGuiMutexAll " << sGLockLevel << ' ' << IsMainThread());
	int q = sGLockLevel;
	if(q) {
		sGLock.Leave();
		sGLockLevel = 0;
	}
	LLOG("LeaveGuiMutexAll " << q << ' ' << IsMainThread());
	return q;
}

bool ThreadHasGuiLock()
{
	return Thread::IsMain() || sGLockLevel;
}

void WakeUpGuiThread();

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

void Ctrl::Call(Callback cb)
{
	if(IsMainThread())
		cb();
	else {
		CallBox cbox;
		cbox.cb = cb;
		UPP::PostCallback(callback1(PerformCall, &cbox));
		WakeUpGuiThread();
		int level = LeaveGuiMutexAll(); // Unlock GUI to give main thread chance to handle Call
		cbox.sem.Wait();
		EnterGuiMutex(level);
	}
}

void Ctrl::ShutdownThreads()
{
	Thread::BeginShutdownThreads();
	while(Thread::GetCount()) {
		ProcessEvents();
		Sleep(0);
	}
	Thread::EndShutdownThreads();
}

#else

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
	Call(PTEBACK(WndDestroy0));
}

void Ctrl::WndShow(bool b)
{
	Call(PTEBACK1(WndShow0, b));
}

void Ctrl::WndUpdate()
{
	Call(PTEBACK(WndUpdate0));
}

void Ctrl::SetWndForeground()
{
	Call(PTEBACK(SetWndForeground0));
}

bool Ctrl::WndEnable(bool b)
{
	Call(PTEBACK1(WndEnable0, &b));
	return b;
}

bool Ctrl::SetWndFocus()
{
	bool b;
	Call(PTEBACK1(SetWndFocus0, &b));
	return b;
}

void Ctrl::WndSetPos(const Rect& rect)
{
	Call(PTEBACK1(WndSetPos0, rect));
}

void Ctrl::WndUpdate(const Rect& r)
{
	Call(PTEBACK1(WndUpdate0r, r));
}

void  Ctrl::WndScrollView(const Rect& r, int dx, int dy)
{
	Call(PTEBACK3(WndScrollView0, r, dx, dy));
}

void Ctrl::EventLoop(Ctrl *ctrl)
{
	Call(callback1(&Ctrl::EventLoop0, ctrl));
}

END_UPP_NAMESPACE
