#include <Core/Core.h>

using namespace Upp;

void Test(int ini, int re)
{
	RLOG("----------");
	RLOG("Test trying realloc from " << ini << " to " << re);
	byte *h = new byte[ini];

	RDUMP(GetMemoryBlockSize(h));
	RDUMP(TryRealloc(h, re));
	RDUMP(GetMemoryBlockSize(h));
	
	delete[] h;
}

CONSOLE_APP_MAIN
{
	Test(200, 220);
	Test(194, 204);
	Test(2000, 2070);
	Test(2000, 2273);
	Test(500000, 600000);
	Test(500000, 500002);
}
