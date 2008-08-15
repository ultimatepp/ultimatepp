#include <Core/Core.h>

using namespace Upp;

__declspec(thread) int x;

CONSOLE_APP_MAIN
{
	x += 10;
}
