#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	LOG("Test");
	LOGBEGIN();
	LOG("Test 1");
	LOG("Test A");
	LOGEND();
	LOG("Test 2");
	LOG("Test B");
}
