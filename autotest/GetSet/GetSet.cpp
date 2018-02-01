#include <Core/Core.h>

using namespace Upp;

template <class T>
void Test()
{
	T x;
	DDUMP(x.Get(0, "321"));
	ASSERT(x.Get(0, "321") == "321");
	x.Add("123");
	DDUMP(x.Get(0, "321"));
	ASSERT(x.Get(0, "321") == "123");
	String h = "test";
	x.Set(1, pick(h));
	DDUMP(h);
	ASSERT(h.GetCount() == 0);
	DDUMP(x);
	ASSERT(x.GetCount() == 2);
	ASSERT(x[0] == "123");
	ASSERT(x[1] == "test");
}

CONSOLE_APP_MAIN
{
	Test<Vector<String>>();
	Test<Array<String>>();
}
