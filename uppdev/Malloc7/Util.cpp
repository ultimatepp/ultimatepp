#include "Malloc.h"

#define LTIMING(x)

void *sPeak;
bool sWasPeak;
int sKB;

void *SysAllocRaw(size_t size)
{
	LTIMING("SysAllocRaw");
	if(sPeak) sWasPeak = true;
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
	return p;
}
