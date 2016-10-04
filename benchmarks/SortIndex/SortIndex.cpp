#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Index<String> n;
	Vector<String> v;
	
	for(int i = 0; i < 10000000; i++) {
		String s = AsString(Random());
		n.Add(s);
		v.Add(s);
	}
	
	{
		RTIMING("Sort");
		Sort(v);
	}

	{
		RTIMING("IndexSort");
		SortIndex(n);
	}
}
