#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<int> v;
	v.Add(1);
	v.Add(2);
	v.Add(3);
	for(int i = 0; i < v.GetCount(); i++)
		LOG(v[i]);
	LOG("..and now iterators..");
	for(Vector<int>::Iterator q = v.Begin(), e = v.End(); q != e; q++)
		LOG(*q);
}
