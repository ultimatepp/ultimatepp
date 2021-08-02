#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	chdir("/home/cxl/upp.src");
	ASSERT(Sys("git status").Find("nothing to commit, working tree clean") >= 0);

	LOG("=========== OK");
}
