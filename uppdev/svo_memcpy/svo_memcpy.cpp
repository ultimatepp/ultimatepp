#include "svo_memcpy.h"

#define BENCH(fn, n) { RTIMING(#fn " " #n); for(int i = 0; i < 10000000; i++) fn(t, s, n); }

force_inl

CONSOLE_APP_MAIN
{
	char s[20];
	char t[20];

	BENCH(Test1, 11);
	BENCH(Test2, 11);
	BENCH(Test3, 11);
	BENCH(Test4, 11);

	BENCH(Test1, 5);
	BENCH(Test2, 5);
	BENCH(Test3, 5);
	BENCH(Test4, 5);

	BENCH(Test1, 3);
	BENCH(Test2, 3);
	BENCH(Test3, 3);
	BENCH(Test4, 3);

/*
	for(int i = 0; i < 10000000; i++) {
		{
			RTIMING("svo_memcpy 11")	
			Test1(t, s, 11);
		}
		{
			RTIMING("SVO_MEMCPY 11")	
			Test2(t, s, 11);
		}
		{
			RTIMING("memcpy 11")	
			Test3(t, s, 11);
		}
		{
			RTIMING("svo_memcpy2 11")	
			Test4(t, s, 11);
		}
	}

	for(int i = 0; i < 10000000; i++) {
		{
			RTIMING("svo_memcpy 5")	
			Test1(t, s, 5);
		}
		{
			RTIMING("SVO_MEMCPY 5")	
			Test2(t, s, 5);
		}
		{
			RTIMING("memcpy 5")	
			Test3(t, s, 5);
		}
		{
			RTIMING("svo_memcpy2 5")	
			Test4(t, s, 5);
		}
	}
*/
}

