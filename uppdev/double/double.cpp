#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	double x = 0;
	for(int i = 0; i < 10000000; i++)
		x += 0.01;
	DUMP(x);
}

