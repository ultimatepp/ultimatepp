#include <Core/Core.h>

using namespace Upp;

struct Foo {
	int bar;
	
	Foo() { bar = 0; }
};

bool TestFn(Gate1<Foo> gate)
{
	LOG("In TestFn");
	Foo foo;
	return gate(foo);
}

bool TestGate(Foo x) {
	LOG("In ReservationGate");
	return true;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	TestFn(TestGate);
//	TestFn(callback(TestGate));
}
