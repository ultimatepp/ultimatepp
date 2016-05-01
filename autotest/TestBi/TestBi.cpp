#include <Core/Core.h>

using namespace Upp;

template <class Bi>
void Test()
{
	Bi x { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	ASSERT(x.PopHead() == 1);
	ASSERT(x.PopHead() == 2);
	ASSERT(x.PopTail() == 9);
	ASSERT(x.PopHead() == 3);
	ASSERT(x.PopTail() == 8);
	ASSERT(x.PopHead() == 4);
	ASSERT(x.PopTail() == 7);
	ASSERT(x.PopTail() == 6);
	ASSERT(x.PopTail() == 5);
	ASSERT(x.GetCount() == 0);
}

template <class V>
void Test2()
{
	V x { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for(int i = 9; i >= 1; i--)
		ASSERT(x.Pop() == i);
	ASSERT(x.GetCount() == 0);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test<BiVector<int>>();

	Test2<Vector<int>>();
	
	LOG("=========== Everything OK");
}
