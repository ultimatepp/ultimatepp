#include "Tutorial.h"

void ComparableTutorial()
{
	/// .SgnCompare and CombineCompare
	
	/// Traditional approach of C language of representing comparison results was 3-state:
	/// comparing a and b results in negative value (if a < b), zero (if a == b) or positive
	/// value (a > b). In C++ standard library, comparisons are usually represented with `bool`
	/// predicates.
	
	/// However, with `bool` predicate it becomes somewhat more difficult to provide
	/// comparisons for composite types:
	
	struct Foo {
		String a;
		int    b;
		int    c;
		
		// we want to order Foo instances by a first, then b, then c
		
		bool operator<(const Foo& x) const {
			return a < x.a ? true
			               : a == x.a ? b < x.b ? true
			                          : b == x.b ? false
			                                     : c < x.c
			               : false;
		}
	};
	
	/// U++ provides standard function `SgnCompare`, which returns negative value/zero/positive
	/// in "C style":
	
	int a = 1;
	int b = 2;
	
	DUMP(SgnCompare(a, b));
	DUMP(SgnCompare(b, a));
	DUMP(SgnCompare(a, a));
	
	/// Default implementation of `SgnCompare` calls `Compare` method of value:
	
	struct MyClass {
		int val;
		
		int Compare(const MyClass& x) const { return SgnCompare(val, x.val); }
	};
	
	/// `SgnCompare` is now defined for `MyClass`:
	
	MyClass u, v;
	u.val = 1;
	v.val = 2;
	
	DUMP(SgnCompare(u, v));
	DUMP(SgnCompare(v, u));
	DUMP(SgnCompare(v, v));

	/// Now getting back to `Foo`, with `SgnCompare` `operator<` becomes much less difficult:
	
	struct Foo2 {
		String a;
		int    b;
		int    c;
		
		bool operator<(const Foo2& x) const {
			int q = SgnCompare(a, x.a);
			if(q) return q < 0;
			q = SgnCompare(b, x.b);
			if(q) return q < 0;
			q = SgnCompare(c, x.c);
			return q < 0;
		}
	};
	
	/// Alternatively, it is possible to define just `Compare` method and use `Comparable`
	/// ^https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern:CRTP idiom^ to
	/// define all relation operators:

	struct Foo3 : Comparable<Foo3> {
		String a;
		int    b;
		int    c;
		
		int Compare(const Foo3& x) const {
			int q = SgnCompare(a, x.a);
			if(q) return q;
			q = SgnCompare(b, x.b);
			if(q) return q;
			return SgnCompare(c, x.c);
		}
	};
	
	Foo3 m, n;
	m.a = "A";
	m.b = 1;
	m.c = 2;
	n.a = "A";
	n.b = 1;
	n.c = 3;
	
	DUMP(m < n);
	DUMP(m == n);
	DUMP(m != n);
	DUMP(SgnCompare(m, n));
	
	/// While the content of `Compare` method is trivial, it can be further simplified using
	/// `CombineCompare` helper class:

	struct Foo4 : Comparable<Foo4> {
		String a;
		int    b;
		int    c;
		
		int Compare(const Foo4& x) const {
			return CombineCompare(a, x.a)(b, x.b)(c, x.c);
		}
	};
	
	Foo4 o, p;
	o.a = "A";
	o.b = 1;
	o.c = 2;
	p.a = "A";
	p.b = 1;
	p.c = 3;
	
	DUMP(o < p);
	DUMP(o == p);
	DUMP(o != p);
	DUMP(SgnCompare(o, p));
	
	///
}
