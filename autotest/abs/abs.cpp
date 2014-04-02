#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	ASSERT(abs(-1) == 1);
	ASSERT(abs((int16)-1) == 1);
	ASSERT(abs((int64)-1) == 1);
	ASSERT(abs((int8)-1) == 1);
	ASSERT(abs((float)-1) == 1);
	ASSERT(abs((double)-1) == 1);
	LOG("OK");
}
