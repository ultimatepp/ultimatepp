#include <Core/Core.h>

int& GLOBAL_V(int, mytest);

CONSOLE_APP_MAIN
{
	mytest();
}

