#include <Core/Core.h>

using namespace Upp;

struct Foo : AssignValueTypeNo<Foo, 10010> {
	int x;
	
	Foo(int x) : x(x) {}
	Foo() {}

	String ToString() const             { return AsString(x); }
	unsigned GetHashValue() const       { return x; }
	void Serialize(Stream& s)           { s % x; }
	bool operator==(const Foo& b) const { return x == b.x; }
	bool IsNullInstance() const         { return IsNull(x); }
	
	operator Value()    { return RichToValue(*this); }
	Foo(const Value& v) { *this = ValueTo<Foo>(v); }
};

INITBLOCK {
	RichValue<Foo>::Register();
}

CONSOLE_APP_MAIN
{
	Value v = RawToValue(Foo(12345));
	DUMP(v.Is<Foo>());
	DUMP(ValueTo<Foo>(v).x);
	
	Value a = RichToValue(Foo(54321));
	Value b = RichToValue(Foo(54321));
	DUMP(a == b);
	DUMP(IsNull(a));
	String s = StoreAsString(a);
	LoadFromString(v, s);
	DUMP(ValueTo<Foo>(v));
	
	Value c = Foo(321);
	Foo x = c;
	DUMP(x);
}
