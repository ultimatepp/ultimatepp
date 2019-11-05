#include "Core.h"

namespace Upp {

#ifdef CPU_X86

#ifdef COMPILER_MSC
#include <intrin.h> 
#else
#include <cpuid.h>
#endif

static bool sHasMMX;
static bool sHasSSE;
static bool sHasSSE2;
static bool sHasSSE3;
static bool sHypervisor;

static void sCheckCPU()
{
	static bool done;
	if(done) return;
	done = true;
	ONCELOCK {
		unsigned int eax, ebx, ecx, edx;
		#ifdef COMPILER_MSC
			int cpuInfo[4];
			__cpuid(cpuInfo, 1);
			eax = cpuInfo[0];
			ebx = cpuInfo[1];
			ecx = cpuInfo[2];
			edx = cpuInfo[3];
		#else
			__get_cpuid(1, &eax, &ebx, &ecx, &edx);
		#endif
		// https://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits
		sHasMMX = edx & (1 << 23);
		sHasSSE = edx & (1 << 25);
		sHasSSE2 = edx & (1 << 26);
		sHasSSE3 = ecx & 1;
		sHypervisor = ecx & (1 << 31);
	}
}

INITBLOCK {
//	sCheckCPU();
}

bool CpuMMX()        { sCheckCPU(); return sHasMMX; }
bool CpuSSE()        { sCheckCPU(); return sHasSSE; }
bool CpuSSE2()       { sCheckCPU(); return sHasSSE2; }
bool CpuSSE3()       { sCheckCPU(); return sHasSSE3; }
bool CpuHypervisor() { sCheckCPU(); return sHypervisor; }

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_BSD
#include <sys/param.h>
#include <sys/sysctl.h>
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
#ifdef PLATFORM_BSD
		int mib[2];
		size_t len = sizeof(n);
		mib[0] = CTL_HW;
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &n, &len, NULL, 0);
		n = minmax(n, 1, 256);
#elif defined(PLATFORM_SOLARIS)
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
#else

#ifdef PLATFORM_LINUX
	#ifdef PLATFORM_ANDROID
	#include <cpu-features.h>
	
	int CPU_Cores()
	{
		return android_getCpuCount();
	}
	
	#else
	#include <sys/sysinfo.h>

	int CPU_Cores()
	{
		return minmax(get_nprocs(), 1, 256);
	}
	#endif
#else
int CPU_Cores()
{
	return 1;
}
#endif

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

void EndianSwap(word *v, size_t count) ENDIAN_SWAP
void EndianSwap(int16 *v, size_t count) ENDIAN_SWAP
void EndianSwap(dword *v, size_t count) ENDIAN_SWAP
void EndianSwap(int *v, size_t count) ENDIAN_SWAP
void EndianSwap(int64 *v, size_t count) ENDIAN_SWAP
void EndianSwap(uint64 *v, size_t count) ENDIAN_SWAP

#ifdef COMPILER_MINGW

static int fast_mingw_tlsndx = 0;

void PreallocateFastMingwTls()
{ // preallocate "simple" tls entries so that they are not used up by other code
	ONCELOCK { // Preallocate
		fast_mingw_tlsndx = TlsAlloc();
		while(TlsAlloc() < 60);
	}
}

INITBLOCK {
	PreallocateFastMingwTls();
}

int FastMingwTlsAlloc()
{
	int ndx;
	INTERLOCKED {
		PreallocateFastMingwTls();
		ndx = fast_mingw_tlsndx++;
		ASSERT(ndx <= 60);
	}
	return ndx;
}

#endif

}
