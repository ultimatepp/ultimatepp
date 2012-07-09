#include <Core/Core.h>

using namespace Upp;

struct RawFoo {
	String x;
};

struct Foo : ValueType<Foo, 10010> {
	int x;
	
	Foo(const Nuller&)                  { x = Null; }
	Foo(int x) : x(x) {}
	Foo() {}

	String ToString() const             { return AsString(x); }
	unsigned GetHashValue() const       { return x; }
	void Serialize(Stream& s)           { s % x; }
	bool operator==(const Foo& b) const { return x == b.x; }
	bool IsNullInstance() const         { return IsNull(x); }
	
	operator Value()                    { return RichToValue(*this); }
	Foo(const Value& v)                 { *this = v.Get<Foo>(); }
};

INITBLOCK {
	Value::Register<Foo>();
}

CONSOLE_APP_MAIN
{
	RawFoo h;
	h.x = "hello";
	Value q = RawToValue(h);
	DUMP(q.Is<RawFoo>());
	DUMP(q.To<RawFoo>().x);
	
	Value a = RichToValue(Foo(54321));
	Value b = RichToValue(Foo(54321));
	DUMP(a == b);
	DUMP(IsNull(a));
	String s = StoreAsString(a);
	Value v;
	LoadFromString(v, s);
	DUMP(v.Get<Foo>());
	
	Value c = Foo(321);
	Foo x = c;
	DUMP(x);
}
