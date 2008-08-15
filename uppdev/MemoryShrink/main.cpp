#include <CtrlLib/CtrlLib.h>

#define N 100000

Vector<byte *> data;

void Alloc() {
	for(int i = 0; i < N; i++)
		data[i] = new byte[200];
}

void Free() {
	for(int i = 0; i < N; i++)
		delete data[i];
}

GUI_APP_MAIN
{
/*	for(int i = 0; i < 1000; i++) {
		void *b = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
		DUMP(b);
		VirtualFree(b, 4096, MEM_DECOMMIT);
		DUMP(VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE));
	}*/
	LOG("============================================================");
	for(int i = 0; i < 5000; i++) {
		data.SetCount(N);
		Alloc();
		PromptOK("Allocated!");
		Free();
		MemoryProbe(Format("Before shrink %d"), MEMORY_PROBE_FULL);
		TimeStop tm;
		MemoryShrink();
		DUMP(tm);
		MemoryProbe(Format("Pass %d"), MEMORY_PROBE_SUMMARY);
		PromptOK("Freed!");
	}
}
