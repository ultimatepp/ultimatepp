#include "SysInfo.h"

NAMESPACE_UPP

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
uint64 start, end;
unsigned long nCtr, nFreq, nCtrStop;

#if defined(__MINGW32__)
uint64 __rdtsc() {
	#if defined(__MINGW64__)	// It does not work for now
      	unsigned int lo, hi;
      	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
      	return uint64(lo) | (uint64(hi) << 32); 
	#else
      	unsigned int lo, hi;
      	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
      	return uint64(lo) | (uint64(hi) << 32); 
	#endif
}
#endif

int GetCpuSpeed()
{
    if(!QueryPerformanceFrequency((LARGE_INTEGER *) &nFreq)) 
    	return 0;
    QueryPerformanceCounter((LARGE_INTEGER *) &nCtrStop);
    nCtrStop += nFreq;								
    
    start = __rdtsc();
    do 
         QueryPerformanceCounter((LARGE_INTEGER *) &nCtr);
    while (nCtr < nCtrStop);
    end = __rdtsc();
	return int((end - start)/1000000);
}

#endif

#if defined(PLATFORM_POSIX)

#define __rdtsc(tm) __asm__ __volatile__ (".byte 0x0f; .byte 0x31" :"=a" (tm))
#define COUNT_SEC   (double)tv.tv_sec + (1.e-6)*tv.tv_usec

int GetCpuSpeed()
{
	struct timeval tv;
  	double cnt1, cnt2;
  	unsigned long start, end;

  	__rdtsc(start);
  	gettimeofday(&tv, 0);
  	cnt1 = COUNT_SEC + 0.01;

  	do {
    	gettimeofday(&tv, 0);
    	cnt2 = COUNT_SEC;
  	} while(cnt2 < cnt1);

  	__rdtsc(end);

  	return int((end-start)/10000);
}
#endif

END_UPP_NAMESPACE