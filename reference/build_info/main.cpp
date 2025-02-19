#include <Core/Core.h>

using namespace Upp;

// import data from build_info.cpp
extern int bm_YEAR;
extern int bm_MONTH;
extern int bm_DAY;
extern int bm_HOUR;
extern int bm_MINUTE;
extern int bm_SECOND;
extern const char *bm_MACHINE;
extern const char *bm_USER;
extern const char *bm_GIT_REVCOUNT;
extern const char *bm_GIT_BRANCH;
extern const char *bm_GIT_HASH;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	
	Time tm(bm_YEAR, bm_MONTH, bm_DAY, bm_HOUR, bm_MINUTE, bm_SECOND);
	LOG("Compiled " << tm << ", " << (GetSysDate() - (Date)tm) << " day(s) ago");
	LOG("Compiled by user " << bm_USER);
	LOG("Compiled on machine " << bm_MACHINE);
	LOG("Git revcount " << bm_GIT_REVCOUNT);
	LOG("Git hash " << bm_GIT_HASH);
	LOG("Git branch " << bm_GIT_BRANCH);
}
