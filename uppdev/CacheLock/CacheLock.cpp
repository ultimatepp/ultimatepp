#include <Core/Core.h>

using namespace Upp;

/*
struct CacheLock {
	Mutex mutex;
	bool  dirty;

public:
	void Dirty()        { dirty = true; }
	void Clear()        { dirty = false; }
	bool EnterUpdate() {
		if(dirty) {
			mutex.Enter();
			if(dirty) return true;
			mutex.Leave();
		}
		return false;
	}
	void LeaveUpdate() {
		dirty = false;
		mutex.Leave();
	}
};

bool b = dirty;
mfence
if(b) {
	initcode;
	sfence
	dirty = false;
}
*/

static bool y;

CONSOLE_APP_MAIN
{
	y = ReadWithBarrier(y);
}
