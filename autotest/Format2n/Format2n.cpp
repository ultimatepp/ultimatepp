#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	double x = 1e25;
	for(int i = 0; i < 50; i++) {
		DDUMP(Format("%2.n", x));
		x = x * 0.1;
	}
	CheckLogEtalon();
}
