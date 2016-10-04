#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String& s = Single<String>();
	ASSERT(IsNull(s));
		
	Point& p = Single<Point>(10, 20);
	ASSERT(p.x == 10);
	ASSERT(p.y == 20);
	
	{
		One<String> o;
		ASSERT(o.Create<String>("hello!") == "hello!");
		
		One<WString> wo;
		ASSERT(IsNull(wo.Create()));
	}

	{
		Any o;
		ASSERT(o.Create<String>("hello!") == "hello!");
		
		Any wo;
		ASSERT(IsNull(wo.Create<WString>()));
	}
	
	{
		Vector<Point> p;
		p.Create(1, 2);
		ASSERT(p.Top().x == 1);
		ASSERT(p.Top().y == 2);
		p.Create();
	}

	{
		Array<Point> p;
		p.Create<Point>(1, 2);
		ASSERT(p.Top().x == 1);
		ASSERT(p.Top().y == 2);
		p.Create<Point>();
	}
	
	{
		ArrayMap<String, String> p;
		p.Create<String>("k1");
		ASSERT(IsNull(p.Top()));
		p.Create<String>("k2", "Hello!");
		ASSERT(p.Top() == "Hello!");
	}

	{
		FixedArrayMap<String, String> p;
		p.Create<String>("k1");
		ASSERT(IsNull(p[0]));
		p.Create<String>("k2", "Hello!");
		ASSERT(p[1] == "Hello!");
	}

	{
		InArray<String> p;
		p.InsertCreate<String>(0, "A1");
		ASSERT(p[0] == "A1");
		p.Create<String>("Hello!");
		ASSERT(p[1] == "Hello!");
	}

	{
		SortedArrayMap<String, String> p;
		p.Create<String>("k1");
		ASSERT(IsNull(p.Get("k1")));
		p.Create<String>("k2", "Hello!");
		ASSERT(p.Get("k2") == "Hello!");
	}


	LOG("-----------------  OK");
}
