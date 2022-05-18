#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String s = LoadFile("/proc/meminfo");
	Cout() << s;
	ASSERT(s.GetCount() > 0);
	ASSERT(LoadFile("/proc/this_file_does_not_exist").IsVoid());
}
