#include <Core/Core.h>

using namespace Upp;

#define Check(a, b) if(a != b) { DLOG(#a << "=" << a << " != " << b); NEVER(); }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<int> x = { 1, 5, 4, 3, 2, 10, 3 };
	             //   0  1  2  3  4   5  6
	Check(FindMin(SubRange(x, 0, 2)), 0);
	Check(FindMax(SubRange(x, 0, 3)), 1);
	Check(FindMax(SubRange(x, 2, 3)) + 2, 2);
	Check(FindMax(SubRange(x, 2, 4)) + 2, 5);
	Check(FindMin(SubRange(x, 2, 4)) + 2, 4);
	
	Check(Max(x), 10);
	Check(Min(x), 1);
	
	Vector<int> v = FindAll(x, [](int x) { return (x & 1) == 0; });
	DUMP(v);
	ASSERT(AsString(v) == "[2, 4, 5]");
	
	Check(FindIndex(x, 3), 3);
	Check(FindIndex(x, 3, 4), 6);
	Check(FindIndex(x, 2), 4);
	
	Sort(x);
	DUMP(x);
	ASSERT(AsString(x) == "[1, 2, 3, 3, 4, 5, 10]");
	                     // 0  1  2  3  4  5  6
	Check(FindBinary(x, 1), 0);
	Check(FindBinary(x, 2), 1);
	Check(FindBinary(x, 7), -1);
	Check(FindBinary(x, 0), -1);
	Check(FindBinary(x, 11), -1);
	Check(FindBinary(x, 10), 6);
	
	Check(FindLowerBound(x, 3), 2);
	Check(FindUpperBound(x, 3), 4);
	
	Check(Count(x, 3), 2);
	Check(Count(x, 10), 1);
	Check(Count(x, 11), 0);
	Check(CountIf(x, [] (auto x) { return x % 2 == 0; }), 3);
	
	LOG("================ OK");
}
