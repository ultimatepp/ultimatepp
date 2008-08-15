#include <Core/Core.h>

struct Foo : Pte<Foo> {
	virtual void Test() { LOG("BASE!"); }

	int bar;
};

struct Derived : Date, Foo {
	virtual void Test() { LOG("DERIVED!"); }

	int bar2;
};

CONSOLE_APP_MAIN
{
	Ptr<Derived> pd;
	{
		Derived d;
		pd = &d;
		Callback x = callback(&d, &Foo::Test);
		x();
		DUMP((void *)&d);
		DUMP((void *)~pd);
		DUMP(pd);
	}
	DUMP(pd);

	Uuid uuid;
	Ptr<Foo> x;
	{
		Foo a;
		DUMP((void *)&a);
		DUMP(x);
		x = &a;
		DUMP(x);
		uuid = x.GetUuid();
		DUMP(x);
		DUMP(a.GetUuid());
	}
	DUMP(x);
	Foo a;
	a.SetUuid(uuid);
	DUMP(x);
}
