#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Tuple2<int, String> x = Tuple(12, (const char *)"hello");
	
	DUMP(x.a);
	DUMP(x.b);
	DUMP(x);
	
	x.SetA(1);
	DUMP(x);
	
	x.Set(0, "bar");
	DUMP(x);
	
	Tuple2<int, String> y = Tuple<int, String>(13, "hello");
	
	DUMP(x == y);
	y = x;
	DUMP(x == y);
	
	int i; String s;
	Tie(i, s) = x;
	DUMP(i);
	DUMP(s);
	
	Index< Tuple2<int, String> > ndx;
	ndx.Add(x);
	ndx.Add(y);
	
	DDUMP(ndx.Find(x));
	DDUMP(ndx.Find(y));
}
