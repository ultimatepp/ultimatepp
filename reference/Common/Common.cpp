#include <Core/Core.h>

using namespace Upp;

struct Foo : Moveable<Foo>, Comparable<Foo> {
	String a;
	int    b;

	String   ToString() const            { return String().Cat() << '(' << a << ", " << b << ')'; }
	unsigned GetHashValue() const        { return CombineHash() << a << b; }
	int      Compare(const Foo& x) const { return CombineCompare(a, x.a)(b, x.b); }
	void     Serialize(Stream& s)        { s % a % b; }
	void     Xmlize(XmlIO xml)           { xml("text", a)("number", b); }
	bool     IsNullInstance() const      { return IsNull(a); }

	Foo()                                {}
};

Foo RandFoo()
{
	Foo c;
	c.a << (rand() & 3);
	c.b = rand() & 3;
	return c;
}

CONSOLE_APP_MAIN
{
	VectorMap<Foo, int> data;
	for(int i = 0; i < 100; i++) {
		Foo a = RandFoo();
		Foo b = RandFoo();
		LOG(a << " < " << b << " ... " << (a < b));
		LOG(a << " == " << b << " ... " << (a == b));
		DUMP(GetHashValue(a));
		String x = StoreAsString(a);
		LoadFromString(b, x);
		DUMP(b);
		LOG("--------------");
		data.GetAdd(a, 0)++;
	}
	Vector<int> q = GetSortOrder(data.GetKeys());
	for(int i = 0; i < data.GetCount(); i++)
		LOG(data.GetKey(q[i]) << " ... " << data[q[i]]);
	LOG("------");
	LOG(StoreAsXML(data, "Foo"));
	Foo a;
	DUMP(IsNull(a));
	a = RandFoo();
	DUMP(IsNull(a));
}
