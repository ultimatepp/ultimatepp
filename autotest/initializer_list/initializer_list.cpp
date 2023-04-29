#include <Core/Core.h>

using namespace Upp;

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
	DDUMP(v);
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

template <class T>
void TestMap()
{
	DLOG("--------------------------");
	DLOG(typeid(T).name());
	T m{ {"1", 1}, {"2", 2} };
	DDUMP(AsString(m));
	ASSERT(AsString(m) == "{1: 1, 2: 2}");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Test<Vector<int>>();
	Test<Array<int>>();

	Test2<InVector<int>>();
	Test2<Index<int>>();
	Test2<BiVector<int>>();
	Test2<BiArray<int>>();
	
	TestMap<VectorMap<String, int>>();
	TestMap<ArrayMap<String, int>>();
	TestMap<SortedVectorMap<String, int>>();
	TestMap<SortedArrayMap<String, int>>();
	
	DLOG("--------------------------");
	Buffer<String> x = { "one", "two", "three" };
	for(int i = 0; i < 3; i++)
		DDUMP(x[i]);
	ASSERT(x[0] == "one");
	ASSERT(x[1] == "two");
	ASSERT(x[2] == "three");

	DLOG("--------------------------");
	ValueArray va = { "one", 2, Date(2015, 3, 3) };
	DDUMP(va);
	ASSERT(AsString(va) == "[one, 2, 03/03/2015]");

	DLOG("--------------------------");
	ValueMap vm = { { "one", 1 }, { 2, "two" } };
	DDUMP(vm);
	ASSERT(AsString(vm) == "{ one: 1, 2: two }");
	
//	VectorTest<InVector<int>>();
//	VectorTest<InArray<
	DLOG("--------------------------");
	DLOG("Everything OK");
}
