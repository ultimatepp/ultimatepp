#include <Core/Core.h>

using namespace Upp;

template <class T>
void CompareArray()
{
	T a;
	a.Add(1);
	a.Add(2);
	T b = clone(a);
	ASSERT(a == b);
	b.At(1) = 3;
	ASSERT(a != b);
	ASSERT(b > a);
	
	b = clone(a);
	b.Add(10);
	ASSERT(a != b);
	ASSERT(b > a);
}

template <class T>
void CompareBiArray()
{
	T a;
	a.AddTail(1);
	a.AddTail(2);
	T b = clone(a);
	ASSERT(a == b);
	b[1] = 3;
	ASSERT(a != b);
	ASSERT(b > a);
	
	b = clone(a);
	b.AddTail(10);
	ASSERT(a != b);
	ASSERT(b > a);
}

template <class T>
void CompareIndex()
{
	T a;
	a.Add(1);
	a.Add(2);
	T b = clone(a);
	ASSERT(a == b);
	b.Add(3);
	ASSERT(a != b);
	ASSERT(b > a);
	b.Clear();
	b.Add(1);
	b.Add(3);
	ASSERT(a != b);
	ASSERT(b > a);
}

template <class T>
void CompareMap()
{
	T a;
	a.Add(1, 2);
	a.Add(3, 4);
	
	T b = clone(a);
	
	ASSERT(a == b);
	b.Add(4, 4);
	ASSERT(a != b);
	ASSERT(b > a);
	
	b.Clear();
	b.Add(2, 2);
	b.Add(3, 4);
	ASSERT(b > a);
	
	b.Clear();
	b.Add(1, 2);
	b.Add(3, 5);
	ASSERT(b > a);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	CompareArray< Vector<int> >();
	CompareArray< Array<int> >();
	CompareArray< InVector<int> >();
	CompareArray< InArray<int> >();

	CompareBiArray< BiVector<int> >();
	CompareBiArray< BiArray<int> >();

	CompareIndex< Index<int> >();
	CompareIndex< SortedIndex<int> >();
	
	CompareMap<VectorMap<int, int> >();
	CompareMap<ArrayMap<int, int> >();

	CompareMap<SortedVectorMap<int, int> >();
	CompareMap<SortedArrayMap<int, int> >();
	
	LOG("===== OK");
}
