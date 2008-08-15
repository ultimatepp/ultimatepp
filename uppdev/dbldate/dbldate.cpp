#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 365 * 100000; i++) {
		Date d = i + Date(1, 1, 1);
		double x = (d - Date(-4000, 1, 1)) * 1.0E300;
		ASSERT(x >= 1.0E300);
		Date d1 = int(x / 1.0E300 + 0.5) + Date(-4000, 1, 1);
		ASSERT(d == d1);
		if((i & 1023) == 0)
			Cout() << i / 365 << "   " << x << '\n';
	}
}
