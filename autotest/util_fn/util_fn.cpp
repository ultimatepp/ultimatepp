#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	ASSERT(min(4, 76, 8, 23, 5, 7, 3, 1, 7, 34, 8, 4345, 76, 7, 3) == 1);
	ASSERT(max(4, 76, 8, 23, 5, 7, 3, 1, 7, 34, 8, 4345, 76, 7, 3) == 4345);
	ASSERT(clamp(0, 1, 10) == 1);
	ASSERT(clamp(1, 1, 10) == 1);
	ASSERT(clamp(5, 1, 10) == 5);
	ASSERT(clamp(10, 1, 10) == 10);
	ASSERT(clamp(11, 1, 10) == 10);
	
	ASSERT(findarg(4, 0, 1, 2, 3, 4, 5, 6, 7) == 4);
	ASSERT(findarg(8, 0, 1, 2, 3, 4, 5, 6, 7) == -1);

	ASSERT(decode(4, 0, 10, 1, 11, 2, 12, 3, 13, 4, 14, 5, 15, 16) == 14);
	ASSERT(decode(20, 0, 10, 1, 11, 2, 12, 3, 13, 4, 14, 5, 15, 16) == 16);
	
	LOG("=================== OK");
}
