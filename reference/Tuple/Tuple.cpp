#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Tuple<int, String> x = MakeTuple(12, (const char *)"hello");
	
	DUMP(x.a);
	DUMP(x.b);
	DUMP(x);

	DUMP(x.GetCount());
	DUMP(x.Get(0));
	DUMP(x.Get(1));
	DUMP(x.GetArray());
	DUMP(x.Get<0>());
	DUMP(x.Get<String>());
	
	x.Set(1, "bar");
	DUMP(x);
	ValueArray va { 22, "world" };
	x.SetArray(va);
	DUMP(x);
	
	Tuple<int, String> y = MakeTuple<int, String>(13, "hello");
	
	DUMP(x == y);
	y = x;
	DUMP(x == y);
	
	int i; String s;
	Tie(i, s) = x;
	DUMP(i);
	DUMP(s);
	
	Index< Tuple<int, String> > ndx;
	ndx.Add(x);
	ndx.Add(y);
	
	DDUMP(ndx.Find(x));
	DDUMP(ndx.Find(y));
}
