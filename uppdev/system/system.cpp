#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
#if __SIZEOF_POINTER__ == 8
	Cout() << "64\n";
#endif
}
