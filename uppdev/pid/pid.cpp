#include <Core/Core.h>

using namespace Upp;

String GetPidExePath(int pid)
{
	static char h[_MAX_PATH + 1];
	char link[100];
#ifdef PLATFORM_FREEBSD
	sprintf(link, "/proc/%d/file", pid);
#else
	sprintf(link, "/proc/%d/exe", pid);
#endif
	int ret = readlink(link, h, _MAX_PATH);
	if(ret > 0 && ret < _MAX_PATH)
		h[ret] = '\0';
	else
		*h = '\0';
	return h;
}

CONSOLE_APP_MAIN
{
	DUMP(GetExeFilePath());
	DUMP(GetPidExePath(getpid()));
}
