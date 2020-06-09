#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	uint64 free, total;
	GetSystemMemoryStatus(total, free);
	
	LOG("Total memory: " << (total >> 20) << " MB");
	LOG("Free memory: " << (free >> 20) << " MB");
}
