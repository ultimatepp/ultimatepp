#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_FILE|LOG_COUT);

	Date d = GetSysDate();
	Time t = GetSysTime();
	String h = Json("date", d)("time", t)("array", JsonArray() << d << t);
	DDUMP(h);
	
	Value v = ParseJSON(h);
	ASSERT(v["date"] == d);
	ASSERT(v["time"] == t);
	ASSERT(v["array"][0] == d);
	ASSERT(v["array"][1] == t);
}
