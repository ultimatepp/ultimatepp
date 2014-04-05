#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Tuple3<int, String, String> x = MakeTuple<int, String, String>(12, "hello", "world");

	DUMP(x.a);
	DUMP(x.b);
	DUMP(x.c);
	
	DUMP(x);

	DUMP(GetHashValue(x));

	Tuple3<int, String, String> y = x;

	DUMP(x != y);
	DUMP(x.Compare(y));
	
	y.b = "a";
	
	DUMP(SgnCompare(x, y));
	DUMP(x < y);
	
	static Tuple2<int, const char *> map[] = {
		{ 1, "one" },
		{ 2, "one" },
		{ 3, "one" },
	};
	
	Tuple2<int, const char *> *f = FindTuple(map, __countof(map), 2);
	if(f)
		DUMP(f->b);
}
