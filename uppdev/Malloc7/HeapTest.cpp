#include "Malloc.h"

using namespace Upp;

void HeapCheckDebug() { HeapCheck(); }

void HSet(byte *ptr, int sz)
{
	if(!ptr) return;
	// Allocator specific!
	size_t l = (uintptr_t)ptr & ~4095;
	size_t qp = (uintptr_t)ptr;
	ASSERT(!(qp >= l && qp < l + sizeof(MSmall::Page) || qp + sz >= l && qp + sz < l + sizeof(MSmall::Page)));
	
	int q = 0; // (uintptr_t)ptr % 257;
	while(sz--)
		*ptr++ = (byte)q++;
}

void HCheck(byte *ptr, int sz)
{
	if(!ptr) return;
	int q = 0; // (uintptr_t)ptr % 257;
	while(sz--)
		ASSERT(*ptr++ == (byte)q++);
}

int     count;
byte   *ptr[8192];
size_t  sz[8192];

void MCheck()
{
	HeapCheckDebug();
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
	HeapCheckDebug();
}

void MClear(bool del = false) {
	for(int i = 0; i < count; i++) {
		HCheck(ptr[i], sz[i]);
		MemFree(ptr[i]);
		ptr[i] = NULL;
		sz[i] = 0;
		MCheck();
	}
	HeapCheckDebug();
}

int RndSize()
{
	return rand() & 7 ? rand() & 255 : rand() & 1 ? rand() & 8191 : rand() + rand() + rand();
}

void HeapTest(int cnt, int N, bool chk)
{
	int mask = cnt - 1;
	count = cnt;
	memset(ptr, 0, sizeof(ptr));
	memset(sz, 0, sizeof(sz));
	MemFree(MemAlloc(0));
	for(int n = 1; n < N; n++) {
		if(n % 1000 == 0)
			LOG(n);
		if(n == 263)
			LOG("BP");
		int q = n & mask;
		if(chk)
			MCheck();
		else
			HCheck(ptr[q], sz[q]);
		MemFree((byte *)ptr[q]);
		ptr[q] = NULL;
		if(chk) MCheck();
		sz[q] = RndSize();
		//DUMP(sz[q]);
		ptr[q] = (byte *)MemAlloc(sz[q]);
		//DUMP((int)ptr[q] & 4095);
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
		MemFree((byte *)ptr[q]);
		ptr[q] = NULL;
		if(chk) MCheck();
		sz[q] = RndSize();
		ptr[q] = (byte *)MemAlloc(sz[q]);
//		LOGF("Alloc: %d %p\n", sz[q], (void *)ptr[q]);
		HSet(ptr[q], sz[q]);
		if(chk) MCheck();
	}
	MClear();
	LOG("Stage 2 (MemAlloc/MemFree) passed");
}

void HeapTest()
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	for(;;) {
		LOG("Allocator test:");
		HeapTest(256, 100000, true);
		LOG("Checked allocator test Passed");
		HeapTest(8192, 1000000, false);
		LOG("Quick allocator test Passed");
		HeapTest(8192, 100 * 1000000, false);
		LOG("Allocator test Passed");
	}
}
