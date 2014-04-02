#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_FILE|LOG_COUT);

	for(int i = 0; i < 30000; i++) {
		Date d = Date(1970, 1, 1) + i;
		int64 n = GetUTCSeconds(ToTime(d));
//		DLOG(d << ' ' << GetLeapSeconds(d) << ' ' << n);
		ASSERT(TimeFromUTC(n) == ToTime(d));
	}
	LOG("=========== OK");
}
