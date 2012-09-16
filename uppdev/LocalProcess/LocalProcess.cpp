#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String out;
	Sys("ls /sdf", out);
	DDUMP(out);
}
