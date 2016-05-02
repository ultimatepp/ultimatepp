#include <Core/Core.h>

using namespace Upp;

template <class T>
void ArrayTest()
{
	T array;
	array.Add(1);
	array.Add(2);
	array.Add(3);
	DUMP(array);
}

template <class T>
void BiArrayTest()
{
	T array;
	array.AddTail(1);
	array.AddTail(2);
	array.AddTail(3);
	DUMP(array);
}

template <class T>
void MapTest()
{
	T map;
	map.Add(1, "one");
	map.Add(2, "two");
	map.Add(3, "three");
	DUMP(map);
	map.Unlink(1);
	DUMP(map);
}

template <class T>
void SortedMapTest()
{
	T map;
	map.Add(1, "one");
	map.Add(2, "two");
	map.Add(3, "three");
	DUMP(map);
}

template <class T>
void FixedMapTest()
{
	T map;
	map.Add(1, "one");
	map.Add(2, "two");
	map.Add(3, "three");
	map.Finish();
	DUMP(map);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	ArrayTest< Vector<int> > ();
	ArrayTest< Array<int> > ();
	ArrayTest< InVector<int> > ();
	ArrayTest< InArray<int> > ();
	ArrayTest< Index<int> > ();
	ArrayTest< SortedIndex<int> > ();

	BiArrayTest< BiVector<int> > ();
	BiArrayTest< BiArray<int> > ();
	
	MapTest< VectorMap<int, String> >();
	MapTest< ArrayMap<int, String> >();

	SortedMapTest< SortedVectorMap<int, String> >();
	SortedMapTest< SortedArrayMap<int, String> >();

	FixedMapTest< FixedVectorMap<int, String> >();
	FixedMapTest< FixedArrayMap<int, String> >();
	
	One<int> x;
	DUMP(x);
	x.Create() = 1;
	DUMP(x);
	
	LOG("======== OK");
}
