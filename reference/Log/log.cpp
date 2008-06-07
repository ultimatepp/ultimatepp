#include <Core/Core.h>

using namespace Upp;

struct Foo {
	int i;

	String ToString() const { return "Foo: " + AsString(i); }

	Foo() { i = 0; }
	Foo(int i) : i(i) {}
};

CONSOLE_APP_MAIN
{
	LOG(123);
	LOG("Loop:");
	LOGBEGIN();
	for(int i = 0; i < 4; i++)
		DUMP(i);
	LOGEND();
	DUMP(Foo(1));
	Vector<String> x;
	x.Add("One");
	x.Add("Two");
	x.Add("Three");
	DUMPC(x);
	static char h[200] = "alsjkdfhlaksjdhfklajshdfklj";
	LOGHEXDUMP(h, 200);
	RLOG("This will appear even in the release mode");
}
