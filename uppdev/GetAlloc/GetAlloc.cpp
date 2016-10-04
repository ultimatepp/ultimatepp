#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<int> v;
	for(int i = 0; i < 1000; i++) {
		v.At(i);
		RLOG(v.GetCount() << ' ' << v.GetAlloc());
	}
}
