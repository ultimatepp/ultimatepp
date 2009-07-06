#include "Draw.h"

NAMESPACE_UPP

static StaticMutex sGuiLock;

static thread__ int sGuiLockLevel = 0;

void EnterGuiMutex()
{
	if(sGuiLockLevel++ == 0)
		sGuiLock.Enter();
}

void EnterGuiMutex(int n)
{
	if(n > 0) {
		if(sGuiLockLevel == 0)
			sGuiLock.Enter();
		sGuiLockLevel += n;
	}
}

void LeaveGuiMutex()
{
	ASSERT(sGuiLockLevel > 0);
	if(--sGuiLockLevel == 0)
		sGuiLock.Leave();
}

int LeaveGuiMutexAll()
{
	int q = sGuiLockLevel;
	if(q) {
		sGuiLock.Leave();
		sGuiLockLevel = 0;
	}
	return q;
}

END_UPP_NAMESPACE
