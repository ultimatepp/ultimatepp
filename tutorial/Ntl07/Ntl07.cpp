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
	num.AddHead(new Integer(3));
	num.AddTail(new Double(15.5));
	num.AddHead(new Double(2.23));
	num.AddTail(new Integer(2));
	num.AddHead(new Integer(20));
	num.AddTail(new Double(-2.333));
	DUMPC(num);
}
