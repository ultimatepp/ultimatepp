#include <Core/Core.h>

using namespace Upp;

#define TEST(a, b) \
{ Time av = a; \
LOG(#a << " -> " << av); \
LOG(b); \
ASSERT(a == b); \
LOG("==================="); }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	DUMP(GetTimeZoneText());

	DUMP(GetTimeZone());	
	Date d = GetSysDate();
	if(d < Date(d.year, 3, 20) && d > Date(d.year, 10, 30)) {
		LOG("Normal CZ time");
		ASSERT(GetTimeZone() == 60);
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1200"), Time(2014, 3, 25, 23, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1000"), Time(2014, 3, 25, 21, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0100"), Time(2014, 3, 25, 12, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0000"), Time(2014, 3, 25, 11, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -0100"), Time(2014, 3, 25, 10, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1000"), Time(2014, 3, 25,  1, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1200"), Time(2014, 3, 24, 23, 1, 15));
	}
	if(d > Date(d.year, 3, 30) && d < Date(d.year, 10, 20)) {
		LOG("Daylight saving CZ time");
		ASSERT(GetTimeZone() == 120);
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1200"), Time(2014, 3, 25,  2, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1000"), Time(2014, 3, 25,  4, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0100"), Time(2014, 3, 25, 13, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0000"), Time(2014, 3, 25, 14, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -0100"), Time(2014, 3, 25, 15, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1000"), Time(2014, 3, 26,  0, 1, 15));
		TEST(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1200"), Time(2014, 3, 26,  2, 1, 15));
	}
	ASSERT(ScanTimeZone("-1000") == -600);
	DUMP(ScanTimeZone(GetTimeZoneText()));
	ASSERT(ScanTimeZone("+1000") == 600);
	DUMP(WwwFormat(GetSysTime()));
	
	for(int i = 0; i < 24 * 60 * 60 * 400; i += 133) {
		if(i % 1000 * 133 == 0)
			LOG("Done " << i);
		Time tm = Time(2010, 1, 1) + i;
		ASSERT(ScanWwwTime(WwwFormat(tm)) == tm);		
	}
	
	LOG("================== OK");
}
