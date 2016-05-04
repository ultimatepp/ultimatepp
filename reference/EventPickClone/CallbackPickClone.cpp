#include <Core/Core.h>

using namespace Upp;

void Do(const Vector<String>& h)
{
	LOG("Event invoked with " << h);
}

CONSOLE_APP_MAIN
{
	Vector<String> v;
	v << "Just" << "a" << "test";
	
	Event<> ev1;
	ev1 << [v = clone(v)] { Do(v); };
	DUMP(v);

	v << "2";
	Event<> ev2;
	ev2 << [v = pick(v)] { Do(v); };
	DUMP(v);

	ev1();
	ev2();
}
