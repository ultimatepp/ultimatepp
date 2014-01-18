#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	double d = 0;
	ASSERT(!IsNaN(d));
	ASSERT(!IsInf(d));
	ASSERT(IsFin(d));
	
	d = sqrt(StrDbl("-1.0"));
	ASSERT(IsNaN(d));
	ASSERT(!IsInf(d));
	ASSERT(!IsFin(d));

	d = 1e300;
	d *= d;
	ASSERT(!IsNaN(d));
	ASSERT(IsInf(d));
	ASSERT(!IsFin(d));

	d = 1e300;
	d *= -d;
	ASSERT(!IsNaN(d));
	ASSERT(IsInf(d));
	ASSERT(!IsFin(d));
	
	LOG("Everything OK");
}
