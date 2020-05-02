#include <Core/Core.h>

using namespace Upp;

void *ptr[1000];

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	SeedRandom(0);
	PeakMemoryProfile();
#ifdef CPU_ARM
	for(int i = 0; i < 5000000; i++)
#else
	for(int i = 0; i < 100000000; i++)
#endif
	{
		if(i % 10000 == 0)
			DLOG(i);
		int ii = Random(1000);
		if(ptr[ii])
			MemoryFree(ptr[ii]);
		ptr[ii] = MemoryAlloc(Random(400000));
	}
	for(void *p : ptr)
		MemoryFree(p);
	
	DLOG(*PeakMemoryProfile());
	DLOG(MemoryProfile());
	DLOG("================ OK");
}
