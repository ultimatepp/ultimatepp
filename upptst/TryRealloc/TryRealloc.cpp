#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(size_t isz : { 10, 4000, 100000 }) {
		RLOG("=============================================");
		RDUMP(isz);
		void *ptr = MemoryAlloc(isz);
		for(size_t sz : { 10, 5000, 10000, 2000, 40000, 70000 }) {
			RDUMP(sz);
			RDUMP(MemoryTryRealloc(ptr, sz));
			RDUMP(sz);
			RLOG("--------");
		}
		MemoryFree(ptr);
	}
}