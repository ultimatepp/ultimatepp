#include "Tutorial.h"

void CoAlgoTutorial()
{
	/// .Parallel algorithms
	
	/// U++ provides a parallel versions of algorithms where it makes sense. The naming scheme
	/// is 'Co' prefix before the name of algorithm designates the parallel version.
	
	/// So the parallel version of e.g. `FindIndex` is `CoFindIndex`, for `Sort` it is `CoSort`:
	
	Vector<String> x{ "zero", "one", "two", "three", "four", "five" };
	
	DUMP(FindIndex(x, "two"));
	DUMP(CoFindIndex(x, "two"));
	
	CoSort(x);
	DUMP(x);
	
	/// Caution should be exercised when using these algorithms - for small datasets, they are
	/// almost certainly slower than single-threaded versions.
}