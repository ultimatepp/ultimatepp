#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	try {
		CoDo([] { throw "Worker exception"; });
	}
	catch(const char *s) {
		LOG("Caught: " << s);
	}
}
