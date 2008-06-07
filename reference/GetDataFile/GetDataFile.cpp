#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	puts(LoadFile(GetDataFile("data.txt")));
}
