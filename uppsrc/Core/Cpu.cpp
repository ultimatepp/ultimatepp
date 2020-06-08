#include "Core.h"

#ifdef CPU_X86
#ifdef COMPILER_MSC
#include <intrin.h> 
#else
#include <cpuid.h>
#endif
#endif

namespace Upp {

#ifdef CPU_X86

static bool sHasMMX;
static bool sHasSSE;
static bool sHasSSE2;
static bool sHasSSE3;
static bool sHasAVX;
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
		sHasAVX = ecx & (1 << 28);
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
bool CpuAVX()        { sCheckCPU(); return sHasAVX; }
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

void GetSystemMemoryStatus(uint64& total, uint64& free)
{
#ifdef PLATFORM_WIN32
	MEMORYSTATUSEX m;
	GlobalMemoryStatusEx(&m);
	total = m.ullTotalPhys;
	free = m.ullAvailPhys;
#elif defined(PLATFORM_LINUX)
	int pgsz = getpagesize();
	total = sysconf(_SC_PHYS_PAGES) * pgsz;
	free = sysconf(_SC_AVPHYS_PAGES) * pgsz;
#else
	total = 512*1024*1024;
	free = 16*1024*1024;
#endif
}

#define ENDIAN_SWAP { while(count--) { EndianSwap(*v++); } }

void EndianSwap(word *v, size_t count) ENDIAN_SWAP
void EndianSwap(int16 *v, size_t count) ENDIAN_SWAP
void EndianSwap(dword *v, size_t count) ENDIAN_SWAP
void EndianSwap(int *v, size_t count) ENDIAN_SWAP
void EndianSwap(int64 *v, size_t count) ENDIAN_SWAP
void EndianSwap(uint64 *v, size_t count) ENDIAN_SWAP

}
