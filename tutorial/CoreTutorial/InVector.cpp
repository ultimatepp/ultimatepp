#include "Tutorial.h"

void InVectorTutorial()
{
	/// .`InVector`, `InArray`

	/// `InVector` and `InArray` are container types quite similar to `Vector`/`Array`, but
	/// they trade the speed of `operator[]` with the ability to insert or remove elements at
	/// any position quickly. You can expect `operator[]` to be about 10 times slower than in
	/// Vector (but that is still quite fast), while `Insert` at any position scales well up to
	/// hundreds of megabytes of data (e.g. `InVector` containing 100M of String elements is
	/// handled without problems).

	InVector<int> v;
	for(int i = 0; i < 1000000; i++)
		v.Add(i);
	v.Insert(0, -1); // This is fast

	/// While the interface of `InVector`/`InArray` is almost identical to `Vector`/`Array`,
	/// `InVector`/`InArray` in addition implements `FindLowerBound`/`FindUpperBound` methods -
	/// while normal generic range algorithms work, it is possible to provide
	/// `InVector`/`InArray` specific optimizations that basically match the performace of
	/// `Find*Bound` on simple `Vector`.

	DUMP(v.FindLowerBound(55));
	
	///
}
