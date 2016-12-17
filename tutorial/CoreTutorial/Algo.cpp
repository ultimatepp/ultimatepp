#include "Tutorial.h"

void Algo()
{
	/// .Algorithms
	
	/// In principle, is is possible to apply C++ standard library algorithms on U++ containers
	/// or ranges.
	
	/// U++ algorithms are tuned for U++ approach - they work on ranges and they prefer
	/// indices. Sometimes, U++ algorithm will perform faster with U++ types than standard
	/// library algorithm.
	
	/// `FindIndex` performs linear search to find element with given value and returns its
	/// index or -1 if not found:
	
	Vector<int> data { 5, 3, 7, 9, 3, 4, 2 };
	

	DUMP(FindIndex(data, 3));
	DUMP(FindIndex(data, 6));

	/// `SubRange` can be used to apply algorithm on subrange of container:
	
	DUMP(FindIndex(SubRange(data, 2, data.GetCount() - 2), 3));
	
	/// `FindMin` and `FindMax` return the index of minimal / maximal element:


	DUMP(FindMin(data));
	DUMP(FindMax(data));

	/// `Min` and `Max` return the %value% of minimal / maximal element:
	
	DUMP(Min(data));
	DUMP(Max(data));
	
	/// If the range is empty, `Min` and `Max` are undefined (ASSERT fails in debug mode),
	/// unless the value is specified as second parameter to be used in this case:

	Vector<int> empty;
//	DUMP(Min(empty)); // This is undefined (fails in ASSERT)
	DUMP(Min(empty, -99999));
	
	/// `Count` returns the number of elements with specified value, `CountIf` the number of
	/// elements that satisfy predicate:

	DUMP(Count(data, 11));
	DUMP(CountIf(data, [=](int c) { return c >= 5; }));

	/// `Sum` return the sum of all elements in range:
	
	DUMP(Sum(data));

	/// Sorted containers can be searched with bisection. U++ provides usual upper / lower
	/// bound algorithms. `FindBinary` returns the index of element with given value or -1 if
	/// not found:
	
	data = { 5, 7, 9,  9, 14, 20, 23, 50 };
	      // 0  1  2   3   4   5   6   7
	DUMP(FindLowerBound(data, 9));
	DUMP(FindUpperBound(data, 9));
	DUMP(FindBinary(data, 9));
	DUMP(FindLowerBound(data, 10));
	DUMP(FindUpperBound(data, 10));
	DUMP(FindBinary(data, 10));
	
	///
}