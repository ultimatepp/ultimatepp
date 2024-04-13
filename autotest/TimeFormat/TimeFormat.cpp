#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	for(const char *s : { "%2:Month %3:d, %1:d;mdy", "%2:MON %3:d, %1:d;mdy" }) {
		String fmt, scan;
		SplitTo(s, ";", fmt, scan);
		SetDateFormat(fmt);
		SetDateScan(scan);
	
		Date d = Date(2024, 4, 13);
	
		DDUMP(d);
		DDUMP(ScanDate(AsString(d)));
	}
	
	CheckLogEtalon();
}
