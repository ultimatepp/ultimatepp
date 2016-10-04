#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	MEMORYSTATUS m;
	GlobalMemoryStatus(&m);
	DUMP(m.dwTotalPhys);
	DUMP(IsDecentMachine());
}

