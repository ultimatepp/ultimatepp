#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_FILE|LOG_COUT);

	for(int i = 0; i < 30000; i++) {
		Date d = Date(1970, 1, 1) + i;
		int64 n = GetUTCSeconds(ToTime(d));
//		DLOG(d << ' ' << GetLeapSeconds(d) << ' ' << n);
		ASSERT(TimeFromUTC(n) == ToTime(d));
		
		Date d2 = d;
		ASSERT(d2++ == d);
		ASSERT(d2 == d + 1);
		ASSERT(d2-- == d + 1);
		ASSERT(d2 == d);
	}

	ASSERT(GetMonths(Date(2013, 2, 5), Date(2013, 3, 4)) == 0);
	ASSERT(GetMonths(Date(2013, 2, 5), Date(2013, 3, 5)) == 1);
	ASSERT(GetMonths(Date(2013, 8, 31), Date(2013, 9, 30)) == 0);
	ASSERT(GetMonths(Date(2013, 8, 31), Date(2013, 10, 1)) == 1);

	ASSERT(GetMonthsP(Date(2016, 1, 1), Date(2016, 1, 1)) == 0);
	ASSERT(GetMonthsP(Date(2016, 1, 1), Date(2016, 1, 2)) == 1); // partial
	ASSERT(GetMonthsP(Date(2016, 1, 1), Date(2016, 2, 1)) == 1); // exact one
	ASSERT(GetMonthsP(Date(2016, 1, 1), Date(2016, 2, 2)) == 2); // partial 2nd
	
	LOG("=========== OK");
}
