#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Rect r(2, 4, 6, 8);
	
	ASSERT(r * 2 == Rect(4, 8, 12, 16));
	ASSERT(r / 2 == Rect(1, 2, 3, 4));
	
	LOG("================= OK");
}
