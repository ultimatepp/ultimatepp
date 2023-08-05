#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	DDUMP(FormatE(0.0, 2));
	DDUMP(FormatE(0.0,  2, FD_CAP_E | FD_EXP | FD_MINIMAL_EXP));
	DDUMP(FormatE(1234, 2, FD_CAP_E | FD_EXP | FD_MINIMAL_EXP));
	
	CheckLogEtalon();
}
