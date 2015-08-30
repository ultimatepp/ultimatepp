#include <Core/Core.h>

using namespace Upp;

#ifdef CPP_11

template <class T>
void Test()
{
	DLOG("--------------------------");
	DLOG(typeid(T).name());
	T v = { 1, 2, 3, 4 };
	DDUMP(v);
	v.Insert(1, { 11, 12 });
	DDUMP(v);
	v.Append({333});
	ASSERT(AsString(v) == "[1, 11, 12, 2, 3, 4, 333]");
}

template <class T>
void Test2()
{
	DLOG("--------------------------");
	DLOG(typeid(T).name());
	T v = { 1, 2, 3, 4 };
	DDUMP(v);
	ASSERT(AsString(v) == "[1, 2, 3, 4]");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test<Vector<int>>();
	Test<Array<int>>();

	Test2<InVector<int>>();
	Test2<Index<int>>();
	Test2<ArrayIndex<int>>();
	Test2<BiVector<int>>();
	Test2<BiArray<int>>();

//	VectorTest<InVector<int>>();
//	VectorTest<InArray<
	DLOG("--------------------------");
	DLOG("Everything OK");
}

#else

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	DLOG("Not a C++11 compiler - test omitted");
}

#endif