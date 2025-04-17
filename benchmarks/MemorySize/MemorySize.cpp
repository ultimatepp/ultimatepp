#include <Core/Core.h>

using namespace Upp;

// TEST IN RELEASE MODE!

CONSOLE_APP_MAIN
{
	for(int q = 5; q < 500*1024*1024; q = q + 1 + q / 10) {
		size_t sz = q;
		void *ptr = MemoryAllocSz(sz);
		DLOG("Requested " << q << ", allocated " << sz << ", GetMemoryBlockSize " << GetMemoryBlockSize(ptr));
		if(sz != GetMemoryBlockSize(ptr))
			Panic("FAILED!");
		MemoryFree(ptr);
	}
}
