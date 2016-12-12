#include "Tutorial.h"

void Vector1()
{
	/// .`Vector` basics
	
	/// `Vector` is the basic container of U++. It is the random access container similar to
	/// `std::vector` with one important performance related difference: There are rules for
	/// elements of `Vector` that allow its implementation to move elements in memory using
	/// plain `memcpy`/`memmove` ("Moveable" concept).
	
	/// Anyway, for now let us start with simple `Vector` of `int`s:
	
		Vector<int> v;
	
	/// You can add elements to the Vector as parameters of the Add method

		v.Add(1);
		v.Add(2);
		
		DUMP(v);
	
	/// Alternative and very important possibility for U++ containers is 'in-place creation'.
	/// In this case, parameter-less Add returns a reference to a new element in `Vector`:

		v.Add() = 3;
		
		DUMP(v);
		
	/// You can also use `operator<<`
	
		v << 4 << 5;

		DUMP(v);
	
	/// `Vector` also supports initializer lists:
	
		v.Append({ 6, 7 });

		DUMP(v);

	/// To iterate `Vector` you can use indices:
	
		for(int i = 0; i < v.GetCount(); i++)
			LOG(v[i]);
	
	/// begin/end interface:

		for(auto q = v.begin(), e = v.end(); q != e; q++)
			LOG(*q);
	
	/// C++11 range-for syntax:
	
		for(const auto& q : v)
			LOG(q);
	
	///
}
