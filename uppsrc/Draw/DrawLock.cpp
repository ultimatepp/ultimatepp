#include "Draw.h"

#define LLOG(x)

NAMESPACE_UPP

#ifdef _MULTITHREADED

static StaticMutex sGLock;

static thread__ int sGLockLevel = 0;

void EnterGMutex()
{
	if(sGLockLevel++ == 0)
		sGLock.Enter();
	LLOG("EnterGMutex");
}

void EnterGMutex(int n)
{
	if(n > 0) {
		if(sGLockLevel == 0)
			sGLock.Enter();
		sGLockLevel += n;
	}
	LLOG("EnterGMutex " << n);
}

void LeaveGMutex()
{
	ASSERT(sGLockLevel > 0);
	if(--sGLockLevel == 0)
		sGLock.Leave();
	LLOG("LeaveGMutex");
}

int LeaveGMutexAll()
{
	int q = sGLockLevel;
	if(q) {
		sGLock.Leave();
		sGLockLevel = 0;
	}
	LLOG("LeaveGMutex all");
	return q;
}

#endif

END_UPP_NAMESPACE
