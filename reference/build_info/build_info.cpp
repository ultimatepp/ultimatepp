// we place this in separate file to speedup compilation
// otherwise main.cpp has to be compiled each time as build_info.h is always new
// and compiling this minimal file is much faster

#include <build_info.h>

int bm_YEAR = bmYEAR;
int bm_MONTH = bmMONTH;
int bm_DAY = bmDAY;
int bm_HOUR = bmHOUR;
int bm_MINUTE = bmMINUTE;
int bm_SECOND = bmSECOND;

const char *bm_MACHINE = bmMACHINE;
const char *bm_USER = bmUSER;

const char *bm_GIT_BRANCH =
#ifdef bm_GIT_BRANCH
	bmGIT_BRANCH
#else
	""
#endif
;
const char *bm_GIT_REVCOUNT =
#ifdef bmGIT_REVCOUNT
	bmGIT_REVCOUNT
#else
	""
#endif
;
const char *bm_GIT_HASH =
#ifdef bm_GIT_HASH
	bmGIT_HASH
#else
	""
#endif
;
