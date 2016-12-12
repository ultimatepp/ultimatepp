#include "Tutorial.h"

void FunctionTutorial()
{
	/// .Function
	
	/// U++ `Function` is quite similar to `std::function` - it is a function wrapper that can
	/// store/copy/invoke any callable target. There are two important differences.
	/// First, invoking empty `Function` is NOP, if `Function` has return type `T`, it returns
	/// `T()`. Second, `Function` allows effective chaining of callable targets using
	/// `operator<<`, if `Function` has return type, the return type of last callable appended
	/// is used.
	
	/// Usually, the callable target is C++11 lambda:
	
	Function<int (int)> fn = [](int n) { LOG("Called A"); return 3 * n; };
	
	LOG("About to call function");
	int n = fn(7);
	DUMP(n);

	/// If you chain another lambda into `Function`, all are called, but the last one's return
	/// value is used:
	
	fn << [](int n) { LOG("Called B"); return n * n; };
	LOG("About to call combined function");
	n = fn(7);
	DUMP(n);

	/// Invoking empty lambda does nothing and returns default constructed return value. This
	/// is quite useful for GUI classes, which have a lot of output events represented by
	/// `Function` which are often unassigned to any action.
	
	fn.Clear();
	LOG("About to call empty function");
	n = fn(7);
	DUMP(n);
	
	/// While using `Function` with lambda expression is the most common, you can use any
	/// target that has corresponding `operator()` defined:
	
	struct Functor {
		int operator()(int x) { LOG("Called Foo"); return x % 2; }
	};
	
	fn = Functor();
	LOG("About to call Functor");
	n = fn(7);
	DUMP(n);
	
	/// As `Function` with `void` and `bool` return types are the most frequently used, U++
	/// defines template aliases `Event`:
	
	Event<> ev = [] { LOG("Event invoked"); };
	
	ev();
	
	/// and `Gate`:
	
	Gate<int> gt = [](int x) { LOG("Gate invoked with " << x); return x < 10; };
	
	bool b = gt(9);
	DUMP(b);
	b = gt(10);
	DUMP(b);
	
	/// Using lambda to define calls to methods with more parameters can be verbose and
	/// error-prone. The issue can be simplified by using `THISFN` macro:
	
	struct Foo {
		void Test(int a, const String& b) { LOG("Foo::Test " << a << ", " << b); }
		
		typedef Foo CLASSNAME; // required for THISFN
		
		void Do() {
			Event<int, const String&> fn;
			
			fn = [=](int a, const String& b) { Test(a, b); };
			fn(1, "using lambda");
			
			fn = THISFN(Test); // this is functionally equivalent, but less verbose
			fn(2, "using THISFN");
		}
	};
	
	Foo f;
	f.Do();
	
	///
}
