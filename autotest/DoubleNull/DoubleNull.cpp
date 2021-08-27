#include <Core/Core.h>

using namespace Upp;

double zero;

CONSOLE_APP_MAIN
{
	double x = Null;
	double y = 1e-306;
	RDUMP(IsNull(x));
	RDUMP(IsNull(y));
	RDUMP(IsNull(1e300));
	RDUMP(IsNull(1e-320));
	RDUMP(IsNull(DBL_MAX));
	RDUMP(IsNull(0));
	RDUMP(IsNull(log(-1)));
	RDUMP(IsNull(1 / zero));

	CheckLogEtalon();
}
