#include <Core/Core.h>

#include <build_info.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DUMP(bmTIME);
	DUMP(bmMACHINE);
	DUMP(bmUSER);
}
