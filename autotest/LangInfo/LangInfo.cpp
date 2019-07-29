#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	const LanguageInfo& f = GetLanguageInfo();
	DUMP(f);
	ASSERT(f.decimal_point == ".");
	ASSERT(f.thousand_separator == ",");
	ASSERT(f.month[0] == "January");
	ASSERT(f.smonth[0] == "Jan");
	ASSERT(f.day[0] == "Monday");
	ASSERT(f.sday[0] == "Mon");
	LOG("============= OK");
}
