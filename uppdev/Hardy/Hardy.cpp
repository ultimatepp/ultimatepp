#include <Core/Core.h>

using namespace Upp;

#include <Core/Core.h>

using namespace Upp;

void AllocTest(void)
{
	RLOG("AllocTest -- Entering");
	const int numAllocs = 100;
	size_t sizeAlloc = 32;
	void *p[numAllocs];
	for(int i = 0; i < numAllocs; i++)
		p[i] = MemoryAllocSz(sizeAlloc);
	RLOG("AllocTest -- Memory allocated");
	for(int i = 0; i < numAllocs; i++)
		MemoryFree(p[i]);
	RLOG("AllocTest -- Memory freed");
}

void Dummy(String s) {}

CONSOLE_APP_MAIN
{
	const char *s1 = "123456789qwert";
	const char *s2 = "123456789qwerty";
	
	RLOG("\n\n================================================================\n");
	RLOG("AllocTest before....");
	AllocTest();
	
	Dummy(s1);
	
	RLOG("AllocTest after s1");
	AllocTest();
	
	Dummy(s2);
	
	RLOG("AllocTest after s2");
	AllocTest();
	
	exit(0);
}
