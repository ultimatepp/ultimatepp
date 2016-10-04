#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Rect r(1, 2, 3, 4);
	Value v = r;
	Value vv;
	DUMP(v);
	LoadFromString(vv, StoreAsString(v));
	DUMP(vv);
}
