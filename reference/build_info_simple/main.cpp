// if build_info.h is used this way, this file will get recompiled each time (as build_info.h
// always changes)

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
#ifdef bmGIT_REVCOUNT
	LOG("Git revcount " << bmGIT_REVCOUNT);
#endif
#ifdef bmGIT_HASH
	LOG("Git hash " << bmGIT_HASH);
#endif
#ifdef bmGIT_BRANCH
	LOG("Git branch " << bmGIT_BRANCH);
#endif
}
