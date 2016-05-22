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
	for(auto q = v.begin(), e = v.end(); q != e; q++)
		LOG(*q);
	LOG("..and now range-for..");
	for(const auto& q : v)
		LOG(q);
}
