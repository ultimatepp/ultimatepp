#include "CtrlCore.h"

#define LLOG(x)   //  DLOG(x)

namespace Upp {

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

bool TryEnterGuiMutex()
{
	if(sGLockLevel == 0 && sGLock.TryEnter()) {
		sGLockLevel++;
		return true;
	}
	return false;
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
	Event<>   cb;
};

void Ctrl::PerformCall(Ctrl::CallBox *cbox)
{
	cbox->cb();
	cbox->sem.Release();
}

void Ctrl::Call(Function<void ()> cb)
{
	if(IsMainThread())
		cb();
	else {
		CallBox cbox;
		cbox.cb << cb;
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

void Ctrl::Call(Function<void ()> cb)
{
	cb();
}

#endif

}
