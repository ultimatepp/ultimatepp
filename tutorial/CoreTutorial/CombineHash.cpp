#include "Tutorial.h"

void CombineHashTutorial()
{
	/// .CombineHash

	/// To simplify providing of high quality hash codes for composite types, U++ provides
	/// `CombineHash` utility class. This class uses `GetHashValue` function to gather hash
	/// codes of all values and combines them to provide final hash value for composite type:

	struct Foo {
		String a;
		int    b;
		
		unsigned GetHashValue() const { return CombineHash(a, b); }
	};

	/// Note that `GetHashValue` is defined as function template that calls `GetHashValue`
	/// method of its argument, therefore defining `GetHashValue` method defines `GetHashValue`
	/// function too:

	Foo x;
	x.a = "world";
	x.b = 22;

	DUMP(GetHashValue(x));
	
	///

	x.a << '!';

	DUMP(GetHashValue(x));
	
	///
}
