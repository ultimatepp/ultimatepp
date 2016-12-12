#include "Tutorial.h"

void ValueTutorial()
{
	/// .Value

	/// Value is sort of equivalent of polymorphic data types from scripting languages like Python
	/// or JavaSript. `Value` can represent values of concrete types, some types also have
	/// extended interoperability with `Value` and it is then possible to e.g. compare `Value`s
	/// containing such types against each other or serialize them for persistent storage.
	
	/// Usually, Value compatible types define typecast operator to `Value` and constructor
	/// from `Value`, so that interaction is for the most part seamless:

	Value a = 1;
	Value b = 2.34;
	Value c = GetSysDate();
	Value d = "hello";

	DUMP(a);
	DUMP(b);
	DUMP(c);
	DUMP(d);

	int x = a;
	double y = b;
	Date z = c;
	String s = d;
	
	DUMP(x);
	DUMP(y);
	DUMP(z);
	DUMP(s);

	/// As for primitive types, Value seamlessly works with `int`, `int64`, `bool` and `double`.
	/// Casting `Value` to a type that it does not contain throws an exception:
	
	try {
		s = a;
		DUMP(s); // we never get here....
	}
	catch(ValueTypeError) {
		LOG("Failed Value conversion");
	}

	/// However, conversion between related types is possible (as long as it is supported by
	/// these types):

	double i = a;
	int j = b;
	Time k = c;
	WString t = d;
	
	DUMP(i);
	DUMP(j);
	DUMP(k);
	DUMP(t);
	
	/// To determine type of value stored in `Value`, you can use `Is` method:

	DUMP(a.Is<int>());
	DUMP(a.Is<double>());
	DUMP(b.Is<double>());
	DUMP(c.Is<int>());
	DUMP(c.Is<Date>());
	DUMP(d.Is<String>());

	/// Note that Is tests for absolute type match, not for compatible types. For that reason,
	/// for widely used compatible types helper functions are defined:

	DUMP(IsNumber(a));
	DUMP(IsNumber(b));
	DUMP(IsDateTime(c));
	DUMP(IsString(d));
	
	///
}