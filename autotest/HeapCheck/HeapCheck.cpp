#include <Core/Core.h>

using namespace Upp;

#ifdef CPU_ARM
#define N 5000
#else
#define N 100000
#endif

#define LLOG(x) // LOG(x)

void Check(int A, int B = 0)
{
	static void *ptr[256];
	
	for(int n = 0; n < N; n++) {
		if(n % 10000 == 0)
			LOG("Random(" << A << ") + " << B << ": " << n);
		int i = Random(256);
		if(ptr[i]) {
			LLOG("Free " << ptr[i]);
			MemoryFree(ptr[i]);
			MemoryCheck();
		}
		
		int sz = Random(A) + B;

		ptr[i] = MemoryAlloc(sz);
		
		LLOG("Alloc " << sz << ": " << ptr[i] << ", blk: " << GetMemoryBlockSize(ptr[i]));
		
		MemoryCheck();
	}
	
	for(int i = 0; i < 256; i++)
		if(ptr[i]) {
			MemoryFree(ptr[i]);
			ptr[i] = NULL;
		}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	double m = msecs();

	SeedRandom(0);
	
	MemoryCheck();

	Check(100000);
	
	Check(60000, 1000);
	Check(60000);
	Check(1000000);
	
	LOG("================= OK " << (msecs() - m) / 1000 << " s");
}
