#include "svo_memcpy.h"

void Test1(void *t, void *s, int n)
{
	svo_memcpy(t, s, n);
}

void Test2(void *t, void *s, int n)
{
	SVO_MEMCPY(t, s, n);
}

void Test3(void *t, void *s, int n)
{
	memcpy(t, s, n);
}

void Test4(void *t, void *s, int n)
{
	svo_memcpy2(t, s, n);
}
