#include "Malloc.h"

#define LLOG(x) // LOG(x)

Heap heaps[4];

void   *bptr[16384];
size_t  bsz[16384];
size_t  total_size;

void HeapSet(byte *ptr, int sz)
{
	if(!ptr) return;
	// Allocator specific!
	size_t l = (uintptr_t)ptr & ~4095;
	size_t qp = (uintptr_t)ptr;
	int q = 0; // (uintptr_t)ptr % 257;
	while(sz--)
		*ptr++ = (byte)q++;
}

void HeapCheck(byte *ptr, int sz)
{
	DUMP((void *)ptr);
	if(!ptr) return;
	int q = 0; // (uintptr_t)ptr % 257;
	while(sz--)
		ASSERT(*ptr++ == (byte)q++);
}

int Hrnd() {
//	return 0;
	return rand() & 3;
}

int Szrnd()
{
//	return 600 + (rand() & 8191);
//	return 600 + rand();
//	return rand() & 511;
	return total_size > 1000 * 1000 * 500 ? rand() & 255
		   : rand() & 3 ? rand() & 511
		   : rand() & 3 ? rand() & 8191
		   : rand() + rand() + rand();
}

void CheckHeaps()
{
	RTIMING("CheckHeaps");
	for(int i = 0; i < 16384; i++)
		HeapCheck((byte *)bptr[i], bsz[i]);
	for(int i = 0; i < 4; i++)
		heaps[i].Check();
}

extern int s4kb__;
extern int s64kb__;
extern int sBig__;

void Test1()
{
	RTIMING("Test1");
	bool prudent = false;
	SetVppLogSizeLimit(200 * 1000 * 1000);
	int doalloc = 20000;
	for(int i = 0; i < 4; i++)
		heaps[i].Init();
//	for(;;)

	for(int n = 0; n < 1000*1000*1000; n++) {
		LLOG("Step " << n);
		if(n % 10000 == 0) {
			Cout() << n << (doalloc > 16384 ? " allocating" : " freeing")
			       << "(" << s4kb__ << ':' << s64kb__ << ':' << sBig__ << ")   ["
			       << total_size << "]\n";
			CheckHeaps();
		}
//		if(n == 114677)
//			LOG("BP");
		int i = n & 16383;
		LLOG("FREE " << (void *)bptr[i]);
		heaps[Hrnd()].Free(bptr[i]);
		bptr[i] = NULL;
		if(prudent)
			CheckHeaps();
		total_size -= bsz[i];
		if(doalloc > 16384) {
			bsz[i] = Szrnd();
			bptr[i] = heaps[Hrnd()].Alloc(bsz[i]);
			LLOG("ALLOCATED " << (void *)bptr[i] << " of size " << bsz[i]);
			HeapSet((byte *)bptr[i], bsz[i]);
		}
		else
			doalloc++;
		if(prudent)
			CheckHeaps();
		total_size += bsz[i];
		if(rand() == 0)
			doalloc = 0;
	//	if(rand() == 1)
	//		doalloc = 20000;
		if(rand() == 0 && (rand() & 7) == 0) {
			Heap& h = heaps[Hrnd()];
			h.Shutdown();
			memset(&h, 0, sizeof(Heap));
			h.Free(h.Alloc(0));
		}
		if(prudent)
			CheckHeaps();
	}
	LLOG("=== Final Free ==============================================================");
	for(int i = 0; i < 16384; i++) {
		if(bptr[i] == (void *)0x05bdd858)
			LOG("BP");
		heaps[0].Free(bptr[i]);
	}
	LLOG("=== Shutdown ==============================================================");
	for(int i = 0; i < 4; i++)
		heaps[i].Shutdown();

	Heap::AuxFinalCheck();

	Cout() << "Passed OK!\n";
}
