#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	DUMP(GetHomeDirectory());
	DUMP(GetHomeDirFile("test.cfg"));
}

