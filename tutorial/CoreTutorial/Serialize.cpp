#include "Tutorial.h"

void Serialize()
{
	/// .Binary serialization
	
	/// Serialization is a mechanism that converts structured data to/from binary stream. In
	/// U++, loading and storing of data is performed by single code, in most cases represented
	/// by method `Serialize`. Serialization is performed directly with basic `Stream`. To this
	/// end, `Stream` features a single boolean representing the direction of serialization
	/// process. The direction can be checked using `IsLoading` and `IsStoring` methods and
	/// changed with `SetStoring` and `SetLoading` methods. Direction is usually set properly
	/// by derived classes (e.g. FileOut sets it to storing, FileIn to loading).
	///
	/// Shortcut to calling `Serialize` method is `operator%`, which is templated overload that
	/// calls `Serialize` for given variable (primitive types have direct overload in `Stream` class):
	
	StringStream ss;
	
	int x = 123;
	Color h = White();
	
	ss % x % h;
	
	StringStream ss2(ss.GetResult());

	int x2;
	Color h2;
	
	ss2 % x2 % h2;
	
	DUMP(x2);
	DUMP(h2);
	
	/// When serialization fails to load the data (e.g. because of wrong structure or not
	/// enough data in the stream), `Stream::LoadError` is invoked, which can trigger the
	/// exception if the stream is `LoadThrowing`:
	
	ss2.Seek(0);
	ss2.LoadThrowing();
	try {
		ss2 % x2 % h2 % x2;
	}
	catch(LoadingError) {
		LOG("Deserialization has failed");
	}
	
	/// Examples so far serve mostly like basic demonstration of serialization. In practice,
	/// the implementation is usually represented by `Serialize` method of class that is to be
	/// compatible with this concept. To that end, it is a good idea to provide means for
	/// future expansion of such class:
	
	
	struct MyFoo {
		int    number;
		Color  color;
		
		void Serialize(Stream& s) {
			int version = 0;
			s / version; // allow backward compatibility in the future
			s.Magic(31415); // put magic number into the stream to check for invalid data
			s % number % color;
		}
	};
	
	MyFoo foo;
	foo.number = 321;
	foo.color = Blue();
	
	/// `StoreAsFile`, `StoreAsString`, `LoadFromFile` and `LoadFromString` are convenience
	/// functions that simplify storing / loading objects to / from the most common forms of
	/// storage:
	
	String data = StoreAsString(foo);
	MyFoo foo2;
	LoadFromString(foo2, data);
	DUMP(foo2.number);
	DUMP(foo2.color);
	
	/// Now if `MyFoo` was to be extended to `MyFoo2` and we wanted to maintain the ability to
	/// load it from binary data stored by original `MyFoo`, we can branch on previously stored
	/// `version`:

	struct MyFoo2 {
		int    number;
		Color  color;
		String text;
		
		void Serialize(Stream& s) {
			int version = 1;
			s / version;
			s % number % color;
			if(version >= 1)
				s % text;
		}
	};
	MyFoo2 foo3;
	LoadFromString(foo3, data);
	DUMP(foo3.number);
	DUMP(foo3.color);
	
	/// Note: `operator/` is Stream method with several overloads optimized for small value -
	/// in this case `int` is stored as single byte if possible (and as 5 bytes if not).
	
	///
}
