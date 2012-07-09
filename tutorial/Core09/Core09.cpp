#include <Core/Core.h>

using namespace Upp;

struct Foo {
	String a;
	int    b;
	
	unsigned GetHashValue() const { return CombineHash(a, b); }
};

CONSOLE_APP_MAIN
{
	Foo x;
	x.a = "world";
	x.b = 22;

	DUMP(GetHashValue(x));

	x.a << '!';

	DUMP(GetHashValue(x));
}
