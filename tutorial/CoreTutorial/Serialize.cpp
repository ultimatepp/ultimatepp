#include "Tutorial.h"

void Serialize()
{
	/// .Binary serialization
	
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
	
	///
	
	ss2.Seek(0);
	ss2.LoadThrowing();
	try {
		ss2 % x2 % h2 % x2;
	}
	catch(LoadingError) {
		LOG("Deserialization has failed");
	}
	
	///
	
	struct MyFoo {
		int    number;
		Color  color;
		
		void Serialize(Stream& s) {
			int version = 0;
			s / version; // allow backward compatibility in the future
			s % number % color;
		}
	};
	
	MyFoo foo;
	foo.number = 321;
	foo.color = Blue();
	
	String data = StoreAsString(foo);
	MyFoo foo2;
	LoadFromString(foo2, data);
	DUMP(foo2.number);
	DUMP(foo2.color);
	
	///

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
	LoadFromString(foo3, h);
	DUMP(foo3.number);
	DUMP(foo3.color);
}