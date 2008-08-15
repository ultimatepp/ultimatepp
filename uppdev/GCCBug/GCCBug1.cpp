#include "GCCBug.h"

char array[256];

void *MemoryAllocSz(size_t& sz)
{
	printf("%d\n", sz);
	return array;
}

void MemoryFree(void *) {}
