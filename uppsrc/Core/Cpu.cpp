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

#define ENDIAN_SWAP { while(count--) { EndianSwap(*v++); } }

void EndianSwap(word *v, size_t count) ENDIAN_SWAP
void EndianSwap(int16 *v, size_t count) ENDIAN_SWAP
void EndianSwap(dword *v, size_t count) ENDIAN_SWAP
void EndianSwap(int *v, size_t count) ENDIAN_SWAP
void EndianSwap(int64 *v, size_t count) ENDIAN_SWAP
void EndianSwap(uint64 *v, size_t count) ENDIAN_SWAP

#ifdef CPU_X86
void huge_memsetd(void *p, dword c, size_t len)
{ // bypasses the cache, good for >4MB
	dword *t = (dword *)p;
	if(((uintptr_t)t & 3) == 0 && len > 64) {
		__m128i val4 = _mm_set1_epi32(c);
		auto Set4S = [&](int at) { _mm_stream_si128((__m128i *)(t + at), val4); };
		while((uintptr_t)t & 15) { // align to 16 bytes for SSE
			*t++ = c;
			len--;
		}
		while(len >= 16) {
			Set4S(0); Set4S(4); Set4S(8); Set4S(12);
			t += 16;
			len -= 16;
		}
		_mm_sfence();
	}
	while(len--)
		*t++ = c;
}

void memsetd_l(dword *t, dword data, size_t len)
{
	__m128i val4 = _mm_set1_epi32(data);
	auto Set4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), val4); };
	Set4(len - 4); // fill tail
	if(len >= 32) {
		if(len >= 1024*1024) { // for really huge data, bypass the cache
			huge_memsetd(t, data, len);
			return;
		}
		Set4(0); // align up on 16 bytes boundary
		const dword *e = t + len;
		t = (dword *)(((uintptr_t)t | 15) + 1);
		len = e - t;
		e -= 32;
		while(t <= e) {
			Set4(0); Set4(4); Set4(8); Set4(12);
			Set4(16); Set4(20); Set4(24); Set4(28);
			t += 32;
		}
	}
	if(len & 16) {
		Set4(0); Set4(4); Set4(8); Set4(12);
		t += 16;
	}
	if(len & 8) {
		Set4(0); Set4(4);
		t += 8;
	}
	if(len & 4)
		Set4(0);
}

void memcpyd_l(dword *t, const dword *s, size_t len)
{
	auto Copy4 = [&](size_t at) { _mm_storeu_si128((__m128i *)(t + at), _mm_loadu_si128((__m128i *)(s + at))); };

	Copy4(0); // align target data up on next 16 bytes boundary
	const dword *e = t + len;
	dword *t1 = (dword *)(((uintptr_t)t | 15) + 1);
	s += t1 - t;
	t = t1;
	len = e - t;
	e -= 16;
	if(len >= 1024*1024) { // for really huge data, call memcpy to bypass the cache
		memcpy(t, s, 4 * len);
		return;
	}
	while(t <= e) {
		Copy4(0); Copy4(4); Copy4(8); Copy4(12);
		t += 16;
		s += 16;
	}
	if(len & 8) {
		Copy4(0); Copy4(4);
		t += 8;
		s += 8;
	}
	if(len & 4)
		Copy4(0);
}
#endif

#ifdef CPU_UNALIGNED
never_inline
void svo_memset_l(byte *t, dword val4, size_t len)
{
	const byte *e = t + len;
	t = (byte *)(((uintptr_t)t | 3) + 1);
	len = e - t;
	memsetd(t, val4, len >> 2);
}

never_inline
void svo_memcpy_l(byte *t, byte *s, size_t len)
{
	const byte *e = t + len;
	byte *t2 = (byte *)(((uintptr_t)t | 3) + 1);
	s += t2 - t;
	t = t2;
	len = e - t;
	memcpyd((dword *)t, (dword *)s, len >> 2);
}
#endif

}
