#include "Core/Core.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StringStream ss;
	ss.Put("Ahoj");
	ss.Reserve(500);
	ss.Put("xxx", 3);
	DDUMP(ss.GetResult());
}
