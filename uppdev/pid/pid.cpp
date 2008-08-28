#include <Core/Core.h>

using namespace Upp;




CONSOLE_APP_MAIN
{
	DUMP(GetExeFilePath());
	DUMP(GetPidExePath(getpid()));
}
