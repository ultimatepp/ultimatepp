#include <Core/Core.h>

using namespace Upp;

template <class T>
void Check()
{
	T a;
	std::includes(a.begin(), a.end(), a.begin(), a.end());
	
	const T& b = a;
	std::includes(b.begin(), b.end(), b.begin(), b.end());
}

CONSOLE_APP_MAIN
{
	Check< Vector<int> > ();
	Check< Array<int> > ();

	Check< BiVector<int> > ();
	Check< BiArray<int> > ();

	Check< Index<int> > ();
	Check< VectorMap<int, int> > ();
	Check< ArrayMap<int, int> > ();

	Check< InVector<int> > ();
	Check< InArray<int> > ();

	Check< SortedIndex<int> > ();
	Check< SortedVectorMap<int, int> > ();
	Check< SortedArrayMap<int, int> > ();
}
