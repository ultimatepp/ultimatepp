#include "Malloc.h"

#define LTIMING(x)

static MemoryProfile *sPeak;

void *MemoryAllocPermanentRaw(size_t size)
{
	if(size >= 256)
		return malloc(size);
	static byte *ptr = NULL;
	static byte *limit = NULL;
	if(ptr + size >= limit) {
		ptr = (byte *)AllocRaw4KB();
		limit = ptr + 4096;
	}
	void *p = ptr;
	ptr += size;
	return p;
}


void *MemoryAllocPermanent(size_t size)
{
#ifdef _MULTITHREADED
	Mutex::Lock __(sHeapLock);
#endif
	return MemoryAllocPermanentRaw(size);
}

MemoryProfile *MemGetPeak()
{
	if(sPeak)
		return sPeak;
	sPeak = (MemoryProfile *)MemoryAllocPermanent(sizeof(MemoryProfile));
	memset(sPeak, 0, sizeof(MemoryProfile));
	return NULL;
}

void DoPeakProfile()
{
	if(sPeak)
		heap.Make(*sPeak);
}

int sKB;

void *SysAllocRaw(size_t size)
{
	LTIMING("SysAllocRaw");
	sKB += int(((size + 4095) & ~4095) >> 10);
#ifdef PLATFORM_WIN32
	void *ptr = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#else
#ifdef PLATFORM_LINUX
	void *ptr =  mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#else
	void *ptr =  mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
#endif
#endif
	if(!ptr)
		Panic("Out of memory!");
	DoPeakProfile();
	return ptr;
}

void  SysFreeRaw(void *ptr, size_t size)
{
	LTIMING("SysFreeRaw");
//	LOGF("SysFreeRaw %X - %d\n", ptr, size);
	sKB -= int(((size + 4095) & ~4095) >> 10);
#ifdef PLATFORM_WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, size);
#endif
}

int s4kb__;
int s64kb__;

void *AllocRaw4KB()
{
	static int   left;
	static byte *ptr;
	static int   n = 32;
	if(left == 0) {
		left = n >> 5;
		ptr = (byte *)SysAllocRaw(left * 4096);
	}
	n = n + 1;
	if(n > 4096) n = 4096;
	void *p = ptr;
	ptr += 4096;
	left--;
	s4kb__++;
	DoPeakProfile();
	return p;
}

void *AllocRaw64KB()
{
	static int   left;
	static byte *ptr;
	static int   n = 32;
	if(left == 0) {
		left = n >> 5;
		ptr = (byte *)SysAllocRaw(left * 65536);
	}
	n = n + 1;
	if(n > 256) n = 256;
	void *p = ptr;
	ptr += 65536;
	left--;
	s64kb__++;
	DoPeakProfile();
	return p;
}

void HeapPanic(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	HexDump(VppLog(), pos, size, 64);
	Panic(text);
}

#ifdef HEAPDBG

void Heap::DbgFreeFill(void *p, int size)
{
	int count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		*ptr++ = 0x65657246;
}

void Heap::DbgFreeCheck(void *p, int size)
{
	int count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		if(*ptr++ != 0x65657246)
			HeapPanic("Writes to freed blocks detected", p, size);
}

void *Heap::DbgFreeCheckK(void *p, int k)
{
	Page *page = (Page *)((uintptr_t)p & ~(uintptr_t)4095);
	ASSERT((byte *)page + sizeof(Page) <= (byte *)p && (byte *)p < (byte *)page + 4096);
	ASSERT((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ASSERT(page->klass == k);
	DbgFreeCheck((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
	return p;
}

void Heap::DbgFreeFillK(void *p, int k)
{
	DbgFreeFill((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
}

#endif

#ifdef flagHEAPSTAT
int stat[65536];
int bigstat;

void Heap::Stat(size_t sz)
{
	if(sz < 65536)
		stat[sz]++;
	else
		bigstat++;
}

EXITBLOCK {
	int sum = 0;
	for(int i = 0; i < 65536; i++)
		sum += stat[i];
	sum += bigstat;
	int total = 0;
	VppLog() << Sprintf("Allocation statistics: (total allocations: %d)\n", sum);
	for(int i = 0; i < 65536; i++)
		if(stat[i]) {
			total += stat[i];
			VppLog() << Sprintf("%5d %8dx %2d%%, total %8dx %2d%%\n",
			                    i, stat[i], 100 * stat[i] / sum, total, 100 * total / sum);
		}
	if(bigstat) {
		total += bigstat;
		VppLog() << Sprintf(">64KB %8dx %2d%%, total %8dx %2d%%\n",
		 	                bigstat, 100 * bigstat / sum, total, 100 * total / sum);
	}
}
#endif
