#include <Core/Core.h>

using namespace Upp;

int RandomValue()
{
	return rand() % 10;
}

CONSOLE_APP_MAIN
{
	Vector<int> v;
	v.Add(1);
	v.Add(2);

	v.Insert(1, 10);
	DUMPC(v);
	v.Remove(0);
	DUMPC(v);
	v.Clear();
	for(int i = 0; i < 10000; i++)
		v.At(RandomValue(), 0)++;
	DUMPC(v);
	Sort(v);
	DUMPC(v);
}
