#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ArrayMap<int, int> x, y;
	BREAK_WHEN_PICKED(x);
	x = y;
}
