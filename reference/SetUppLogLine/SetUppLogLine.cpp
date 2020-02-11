#include <Core/Core.h>

using namespace Upp;

LogLineFn std_log;

void MyLogLine(const char *buffer, int len, int depth)
{
	Cout() << "MYLOG: " << String(buffer, len) << "\n";
	std_log(buffer, len, depth); // call standard log too
}

CONSOLE_APP_MAIN {
	std_log = SetUppLog(MyLogLine);
	LOG("It Works!");
}
