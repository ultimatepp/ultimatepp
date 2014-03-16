#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	SortedIndex<int> x;
	x.Add(5);
	x.Add(3);
	x.Add(7);
	x.Add(1);
	
	DUMPC(x);
	DUMP(x.Find(3));
	DUMP(x.FindLowerBound(3));
	DUMP(x.FindUpperBound(6));
	
	SortedVectorMap<String, int> m;
	m.Add("zulu", 11);
	m.Add("frank", 12);
	m.Add("alfa", 13);
	
	DUMPM(m);
	DUMP(m.Get("zulu"));
}
