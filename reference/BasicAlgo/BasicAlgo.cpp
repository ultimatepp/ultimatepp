#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	Vector<int> data, empty;
	data << 10 << 5 << 11 << 9 << 2;

	DUMP(Sum(data));

	DUMP(FindMin(data, 0, data.GetCount() - 1));
	DUMP(FindMin(data));
	DUMP(Min(data));
//	DUMP(Min(empty)); // This is undefined (fails in ASSERT)
	DUMP(Min(empty, -99999));

	DUMP(Sum(data));
	DUMP(FindMax(data, 3, 2));
	DUMP(FindMax(data));
	DUMP(Max(data));
//	DUMP(Max(empty)); // This is undefined (fails in ASSERT)
	DUMP(Max(empty, 99999));
	
	Vector<String> s = Split("one;two;three", ';');
	DUMP(Sum(s, ""));
	
	DUMP(sgn(10));
	DUMP(sgn(0));
	DUMP(sgn(-7));
	
	DUMP(findarg(3, 1, 7, 6, 3, 2));
	DUMP(findarg(5, 1, 7, 6));
	
	DUMP(decode(2, 1, "one", 2, "two", 3, "three", "?"));
	DUMP(decode(5, 1, "one", 2, "two", 3, "three", "?"));
}
