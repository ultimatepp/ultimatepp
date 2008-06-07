#include <Core/Core.h>

using namespace Upp;

struct Number {
	virtual double Get() const = 0;

	String ToString() const { return AsString(Get()); }
};

struct Integer : public Number {
	int n;
	virtual double Get() const { return n; }

	Integer(int n) : n(n) {}
};

struct Double : public Number {
	double n;
	virtual double Get() const { return n; }

	Double(double n) : n(n) {}
};

bool operator<(const Number& a, const Number& b)
{
	return a.Get() < b.Get();
}

CONSOLE_APP_MAIN
{
	Array<Number> num;
	num.Add(new Integer(3));
	num.Add(new Double(15.5));
	num.Add(new Double(2.23));
	num.Add(new Integer(2));
	num.Add(new Integer(20));
	num.Add(new Double(-2.333));
	DUMPC(num);
	Sort(num);
	DUMPC(num);
}
