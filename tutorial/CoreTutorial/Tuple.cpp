#include "Tutorial.h"

void TupleTutorial()
{
	/// . Tuples

	/// Template class `Tuple` allows combining 2-4 values with
	/// different types. These are principally similar to `std::tuple`, with some advantages.
	/// Unlike `std::tuple`, individual elements are directly accessible as member variables
	/// `a`..`d`, `Tuple` supports persistent storage patterns (`Serialize`, `Jsonize`, `Xmlize`), hash
	/// code (`GetHashValue`), conversion to `String` and Value conversions.
	
	/// To create a `Tuple` value, you can use the `MakeTuple` function.

	Tuple<int, String, String> x = MakeTuple(12, "hello", "world");

	/// Individual values are accessible as members `a` .. `d`:

	DUMP(x.a);
	DUMP(x.b);
	DUMP(x.c);
	
	/// Or using `Get`:
	
	DUMP(x.Get<1>());
	DUMP(x.Get<int>());
	
	/// As long as all individual types have conversion to `String` (`AsString`), the tuple also
	/// has such conversion and thus can e.g. be easily logged:

	DUMP(x);

	/// As long as individual types have defined `GetHashValue`, so does `Tuple`:

	DUMP(GetHashValue(x));

	/// As long as individual types have defined `operator==`, `Tuple` has defined `operator==`
	/// and `operator!=`:

	Tuple<int, String, String> y = x;
	DUMP(x == y);
	DUMP(x != y);
	y.a++;
	DUMP(x == y);
	DUMP(x != y);

	/// As long as all individual types have defined `SgnCompare`,
	/// Tuple has SgnCompare, Compare method and operators <, <=, >, >=:

	DUMP(x.Compare(y));
	DUMP(SgnCompare(x, y));
	DUMP(x < y);
	
	/// GetCount returns the width of `Tuple`:
	
	DUMP(x.GetCount());
	
	/// Elements that are directly convertible with `Value` can be 'Get'/'Set':
	
	for(int i = 0; i < x.GetCount(); i++)
		DUMP(x.Get(i));
	
	///
	
	x.Set(1, "Hi");
	DUMP(x);
	
	/// As long as all individual types are convertible with `Value`, you can convert Tuple to
	/// `ValueArray` and back:
	
	ValueArray va = x.GetArray();
	DUMP(va);

	va.Set(2, "Joe");
	x.SetArray(va);
	
	/// It is OK to assign `Tuple` to `Tuple` with different individual types, as long as types
	/// are directly convertible:
	
	Tuple<double, String, String> d = x;
	DUMP(d);
	
	/// Tie can be used to assign tuple to l-values:
	
	int i;
	String s1, s2;
	
	Tie(i, s1, s2) = x;
	
	DUMP(i);
	DUMP(s1);
	DUMP(s2);

	/// U++ Tuples are carefully designed as POD type, which allows POD arrays to be intialized
	/// with classic C style:
	
	static Tuple2<int, const char *> map[] = {
		{ 1, "one" },
		{ 2, "one" },
		{ 3, "one" },
	};
	

	/// Simple FindTuple template function is provided to search for tuple based on the first
	/// value (`a`) (linear O(n) search):
	
	DUMP(FindTuple(map, __countof(map), 3)->b);
	
	///
}