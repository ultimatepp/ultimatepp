#include <Core/Core.h>
#include <build_info.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#ifdef bmTIME
	LOG("Compiled " << bmTIME);
#endif
#ifdef bmSVN_REVISION
	LOG("Svn revision " << bmSVN_REVISION);
#endif
}
