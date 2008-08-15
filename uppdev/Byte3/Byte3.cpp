#include <Core/Core.h>

using namespace Upp;

struct Test {
	byte a, b, c;
	
	operator int() const { return MAKELONG(c, MAKEWORD(a, b)); }
};

CONSOLE_APP_MAIN
{
	RDUMP(sizeof(Test));
	Test test[10];
	test[0].a = rand();
	test[0].b = rand();
	test[0].c = rand();
	RDUMP(sizeof(test));
	RDUMP((int)test[0]);
}
