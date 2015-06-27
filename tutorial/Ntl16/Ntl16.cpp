#include <Core/Core.h>

using namespace Upp;

struct Foo {
	String a;
	
	int Get() const { return atoi(a); }
	String ToString() const { return a; }
	
	Foo(const char *a) : a(a) {}
};

CONSOLE_APP_MAIN
{
	Vector<int> a;
	Vector<String> b;	
	
	a << 5 << 10 << 2 << 9 << 7 << 3;
	b << "five" << "ten" << "two" << "nine" << "seven" << "three";
	
	IndexSort(a, b);
	DUMP(a);
	DUMP(b);
	
	IndexSort(b, a);
	DUMP(a);
	DUMP(b);
	
	Sort(a, StdGreater<int>());
	DUMP(a);
	
	Vector<int> o = GetSortOrder(a);
	DUMP(o);

	Vector<Point> p;
	p << Point(5, 10) << Point(7, 2) << Point(4, 8) << Point(1, 0);
	
	Sort(p, FieldRelation(&Point::x, StdLess<int>()));
	DUMP(p);
	
	Array<Foo> f;
	f << "12" << "1" << "10" << "7" << "5";
	
	Sort(f, MethodRelation(&Foo::Get, StdLess<int>()));
	DUMP(f);

	Vector<Point> t;
	t << Point(10, 10) << Point(7, 1) << Point(7, 2) << Point(7, 3) << Point(1, 0);
	Sort(t, FieldRelation(&Point::x, StdLess<int>()));
	DUMP(t);
}
