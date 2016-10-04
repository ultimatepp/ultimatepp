#include "Malloc.h"

#ifdef _DEBUG
#define N 1000
#else
#define N 1000*1000
#endif

byte randb[500];

CONSOLE_APP_MAIN
{
//	HeapTest(); return;
	IdMapBenchmark(); return;

	for(int i = 0; i < 500; i++)
		randb[i] = rand();
	heap.Init();
//	if(0)
	{
		{
			RTIMING("NewAlloc 1");
			for(int i = 0; i < 10 * N; i++) {
				void *p1 = heap.Alloc(10);
				heap.Free(p1);
			}
		}
		{
			RTIMING("OldAlloc 1");
			for(int i = 0; i < 10 * N; i++) {
				void *p1 = new byte[10];
				delete[] p1;
			}
		}
	}
	{
		RTIMING("NewAlloc 2");
		for(int i = 0; i < 10 * N; i++) {
			void *p1 = heap.Alloc(10);
			void *p2 = heap.Alloc(10);
			heap.Free(p1);
			heap.Free(p2);
		}
	}
	{
		RTIMING("OldAlloc 2");
		for(int i = 0; i < 10 * N; i++) {
			void *p1 = new byte[10];
			void *p2 = new byte[10];
			delete[] p1;
			delete[] p2;
		}
	}
//	return;
	{
		RTIMING("NewAlloc 50");
		for(int i = 0; i < N; i++) {
			void *p[50];
			for(int i = 0; i < 50; i++)
				p[i] = heap.Alloc(10);
			for(int i = 0; i < 50; i++)
				heap.Free(p[i]);
		}
	}
	{
		RTIMING("OldAlloc 50");
		for(int i = 0; i < N; i++) {
			void *p[50];
			for(int i = 0; i < 50; i++)
				p[i] = new byte[10];
			for(int i = 0; i < 50; i++)
				delete[] p[i];
		}
	}
	LOG("==========");
	{
		RTIMING("NewAlloc 500 rnd");
		for(int i = 0; i < N / 10; i++) {
			void *p[500];
			for(int i = 0; i < 500; i++)
				p[i] = heap.Alloc(randb[i]);
			for(int i = 0; i < 500; i++)
				heap.Free(p[i]);
		}
	}
	{
		RTIMING("OldAlloc 500 rnd");
		for(int i = 0; i < N / 10; i++) {
			void *p[500];
			for(int i = 0; i < 500; i++)
				p[i] = new byte[randb[i]];
			for(int i = 0; i < 500; i++)
				delete[] p[i];
		}
	}
	{
		RTIMING("NewAlloc 5000 rnd");
		for(int i = 0; i < N / 100; i++) {
			void *p[5000];
			for(int i = 0; i < 5000; i++)
				p[i] = heap.Alloc(randb[i]);
			for(int i = 0; i < 5000; i++)
				heap.Free(p[i]);
		}
	}
	{
		RTIMING("OldAlloc 5000 rnd");
		for(int i = 0; i < N / 100; i++) {
			void *p[5000];
			for(int i = 0; i < 5000; i++)
				p[i] = new byte[randb[i]];
			for(int i = 0; i < 5000; i++)
				delete[] p[i];
		}
	}
}
