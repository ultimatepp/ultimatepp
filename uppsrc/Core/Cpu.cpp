#include "Core.h"

#if defined(CPU_X86) && !defined(COMPILER_MSC)
#include <cpuid.h>
#endif

#ifdef PLATFORM_FREEBSD
#include <sys/vmmeter.h>
#endif

#ifdef PLATFORM_MACOS
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#endif

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_BSD
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <sys/sysinfo.h>
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
			if(__get_cpuid(1, &eax, &ebx, &ecx, &edx))
		#endif
		// https://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits
			{
				sHasMMX = edx & (1 << 23);
				sHasSSE = edx & (1 << 25);
				sHasSSE2 = edx & (1 << 26);
				sHasSSE3 = ecx & 1;
				sHasAVX = ecx & (1 << 28);
				sHypervisor = ecx & (1 << 31);
			}
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

#endif

#ifdef PLATFORM_ANDROID
#include <cpu-features.h>
int CPU_Cores()
{
	return android_getCpuCount();
}

#else

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

#endif

void GetSystemMemoryStatus(uint64& total, uint64& available)
{
#ifdef PLATFORM_WIN32
	MEMORYSTATUSEX m;
	m.dwLength = sizeof(m);
	if(GlobalMemoryStatusEx(&m)) {
		total = m.ullTotalPhys;
		available = m.ullAvailPhys;
		return;
	}
#endif
#ifdef PLATFORM_LINUX
	int pgsz = getpagesize();
	total = sysconf(_SC_PHYS_PAGES);
	available = sysconf(_SC_AVPHYS_PAGES);
	if(total >= 0 && available >= 0) {
		total *= pgsz;
		available *= pgsz;
		return;
	}
#endif
#ifdef PLATFORM_MACOS
	mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
	vm_statistics_data_t vmstat;

    int mib[2];
    int64 physical_memory;
    size_t length;

    // Get the Physical memory size
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64);

	if(host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count) == KERN_SUCCESS &&
	   sysctl(mib, 2, &physical_memory, &length, NULL, 0) >= 0) {
		int pgsz = getpagesize();
		available = (vmstat.free_count + vmstat.inactive_count) * pgsz;
//		available = physical_memory - (vmstat.wire_count + vmstat.inactive_count) * pgsz;
		total = physical_memory;
		return;
	}
#endif
#ifdef PLATFORM_FREEBSD
	u_int page_size;
    struct vmtotal vmt;
	size_t vmt_size, uint_size;

    vmt_size = sizeof(vmt);
    uint_size = sizeof(page_size);

    if(sysctlbyname("vm.vmtotal", &vmt, &vmt_size, NULL, 0) >= 0 &&
       sysctlbyname("vm.stats.vm.v_page_size", &page_size, &uint_size, NULL, 0) >= 0) {
		available = vmt.t_free * (u_int64_t)page_size;
		total = vmt.t_avm * (u_int64_t)page_size;
		return;
    }
#endif
	total = 256*1024*1024;
	available = 16*1024*1024;
}

#define ENDIAN_SWAP { while(count--) { EndianSwap(*v++); } }

void EndianSwap(word *v, size_t count) ENDIAN_SWAP
void EndianSwap(int16 *v, size_t count) ENDIAN_SWAP
void EndianSwap(dword *v, size_t count) ENDIAN_SWAP
void EndianSwap(int *v, size_t count) ENDIAN_SWAP
void EndianSwap(int64 *v, size_t count) ENDIAN_SWAP
void EndianSwap(uint64 *v, size_t count) ENDIAN_SWAP

}
