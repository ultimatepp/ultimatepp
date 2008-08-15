#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	Vector<String> test;
	test.Add("1");
	test.Add("2");
	test.Add("3");
	test.Add("5");
	test.Add("4");
	DUMPC(test);
}
