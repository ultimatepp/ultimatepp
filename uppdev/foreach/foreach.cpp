#include <Core/Core.h>

using namespace Upp;

#define foreach(v, a) for(int i_ = 0; i_ < a.GetCount(); i_++) \
						for(bool b = true; b;) \
	                     	for(v = a[i_]; b; b = false)

CONSOLE_APP_MAIN
{
	Vector<int> x;
	for(int i = 0; i < 10; i++)
		x.Add(i);
	foreach(int e, x)
		DUMP(e);
}
