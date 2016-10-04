#include <Core/Core.h>

using namespace Upp;

int IScale(int x, int y, int z);

CONSOLE_APP_MAIN
{
	__BREAK__;
	RDUMP(IScale(100, 200, 300));
}
