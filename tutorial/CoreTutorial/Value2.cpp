#include "Tutorial.h"

struct Foo : ValueType<Foo, 10010> {
	int x;
	
	Foo(const Nuller&)                  { x = Null; }
	Foo(int x) : x(x) {}
	Foo() {}

	// We provide these methods to allow automatic conversion of Foo to/from Value
	operator Value() const              { return RichToValue(*this); }
	Foo(const Value& v)                 { *this = v.Get<Foo>(); }

	String ToString() const             { return AsString(x); }
	unsigned GetHashValue() const       { return x; }
	void Serialize(Stream& s)           { s % x; }
	bool operator==(const Foo& b) const { return x == b.x; }
	bool IsNullInstance() const         { return IsNull(x); }
	int  Compare(const Foo& b) const    { return SgnCompare(x, b.x); }
	// This type does not define XML nor Json serialization
};

INITBLOCK { // This has to be at file level scope
	Value::Register<Foo>(); // need to register value type integer id to allow serialization
}

void Value2Tutorial()
{
	/// .Client types and `Value`, `RawValue`, `RichValue`

	/// There are two Value compatibility levels. The simple one, `RawValue`, has little
	/// requirements for the type used - only copy constructor and assignment operator are
	/// required (and there are even forms of `RawValue` that work for types missing these):

	struct RawFoo {
		String x;
		// default copy constructor and assignment operator are provided by compiler
	};

	/// To convert such type to `Value`, use `RawToValue`:

	RawFoo h;
	h.x = "hello";
	Value q = RawToValue(h);

	DUMP(q.Is<RawFoo>());
	
	/// To convert it back, us 'To' templated member function of `Value`, it returns a constant
	/// reference to the value:
	
	DUMP(q.To<RawFoo>().x);

	/// `RichValue` level `Value`s provide more operations for `Value` - equality test,
	/// `IsNull` test, hashing, conversion to text, serialization (possibly to XML and Json),
	/// comparison. In order to make serialization work, type must also have assigned an
	/// integer id (client types should use ids in range 10000..20000). Type can provide the
	/// support for these operations via template function specializations or (perhaps more
	/// convenient) using defined methods and inheriting from `ValueType` base class template:

#if 0
	struct Foo : ValueType<Foo, 10010> {
		int x;
		
		Foo(const Nuller&)                  { x = Null; }
		Foo(int x) : x(x) {}
		Foo() {}

		// We provide these methods to allow automatic conversion of Foo to/from Value
		operator Value() const              { return RichToValue(*this); }
		Foo(const Value& v)                 { *this = v.Get<Foo>(); }

		String ToString() const             { return AsString(x); }
		unsigned GetHashValue() const       { return x; }
		void Serialize(Stream& s)           { s % x; }
		bool operator==(const Foo& b) const { return x == b.x; }
		bool IsNullInstance() const         { return IsNull(x); }
		int  Compare(const Foo& b) const    { return SgnCompare(x, b.x); }
		// This type does not define XML nor Json serialization
	};

	INITBLOCK { // This has to be at file level scope
		Value::Register<Foo>(); // need to register value type integer id to allow serialization
	}
#endif

	Value a = Foo(54321); // uses Foo::operator Value
	Value b = Foo(54321);
	Value c = Foo(600);

	DUMP(a); // uses Foo::ToString
	DUMP(a == b); // uses Foo::operator==
	DUMP(a == c);
	DUMP(c < a); // uses Foo::Compare

	DUMP(IsNull(a)); // uses Foo::IsNullInstance
	
	Foo foo = c; // Uses Foo::Foo(const Value&)
	DUMP(foo);

	///

	String s = StoreAsString(a); // Uses Foo::Serialize

	Value loaded;
	// Using registered (Value::Registered) integer id creates the correct type, then uses
	// Foo::Serialize to load the data from the stream
	LoadFromString(loaded, s);
	
	DUMP(loaded);
                         
    ///
};
