#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN{
	StdLogSetup(LOG_FILE|LOG_COUT);

	VectorMap<int, String> m;
	m(1, "one")(2, "two");
	DDUMPM(m);
	ASSERT(m.Get(1) == "one");
	ASSERT(m.Get(2) == "two");
	
	{
		SortedVectorMap<int, String> m;
		m(2, "two")(1, "one");
		DDUMPM(m);
		ASSERT(m.Get(1) == "one");
		ASSERT(m.Get(2) == "two");
	}
	{
		SortedArrayMap<int, String> m;
		m(1, "one")(2, "two");
		DDUMPM(m);
		ASSERT(m.Get(1) == "one");
		ASSERT(m.Get(2) == "two");
	}
	{
		ValueMap m;
		m(1, "one")(2, "two");
		DDUMP(m);
		ASSERT(m[1] == "one");
		ASSERT(m[2] == "two");
	}
}
