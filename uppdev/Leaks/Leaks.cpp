#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
//	MemoryAlloc(123);
	for(int i = 0; i < 500; i++) {
		size_t n = i;
//		MemoryAllocSz(n);
		DUMP(i);
		DUMP(n);
		ASSERT(i <= n);
	}
	new int;
}

