#include <Core/Core.h>

using namespace Upp;

struct Foo {
	int x;

	void Action()                { Cout() << "Action: " << x << '\n'; }
	void ActionWithParam(int y)  { Cout() << "ActionWithParam: " << x + y << '\n'; }

	Callback WhenDo;
	void Do()                    { WhenDo(); }

	Foo(int x = 0) : x(x) {}
};

void Fn()
{
	Cout() << "Fn!" << '\n';
}

struct Bar {
	Foo foo;

	void Action() { Cout() << "foo's Do called\n"; }

	typedef Bar CLASSNAME;

	Bar() { foo.WhenDo = THISBACK(Action); }
};

struct Safe : Pte<Safe> {
	void Action() { Cout() << "safe action!\n"; }
};

CONSOLE_APP_MAIN
{
	Foo a(10);
	Callback cb1 = callback(&a, &Foo::Action);
	Callback cb2 = callback(Fn);
	Callback1<int> cb3 = callback(&a, &Foo::ActionWithParam);
	Callback cb4 = callback1(&a, &Foo::ActionWithParam, 30);

	cb1();
	cb2();
	cb3(10);
	cb4();

	Cout() << "---------\n";
	cb4 << cb2;
	cb4();

	Cout() << "---------\n";
	Bar b;
	b.foo.Do();

	Cout() << "---------\n";
	{
		Safe f;
		cb4 = pteback(&f, &Safe::Action);
		Cout() << "callback valid: " << (bool)cb4 << '\n';
		cb4();
	}
	Cout() << "callback valid: " << (bool)cb4 << '\n';
	cb4();
}
