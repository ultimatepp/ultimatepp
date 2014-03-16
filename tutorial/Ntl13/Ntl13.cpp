#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	InVector<int> v;
	for(int i = 0; i < 1000000; i++)
		v.Add(i);
	v.Insert(0, -1); // This is fast
	DUMP(v[0]);
	DUMP(v[1]);
	DUMP(v.FindLowerBound(55));
}
