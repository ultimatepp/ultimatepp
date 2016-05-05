#include <Core/Core.h>

using namespace Upp;

void Foo()
{
	LOG("Foo");
}

void Bar()
{
	LOG("Bar");
}

CONSOLE_APP_MAIN
{
	Event<> a, b;
	a = Proxy(b);
	b = callback(Foo);
	a();
	b = callback(Bar);
	a();
}
