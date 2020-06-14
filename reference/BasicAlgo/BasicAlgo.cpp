#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	DUMP(sgn(10));
	DUMP(sgn(0));
	DUMP(sgn(-7));
	
	DUMP(min(6, 2, 7, 4));
	DUMP(max(6, 2, 7, 4));
	
	DUMP(clamp(0, 1, 10));
	DUMP(clamp(1, 1, 10));
	DUMP(clamp(5, 1, 10));
	DUMP(clamp(10, 1, 10));
	DUMP(clamp(11, 1, 10));

	DUMP(findarg(3, 1, 7, 6, 3, 2));
	DUMP(findarg(5, 1, 7, 6));
	
	DUMP(decode(2, 1, "one", 2, "two", 3, "three", "?"));
	DUMP(decode(5, 1, "one", 2, "two", 3, "three", "?"));
	
	DUMP(get_i(-1, "zero", "one", "two"));
	DUMP(get_i(0, "zero", "one", "two"));
	DUMP(get_i(2, "zero", "one", "two"));
	DUMP(get_i(3, "zero", "one", "two"));

	Vector<int> data = { 10, 5, 11, 9, 2, 11 };
	//                   0   1  2   3  4  5

	DUMP(FindIndex(data, 11));
	DUMP(FindMin(data));
	DUMP(FindMin(data, 0, 4));
	DUMP(Min(data));

	Vector<int> empty;
//	DUMP(Min(empty)); // This is undefined (fails in ASSERT)
	DUMP(Min(empty, -99999));
	
	DUMP(Count(data, 11));
	DUMP(CountIf(data, [=](int c) { return c >= 5; }));

	struct {
		int mul = 1;
		void operator()(int x) { mul *= x; };
	} acc;
	Accumulate(data, acc);
	DUMP(acc.mul);
	
	DUMP(Sum(data));

	DUMP(FindMax(data));
	DUMP(FindMax(SubRange(data, 3, 2)));
	DUMP(Max(data));
//	DUMP(Max(empty)); // This is undefined (fails in ASSERT)
	DUMP(Max(empty, 99999));

	DUMP(Sum(data));
	
	Vector<String> s = { "one", "two", "three" };
	DUMP(Sum(s, ""));
	
	data = { 5, 7, 9,  9, 14, 20, 23, 50 };
	      // 0  1  2   3   4   5   6   7
	DUMP(FindLowerBound(data, 9));
	DUMP(FindUpperBound(data, 9));
	DUMP(FindBinary(data, 9));
	DUMP(FindLowerBound(data, 10));
	DUMP(FindUpperBound(data, 10));
	DUMP(FindBinary(data, 10));
}
