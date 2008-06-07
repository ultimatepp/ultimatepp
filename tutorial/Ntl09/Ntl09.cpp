#include <Core/Core.h>

using namespace Upp;

struct Person : Moveable<Person> {
	String name;
	String surname;

	Person(String name, String surname) : name(name), surname(surname) {}
	Person() {}
};

unsigned GetHashValue(const Person& p)
{
	return CombineHash(p.name, p.surname);
}

bool operator==(const Person& a, const Person& b)
{
	return a.name == b.name && a.surname == b.surname;
}

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

unsigned GetHashValue(const Number& n)
{
	return GetHashValue(n.Get());
}

bool operator==(const Number& a, const Number& b)
{
	return a.Get() == b.Get();
}

CONSOLE_APP_MAIN
{
	Index<Person> p;
	p.Add(Person("John", "Smith"));
	p.Add(Person("Paul", "Carpenter"));
	p.Add(Person("Carl", "Engles"));

	DUMP(p.Find(Person("Paul", "Carpenter")));

	ArrayIndex<Number> n;
	n.Add(new Integer(100));
	n.Add(new Double(10.5));
	n.Add(new Integer(200));
	n.Add(new Double(20.5));

	DUMP(n.Find(Double(10.5)));
}
