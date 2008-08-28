#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(int i = 1; i < 1000; i++) {
		DUMP(Random());
		DUMP(Random(i));
	}
}
