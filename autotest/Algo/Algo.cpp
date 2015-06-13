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
	
	Sort(x);
	Check(FindBinaryIter(x.Begin(), x.End(), 1) - x.Begin(), 0);
	Check(FindBinaryIter(x.Begin(), x.End(), 2) - x.Begin(), 1);
	Check(FindBinaryIter(x.Begin(), x.End(), 7), NULL);
	Check(FindBinaryIter(x.Begin(), x.End(), 0), NULL);
	Check(FindBinaryIter(x.Begin(), x.End(), 11), NULL);
	Check(FindBinaryIter(x.Begin(), x.End(), 10) - x.Begin(), 5);
}
