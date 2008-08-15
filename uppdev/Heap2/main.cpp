#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN {
	for(int i = 0; i < 16384; i += 32)
		LOG(i << "   " << (i >> (5 + (i > 1024))));
}
