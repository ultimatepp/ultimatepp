#include <Core/Core.h>

using namespace Upp;

struct Foo : Tuple2<int, int> {
	int c;
	
	Foo(int a, int b, int c) : Tuple2<int, int>(a, b), c(c) {}
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	{
		BiVector<Tuple2<int, int>> m;
		
		m.CreateTail(3, 4);
		m.CreateHead(1, 2);
		
		DDUMP(m);
	}

	{
		BiArray<Tuple2<int, int>> m;
		
		m.CreateTail(7, 8);
		m.CreateHead(5, 6);
		
		DDUMP(m);
	}

	{
		BiArray<Tuple2<int, int>> m;
		
		m.CreateTail<Foo>(11, 12, 13);
		m.CreateHead(9, 10);
		
		DDUMP(m);
		
		DDUMP(((Foo&)m.Tail()).c);
	}
	
	CheckLogEtalon();
}
