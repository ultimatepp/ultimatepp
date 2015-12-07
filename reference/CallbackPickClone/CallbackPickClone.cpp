#include <Core/Core.h>

using namespace Upp;

void Function(const Vector<String>& h)
{
	LOG("Callback invoked with " << h);
}

CONSOLE_APP_MAIN
{
	Vector<String> v;
	v << "Just" << "a" << "test";
	
	Callback cb1 = callback1(Function, clone(v));
	LOG("Source picked: " << v.IsPicked());
	LOG("Source: " << v);

	v << "2";
	Callback cb2 = callback1(Function, pick(v));
	LOG("Source picked: " << v.IsPicked());

	cb1();
	cb2();
}
