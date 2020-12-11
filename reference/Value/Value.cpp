#include <Core/Core.h>

using namespace Upp;

struct RawFoo {
	String a;
	int    b;
};

struct RichFoo : ValueType<RichFoo, 513, Comparable<RichFoo, Moveable<RichFoo> > > {
	String a;
	int    b;

	int      Compare(const RichFoo& o) const { return CombineCompare(a, o.a)(b, o.b); }
	void     Serialize(Stream& s)            { s % a % b; }
	String   ToString() const                { return String().Cat() << a << ", " << b; }
	unsigned GetHashValue() const            { return CombineHash(a, b); }
	bool     IsNullInstance() const          { return IsNull(a); }

	operator Value() const                   { return RichValue<RichFoo>(*this); }
	RichFoo(const Value& v)                  { *this = v.To<RichFoo>(); }

	RichFoo()                                {}
};

INITBLOCK
{
	Value::Register<RichFoo>();
}

void Print(const Value& x)
{
	Cout() << "-------\n";
	Cout() << "Value: " << x << '\n';
	if(IsError(x))
		Cout() << "Value is Error: " << GetErrorText(x) << '\n';
	else
	if(IsNull(x))
		Cout() << "Value is Null\n";
	else
	if(IsString(x))
		Cout() << "Value is String: " << (String)x << '\n';
	else
	if(IsNumber(x))
		Cout() << "Value is a number: " << (double)x << '\n';
	else
	if(IsDateTime(x))
		Cout() << "Value is either date or time: " << (Date)x << '\n';
	else
	if(IsType<RichFoo>(x))
		Cout() << "Value is of RichFoo type: " << (RichFoo)x << '\n';
	else
	if(IsTypeRaw<RawFoo>(x)) {
		const RawFoo& r = x.To<RawFoo>();
		Cout() << "Value is of RawFoo type: " << r.a << ", " << r.b << '\n';
	}
	else
		Cout() << "Value is of another type: " << x.GetType() << '\n';
}

CONSOLE_APP_MAIN
{
	Print(12);
	Print(12.12);
	Print((int)Null);
	Print(Null);
	Print("some text");
	Print(GetSysDate());
	Print(White());
	Print(Rect(1, 2, 3, 4));
	Print(Point(5, 6));
	Print(ErrorValue("test error"));

	RichFoo x;
	x.a = "SOMETHING";
	x.b = 123456;
	Print(x);
	String bindata = StoreAsString(x);
	RichFoo xx;
	LoadFromString(xx, bindata);
	Print(xx);

	RawFoo y;
	y.a = "TEXT";
	y.b = 321;
	Print(RawToValue(y));
}
