#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Value m = ValueMap{ { 1, "one" }, { 2, "two" } };
	{
		String s;
		for(Value v : m)
			s << v;
		DUMP(s);
		ASSERT(s == "onetwo");
	}
	{
		String s;
		for(int i = 0; i < m.GetCount(); i++)
			s << m[i];
		DUMP(s);
		ASSERT(s == "onetwo");
	}
}
