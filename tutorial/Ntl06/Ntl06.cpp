#include <Core/Core.h>

using namespace Upp;

struct Number {
	virtual double Get() const = 0;

	String ToString() const { return AsString(Get()); }
	
	virtual ~Number() {}
};

struct Integer : public Number {
	int n;
	virtual double Get() const { return n; }

	Integer() {}
};

struct Double : public Number {
	double n;
	virtual double Get() const { return n; }

	Double() {}
};

bool operator<(const Number& a, const Number& b)
{
	return a.Get() < b.Get();
}

CONSOLE_APP_MAIN
{
	Array<Number> num;
	num.Create<Double>().n = 15.5;
	num.Create<Integer>().n = 3;
	DUMPC(num);
	Sort(num);
	DUMPC(num);
}
