#include <Core/Core.h>

using namespace Upp;

struct Base { int x; virtual ~Base() {} };
struct Derived : Base { int y; };

CONSOLE_APP_MAIN
{
	One<int> x;
	x.Create();

	One<Base> o;

	ASSERT(!o.Is<Base>());
	ASSERT(!o.Is<Derived>());

	o.Create();
	ASSERT(o.Is<Base>());
	ASSERT(!o.Is<Derived>());
	
	o.Create<Derived>();
	ASSERT(o.Is<Base>());
	ASSERT(o.Is<Derived>());
}
