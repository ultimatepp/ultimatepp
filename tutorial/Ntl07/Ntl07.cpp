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
	BiVector<int> n;
	n.AddHead(1);
	n.AddTail(2);
	n.AddHead(3);
	n.AddTail(4);
	DUMPC(n);

	n.DropHead();
	DUMPC(n);

	n.DropTail();
	DUMPC(n);

	BiArray<Number> num;
	num.CreateHead<Integer>().n = 3;
	num.CreateTail<Double>().n = 15.5;
	num.CreateHead<Double>().n = 2.23;
	num.CreateTail<Integer>().n = 2;
	DUMPC(num);
}
