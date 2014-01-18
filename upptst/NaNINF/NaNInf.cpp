#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	double d = 0;
	ASSERT(!IsNaN(d));
	ASSERT(!IsInf(d));
	ASSERT(IsFin(d));
	
	d = sqrt(-1.0);
	ASSERT(IsNaN(d));
	ASSERT(!IsInf(d));

	d = 1e300;
	d *= d;
	ASSERT(!IsNaN(d));
	ASSERT(IsInf(d));

	d = 1e300;
	d *= -d;
	ASSERT(!IsNaN(d));
	ASSERT(IsInf(d));
}
