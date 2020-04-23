#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	HttpRequest::Trace();
	RLOG(HttpRequest("https://www.ultimatepp.org").Execute());
}
