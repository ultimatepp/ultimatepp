#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<int> v;
	v.Add(1);
	v.Add(2);
	DUMPC(v);

	Vector<int> v1 = pick(v);
	DUMPC(v1);
//	DUMPC(v); -> v is picked, this would crash with runtime error

	v = clone(v1);
	DUMPC(v1);
	DUMPC(v);

	Vector<int> v2 = clone(v);
	DUMPC(v2);
	DUMPC(v);
}
