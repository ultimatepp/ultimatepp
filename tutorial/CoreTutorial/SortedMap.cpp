#include "Tutorial.h"

void SortedMap()
{
	/// .`SortedIndex`, `SortedVectorMap`, `SortedArrayMap`

	/// `SortedIndex` is similar to regular `Index`, but keeps its elements in sorted order
	/// (sorting predicate is a template parameter, defaults to `StdLess`). Implementation is
	/// using `InVector`, so it works fine even with very large number of elements (performance
	/// is similar to tree based `std::set`). Unlike `Index`, `SortedIndex` provides
	/// lower/upper bounds searches, so it allows range search.

	SortedIndex<int> x;
	x.Add(5);
	x.Add(3);
	x.Add(7);
	x.Add(1);
	
	DUMPC(x);
	DUMP(x.Find(3));
	DUMP(x.Find(3));
	DUMP(x.FindLowerBound(3));
	DUMP(x.FindUpperBound(6));

	/// `SortedVectorMap` and `SortedArrayMap` are then `SortedIndex` based equivalents to
	/// `VectorMap`/`ArrayMap`:
	
	SortedVectorMap<String, int> m;
	m.Add("zulu", 11);
	m.Add("frank", 12);
	m.Add("alfa", 13);
	
	DUMPM(m);
	DUMP(m.Get("zulu"));

	///
}