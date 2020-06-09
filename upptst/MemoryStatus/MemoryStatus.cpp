#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	uint64 total, available;
	GetSystemMemoryStatus(total, available);
	
	LOG("Total memory: " << (total >> 20) << " MB");
	LOG("Available memory: " << (available >> 20) << " MB");
}
