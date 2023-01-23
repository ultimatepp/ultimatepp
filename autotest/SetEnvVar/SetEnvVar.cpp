#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String cmd, out;
	
#ifdef PLATFORM_WIN32
	cmd = "cmd /C \"echo 1 %FOOBAR% 2\"";
	ASSERT(Sys(cmd) == "1 %FOOBAR% 2\r\n");
	SetEnv("FOOBAR","BAZ");
	ASSERT(Sys(cmd) == "1 BAZ 2\r\n");
#else
	cmd = "echo \"1 $FOOBAR 2\"";
	ASSERT(Sys(cmd) == "1  2\n");
	SetEnv("FOOBAR","BAZ");
	ASSERT(Sys(cmd) == "1 BAZ 2\n");
#endif
	
	ASSERT(GetEnv("FOOBAR") == "BAZ");
}
