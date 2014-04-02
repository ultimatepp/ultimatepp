#include <Core/Core.h>

using namespace Upp;

template <class T>
void TestCompareT(T a, T b)
{
	LOG("==== TestCompare(" << a << " " << b << "): " << a.Compare(b));

	ASSERT(sgn(a.Compare(b)) == -1);
	ASSERT(sgn(b.Compare(a)) == 1);
	ASSERT(a.Compare(a) == 0);
	ASSERT(b.Compare(b) == 0);
	
	ASSERT(a > Null);
	ASSERT(b > Null);
	
	ASSERT(a < b);
	ASSERT(a <= a);
	ASSERT(b <= b);
	ASSERT(a <= b);

	ASSERT(b > a);
	ASSERT(b >= a);
	ASSERT(a >= a);
	ASSERT(b >= b);
	
	ASSERT(a != b);
	ASSERT(a == a);
	ASSERT(b == b);
}

void TestCompare(Value a, Value b)
{
	TestCompareT<Value>(a, b);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	TestCompare("ahoj", "ahoj1");
	TestCompare(WString("ahoj"), WString("ahoj1"));
	TestCompare(1, 2);
	TestCompare(1.1, 1.2);
	TestCompare(false, true);
	TestCompare((int64)1, (int64)2);
	TestCompare(GetSysTime(), GetSysTime() + 1);
	TestCompare(GetSysDate(), GetSysDate() + 1);
	
	ValueArray va1; va1 << 1 << 2 << 3;
	ValueArray va2; va2 << 1 << 2 << 4;
	
	TestCompareT(va1, va2);
	TestCompare(va1, va2);
	va2 = va1;
	va2 << 0;
	TestCompareT(va1, va2);
	TestCompare(va1, va2);
	
	ValueMap m1; m1("a", 1)("b", 2);
	ValueMap m2; m2("a", 1)("c", 2);
	
	TestCompareT(m1, m2);

	Value a = m1;
	Value b = m2;
	a.Compare(b);

	TestCompare(m1, m2);
	
	m2 = m1;
	m2("b", 2);

	TestCompareT(m1, m2);
	TestCompare(m1, m2);
	
	TestCompare((bool)false, 123);
	TestCompare((bool)false, (int64)123);
	TestCompare((bool)false, (double)123);
	TestCompare((bool)false, (bool)true);

	TestCompare(12, 123);
	TestCompare(12, (int64)123);
	TestCompare(12, (double)123);
	TestCompare(0, (bool)true);

	TestCompare((int64)12, 123);
	TestCompare((int64)12, (int64)123);
	TestCompare((int64)12, (double)123);
	TestCompare((int64)0, (bool)true);

	TestCompare((double)12, 123);
	TestCompare((double)12, (int64)123);
	TestCompare((double)12, (double)123);
	TestCompare((double)0, (bool)true);
	
	LOG("===== Everything is OK");
}
