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
	
	VectorMap<String, String> y;
	y.Add("Key1", "Value1");
	y.Add("Key2", "Value2");
	DUMPC(y);
	
	static char h[200] = "alsjkdfhlaksjdhfklajshdfklj";
	LOGHEXDUMP(h, 200);
	
	String s = "Some text for hex dumps";
	LOGHEX(s);
	DUMPHEX(s);

	RLOG("Prepending 'R' forces log even in the release mode");
	
	// You need to comment out following line in order to compile in release mode
	DLOG("Prepending 'D' issues error when compiled in release mode. This is to avoid forgotten debug logs");
}
