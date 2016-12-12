#include "Tutorial.h"

void Sorting()
{
	/// .Sorting
	
	/// Unsurprisingly, `Sort` function sorts a range. You can specify sorting predicate,
	/// default is `operator<`:
	
	Vector<String> x { "1", "2", "10" };
	
	Sort(x);
	
	DUMP(x);
	
	///
	
	Sort(x, [](const String& a, const String& b) { return atoi(a) < atoi(b); });
	
	DUMP(x);
	

	/// `IndexSort` is sort variant that is able to sort two ranges (like `Vector` or `Array`)
	/// of the same size, based on values in the first range:

	Vector<int> a { 5, 10, 2, 9, 7, 3 };
	Vector<String> b { "five", "ten", "two", "nine", "seven", "three" };
	
	IndexSort(a, b);
	
	DUMP(a);
	DUMP(b);

	///

	IndexSort(b, a);
	
	DUMP(a);
	DUMP(b);
	
	/// There are also `IndexSort2` and `IndexSort3` variants that sort 2 or 3 dependent ranges.

	/// Sometimes, instead of sorting items in the range, it is useful to know the order of
	/// items as sorted, using `GetSortOrder`:

	Vector<int> o = GetSortOrder(a);
	
	DUMP(o);

	/// Normal `Sort` is not stable - equal items can appear in sorted range in random order.
	/// If maintaining original order of equal items is important, use `StableSort` variant
	/// (with performance penalty):
	
	Vector<Point> t { Point(10, 10), Point(7, 1), Point(7, 2), Point(7, 3), Point(1, 0) };
	StableSort(t, [](const Point& a, const Point& b) { return a.x < b.x; });
	
	DUMP(t);
	
	/// All sorting algorithms have they 'Stable' variant, so there is `StableIndexSort`,
	/// `GetStableSortOrder` etc...
}
