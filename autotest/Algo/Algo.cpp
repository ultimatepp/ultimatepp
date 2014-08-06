#include <Core/Core.h>

using namespace Upp;

#define Check(a, b) if(a != b) { DLOG(#a << "=" << a << " != " << b); NEVER(); }

CONSOLE_APP_MAIN
{
	Vector<int> x;
	x << 1 << 5 << 4 << 3 << 2 << 10;
	//   0    1    2    3    4    5
	Check(FindMin(x, 0, 2), 0);
	Check(FindMax(x, 0, 3), 1);
	Check(FindMax(x, 2, 3), 2);
	Check(FindMax(x, 2, 4), 5);
	Check(FindMin(x, 2, 4), 4);
}
