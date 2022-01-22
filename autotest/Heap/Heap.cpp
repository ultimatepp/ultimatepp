#include <Core/Core.h>

using namespace Upp;

void HSet(byte *ptr, int sz)
{
	if(!ptr) return;
	int q = (uintptr_t)ptr % 257;
	while(sz--)
		*ptr++ = (byte)q++;
}

void HCheck(byte *ptr, int sz)
{
	if(!ptr) return;
	int q = (uintptr_t)ptr % 257;
	while(sz--)
		ASSERT(*ptr++ == (byte)q++);
}

int     count;
byte   *ptr[8192];
size_t  sz[8192];

void MCheck()
{
	MemoryCheckDebug();
	for(int i = 0; i < count; i++)
		HCheck(ptr[i], sz[i]);
}

void HClear() {
	for(int i = 0; i < count; i++) {
		HCheck(ptr[i], sz[i]);
		delete ptr[i];
		ptr[i] = NULL;
		sz[i] = 0;
		MCheck();
	}
	MemoryCheck();
	MemoryCheckDebug();
}

void MClear(bool del = false) {
	for(int i = 0; i < count; i++) {
		HCheck(ptr[i], sz[i]);
		MemoryFree(ptr[i]);
		ptr[i] = NULL;
		sz[i] = 0;
		MCheck();
	}
	MemoryCheck();
	MemoryCheckDebug();
}

int RndSize()
{
	return rand() & 255;
	switch(rand() & 3) {
	case 0: return rand() & 255;
	case 1: return rand() & 16383;
	}
	return (rand() + rand() + rand()) & 0x1ffff;
}

void HeapTest(int cnt, int N, bool chk)
{
#ifdef CPU_ARM
	N = N / 20;
#endif
	int mask = cnt - 1;
	count = cnt;
	memset(ptr, 0, sizeof(ptr));
	memset(sz, 0, sizeof(sz));
	for(int n = 1; n < N; n++) {
		if(n % 1000 == 0)
			LOG(n);
		int q = n & mask;
		if(chk)
			MCheck();
		else
			HCheck(ptr[q], sz[q]);
		delete (byte *)ptr[q];
		ptr[q] = NULL;
		if(chk) MCheck();
		sz[q] = RndSize();
		ptr[q] = new byte[sz[q]];
		HSet(ptr[q], sz[q]);
		if(chk) MCheck();
	}
	HClear();
	MCheck();
	LOG("Stage 1 (new/delete) passed\r\n");
	for(int n2 = 1; n2 < N; n2++) {
		if(n2 % 1000 == 0)
			LOG(n2);
		int q = n2 & mask;
		if(chk)
			MCheck();
		else
			HCheck(ptr[q], sz[q]);
//		LOGF("Free: %p\n", (void *)ptr[q]);
		MemoryFree((byte *)ptr[q]);
		ptr[q] = NULL;
		if(chk) MCheck();
		sz[q] = RndSize();
		ptr[q] = (byte *)MemoryAlloc(sz[q]);
//		LOGF("Alloc: %d %p\n", sz[q], (void *)ptr[q]);
		ASSERT(((int)(uintptr_t)ptr[q] & 15) == 0);
		HSet(ptr[q], sz[q]);
		if(chk) MCheck();
	}
	MClear();
	LOG("Stage 2 (MemoryAlloc/MemoryFree) passed");
	for(int n3 = 1; n3 < N; n3++) {
		if(n3 % 1000 == 0)
			LOG(n3);
		int q = n3 & mask;
		if(chk)
			MCheck();
		else
			HCheck(ptr[q], sz[q]);
		MemoryFree((byte *)ptr[q]);
		ptr[q] = NULL;
		if(chk) MCheck();
		sz[q] = RndSize();
		ptr[q] = (byte *)MemoryAllocSz(sz[q]);
		ASSERT(((int)(uintptr_t)ptr[q] & 15) == 0);
		HSet(ptr[q], sz[q]);
		if(chk) MCheck();
	}
	MClear();
	LOG("Stage 3 (MemoryAllocSz/MemoryFree) passed");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	TimeStop tm;

	for(int pass = 0; pass < 2; pass++) {
		LOG("Allocator test:");
		HeapTest(256, 100000, true);
		LOG("Checked allocator test Passed");
		HeapTest(8192, 1000000, false);
		LOG("Quick allocator test Passed");
		HeapTest(8192, 50 * 1000000, false);
		LOG("Allocator test Passed");
	}
	LOG("Used: " << MemoryUsedKb() << " KB");
	LOG("============== OK " << tm);
}
