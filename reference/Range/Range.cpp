#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<int> v = { 1, 4, 7, 8, 12, 3, 6, 8, 9, 3, 6 };
	               // 0  1  2  3  4   5  6  7  8  9  10

	DUMP(SubRange(v, 2, 7));
	DUMP(ViewRange(v, Vector<int>{ 1, 4, 7 }));
	DUMP(FilterRange(v, [](int c) { return c > 5; }));
	DUMP(ConstRange(1, 5));
	
	Sort(SubRange(v, 3, 4));
	DUMP(v);
}
