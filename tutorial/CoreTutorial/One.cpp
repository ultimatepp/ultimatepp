#include "Tutorial.h"

void OneTutorial()
{
	/// .`One`

	/// `One` is a container that can store none or one element of T or derived from T. It is
	/// functionally quite similar to `std::unique_ptr`, but has some convenient features.

	struct Base {
		virtual String Get() = 0;
		virtual ~Base() {}
	};
	
	struct Derived1 : Base {
		virtual String Get() { return "Derived1"; }
	};
	
	struct Derived2 : Base {
		virtual String Get() { return "Derived2"; }
	};
	
	One<Base> s;
	
	/// `operator bool` of one returns true if it contains an element:
	
	DUMP((bool)s);
	
	///
	
	s.Create<Derived1>();
	DUMP((bool)s);
	DUMP(s->Get());

	/// You can use `Is` to check if certain type is currently stored in `One`:
	
	DUMP(s.Is<Derived1>());
	DUMP(s.Is<Base>());
	DUMP(s.Is<Derived2>());
	
	/// To get a pointer to the contained instance, use `operator~`:
	
	Base *b = ~s;
	DUMP(b->Get());

	/// Clear method removes the element from One:

	s.Clear();
	DUMP((bool)s);
	
	/// Helper function MakeOne derived from One can be used to create contained element:
	
	s = MakeOne<Derived1>();
	DUMP(s->Get());
	
	///

	auto t = pick(s);
	DUMP(t->Get());
	
	///
}
