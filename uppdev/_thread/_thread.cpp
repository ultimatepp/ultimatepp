#include <Core/Core.h>

__thread int test;

CONSOLE_APP_MAIN
{
	test++;
}
