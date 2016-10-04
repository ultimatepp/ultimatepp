#include <Core/Core.h>

struct Bar {
	int x;
	int y;
};

struct Foo {
	int a;
	int b;

	operator Bar() const { return *(Bar *)this; }
};

CONSOLE_APP_MAIN
{
	Size sz(1000, 1000);
	POINT pt = Point(sz);
	Cout() << "Size = " << sz << ", Point = " << Point(sz) << ", POINT = " << Point(pt) << "\n";

	Foo f;
	f.a = 12345;
	f.b = 54321;
	Bar b = f;
	Cout() << b.x << ", " << b.y;
}
