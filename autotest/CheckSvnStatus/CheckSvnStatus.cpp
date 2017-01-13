#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	ASSERT(IsNull(Sys("svn status /home/cxl/upp.src")));
	
	LOG("=========== OK");
}
