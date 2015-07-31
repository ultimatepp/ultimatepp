#include <Core/Core.h>
#include <build_info.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#ifdef bmTIME
	LOG("Compiled " << bmTIME << ", " << (GetSysDate() - (Date)bmTIME) << " day(s) ago");
#endif
#ifdef bmUSER
	LOG("Compiled by user " << bmUSER);
#endif
#ifdef bmMACHINE
	LOG("Compiled on machine " << bmMACHINE);
#endif
#ifdef bmSVN_REVISION
	LOG("Svn revision " << bmSVN_REVISION);
#endif
#ifdef bmSVN_REVISION
	LOG("Svn url " << bmSVN_URL);
#endif
}
