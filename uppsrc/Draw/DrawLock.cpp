#include "Draw.h"

NAMESPACE_UPP

#ifdef _MULTITHREADED

static StaticMutex sGLock;

static thread__ int sGLockLevel = 0;

void EnterGMutex()
{
	if(sGLockLevel++ == 0)
		sGLock.Enter();
}

void EnterGMutex(int n)
{
	if(n > 0) {
		if(sGLockLevel == 0)
			sGLock.Enter();
		sGLockLevel += n;
	}
}

void LeaveGMutex()
{
	ASSERT(sGLockLevel > 0);
	if(--sGLockLevel == 0)
		sGLock.Leave();
}

int LeaveGMutexAll()
{
	int q = sGLockLevel;
	if(q) {
		sGLock.Leave();
		sGLockLevel = 0;
	}
	return q;
}

#endif

END_UPP_NAMESPACE
