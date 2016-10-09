#include <Core/Core.h>

using namespace Upp;

struct Foo {
	int x;

	void Action()                { Cout() << "Action: " << x << '\n'; }
	void ActionWithParam(int y)  { Cout() << "ActionWithParam: " << x + y << '\n'; }

	Event<> WhenDo;

	void Do()                    { WhenDo(); }

	Foo(int x = 0) : x(x) {}
};

void Fn()
{
	Cout() << "Fn!" << '\n';
}

struct Bar {
	Foo foo;

	Bar() {
		foo.WhenDo = [=] { Cout() << "foo's Do called\n"; };
	}
};

struct Safe : Pte<Safe> {
	void Action() { Cout() << "safe action!\n"; }
};

CONSOLE_APP_MAIN
{
	Foo a(10);

	Event<> ev1, ev2, ev4;
	Event<int> ev3;

	ev1 << [&] { a.Action(); };
	ev2 << [] { Fn(); };
	ev3 << [&](int x) { a.ActionWithParam(x); };
	ev4 << [&] { a.ActionWithParam(30); };

	ev1();
	ev2();
	ev3(10);
	ev4();

	Cout() << "---------\n";
	ev4 << ev2;
	ev4();

	Cout() << "---------\n";
	Bar b;
	b.foo.Do();

	Cout() << "---------\n";
	{
		Safe f;
		Ptr<Safe> p = &f;
		ev4 = [=] { if(p) p->Action(); };
		Cout() << "callback valid: " << (bool)ev4 << '\n';
		ev4();
	} // Safe f gets destroyed here, which makes capture 'p' NULL
	ev4(); // captured 'p' is now NULL, so no Action is called
}
