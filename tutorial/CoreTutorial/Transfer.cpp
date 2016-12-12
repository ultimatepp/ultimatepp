#include "Tutorial.h"
#include <vector>

void Transfer()
{
	/// .Transfer issues

	/// Often you need to pass content of one container to another of the same type. U++
	/// containers always support ^topic://Core/srcdoc/pick_$en-us:pick semantics^ (synonym of
	/// std::move), and, depending on type stored, also might support
	/// ^topic://Core/srcdoc/pick_$en-us:clone semantics^. When transferring the value, you
	/// have to explicitly specify which one to use:
	
	Vector<int> v{ 1, 2 };

	DUMP(v);

	Vector<int> v1 = pick(v);
	
	DUMP(v);
	DUMP(v1);
	
	/// now source `Vector` `v` is empty, as elements were 'picked' to `v1`.
	
	/// If you really need to preserve value of source (and elements support deep copy
	/// operation), you can use `clone`:
	
	v = clone(v1);
	
	DUMP(v);
	DUMP(v1);
	
	/// The requirement of explicit `clone` has the advantage of avoiding unexpected deep
	/// copies. For example:
	
	Vector<Vector<int>> x;
	x.Add() << 1 << 2 << 3;

#if 0
	for(auto i : x) { LOG(i); }
#endif

	/// results in run-time error, whereas the equivalent code with `std::vector` compiles but
	/// silently performs deep copy for each iteration:

#if 0
	std::vector<std::vector<int>> sv;
	sv.push_back({1, 2, 3});
	for(auto i : sv) // invokes std::vector<int> copy constructor
		for(auto j : i)
			DUMP(j);
#endif

	/// That said, in certain cases it is simpler to have default copy instead of explicit
	/// `clone`. You can easily achieve that using `WithDeepCopy` template:
	
	WithDeepCopy<Vector<int>> v2;
	
	v2 = v;
	
	DUMP(v);
	DUMP(v2);
	
	///
}
