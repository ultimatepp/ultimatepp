#include "Core.h"

NAMESPACE_UPP

#ifdef CPU_X86

static bool sHasMMX;
static bool sHasSSE;
static bool sHasSSE2;
static bool sHasSSE3;

static void sCheckCPU()
{
	static bool done;
	if(done) return;
	done = true;
#ifdef PLATFORM_OSX11
//	__asm__("pushl %%ebx\n\tmovl $1, %%eax\n\tcpuid\n\tpopl %%ebx" : "=d" (info1), "=c" (info2) : : "%eax");
	sHasMMX = true;
	sHasSSE = true;
	sHasSSE2 = true;
#else
#ifdef CPU_AMD64
	sHasMMX = true;
	sHasSSE = true;
	sHasSSE2 = true;
#else
#ifdef COMPILER_MSC
	dword info1;
	dword info2;
	__asm {
		mov eax, 1
		cpuid
		mov info1, edx
		mov info2, ecx
	}
#else
	dword info1;
	dword info2;
	__asm__("movl $1, %%eax\n\tcpuid" : "=d" (info1), "=c" (info2) : : "%eax", "%ebx");
#endif
	sHasMMX = ((info1 >> 23) & 0x1);
	sHasSSE = ((info1 >> 25) & 0x1);
	sHasSSE2 = ((info1 >> 26) & 0x1);
	sHasSSE3 = ((info2) & 0x1);
#endif
#endif
}

INITBLOCK {
//	sCheckCPU();
}

bool CpuMMX()  { sCheckCPU(); return sHasMMX; }
bool CpuSSE()  { sCheckCPU(); return sHasSSE; }
bool CpuSSE2() { sCheckCPU(); return sHasSSE2; }
bool CpuSSE3() { sCheckCPU(); return sHasSSE3; }

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_FREEBSD
#include <unistd.h>
#else
#include <sys/sysinfo.h>
#endif
#endif

int CPU_Cores()
{
	static int n;
	ONCELOCK {
#ifdef PLATFORM_WIN32
#ifdef CPU_64
		uint64 pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(int i = 0; i < 64; i++)
			n += !!(sa & ((uint64)1 << i));
#else
		DWORD pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(int i = 0; i < 32; i++)
			n += !!(sa & (1 << i));
#endif
#elif defined(PLATFORM_POSIX)
#if defined(PLATFORM_FREEBSD) || defined(PLATFORM_SOLARIS)
		n = minmax((int)sysconf(_SC_NPROCESSORS_ONLN), 1, 256);
#else
		n = minmax(get_nprocs(), 1, 256);
#endif
#else
		n = 1;
#endif
	}
	return n;
}

#endif

#ifdef PLATFORM_WIN32
bool IsDecentMachine()
{
	if(!IsWin2K())
		return false;
	MEMORYSTATUS m;
	GlobalMemoryStatus(&m);
	return m.dwTotalPhys > 500 * 1024 * 1024;

}
#else
bool IsDecentMachine()
{
	return true;
}
#endif

#ifndef CPU_X86
int64 PeekI64(const void *ptr) {
	const byte *p = (const byte *)ptr;
	dword a = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
	dword b = p[4] | (p[5] << 8) | (p[6] << 16) | (p[7] << 24);
	return (int64)a | ((int64)b << 32);
}
#endif

#ifndef CPU_X86
void PokeI64(void *ptr, int64 value) {
	byte *p = (byte *)ptr;
	p[0] = (byte)(value >> 8 * 0);
	p[1] = (byte)(value >> 8 * 1);
	p[2] = (byte)(value >> 8 * 2);
	p[3] = (byte)(value >> 8 * 3);
	p[4] = (byte)(value >> 8 * 4);
	p[5] = (byte)(value >> 8 * 5);
	p[6] = (byte)(value >> 8 * 6);
	p[7] = (byte)(value >> 8 * 7);
}
#endif
#define ENDIAN_SWAP { while(count--) { EndianSwap(*v++); } }

void EndianSwap(word *v, int count) ENDIAN_SWAP
void EndianSwap(int16 *v, int count) ENDIAN_SWAP
void EndianSwap(dword *v, int count) ENDIAN_SWAP
void EndianSwap(int *v, int count) ENDIAN_SWAP
void EndianSwap(int64 *v, int count) ENDIAN_SWAP
void EndianSwap(uint64 *v, int count) ENDIAN_SWAP

END_UPP_NAMESPACE
