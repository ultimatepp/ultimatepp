#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DDUMP(GetTimeZoneText());
	ASSERT(GetTimeZone() == 60);
	ASSERT(ScanTimeZone("-1000") == -600);
	DDUMP(ScanTimeZone(GetTimeZoneText()));
	ASSERT(ScanTimeZone("+1000") == 600);
	DDUMP(WwwFormat(GetSysTime()));
	
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1200") == Time(2014, 3, 25, 23, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +1000") == Time(2014, 3, 25, 21, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0100") == Time(2014, 3, 25, 12, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 +0000") == Time(2014, 3, 25, 11, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -0100") == Time(2014, 3, 25, 10, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1000") == Time(2014, 3, 25,  1, 1, 15));
	ASSERT(ScanWwwTime("Tue, 25 Mar 2014 12:01:15 -1200") == Time(2014, 3, 24, 23, 1, 15));
	
	for(int i = 0; i < 24 * 60 * 60 * 400; i += 133) {
		if(i % 1000 * 133 == 0)
			LOG("Done " << i);
		Time tm = Time(2010, 1, 1) + i;
		ASSERT(ScanWwwTime(WwwFormat(tm)) == tm);		
	}
	
	LOG("================== OK");
}
