#include <Core/Core.h>

using namespace Upp;

struct Person : Moveable<Person> {
	String name;
	String surname;

	String ToString() const { return name + ' ' + surname; }

	Person(String name, String surname) : name(name), surname(surname) {}
	Person() {}
};

bool operator<(const Person& a, const Person& b)
{
	return a.surname == b.surname ? a.name < b.name : a.surname < b.surname;
}

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
	VectorMap<String, Person> m;

	m.Add("1", Person("John", "Smith"));
	m.Add("2", Person("Carl", "Engles"));

	Person& p = m.Add("3");
	p.name = "Paul";
	p.surname = "Carpenter";

	DUMPC(m.GetKeys());
	DUMPC(m.GetValues());

	for(int i = 0; i < m.GetCount(); i++)
		LOG(m.GetKey(i) << ": " << m[i]);

	DUMP(m.Find("2"));
	DUMP(m.Get("2"));

	DUMP(m.Get("33", Person("unknown", "person")));

	m.Unlink(1);
	DUMP(m.Find("2"));

	m.SetKey(1, "33");
	DUMP(m.Get("33", Person("unknown", "person")));

	m.Add("33", Person("Peter", "Pan"));
	int q = m.Find("33");
	while(q >= 0) {
		LOG(m[q]);
		q = m.FindNext(q);
	}

	m.UnlinkKey("33");
	m.Put("22", Person("Ali", "Baba"));
	m.Put("44", Person("Ivan", "Wilks"));

	DUMPC(m.GetKeys());
	DUMPC(m.GetValues());

	Vector<int> order = GetSortOrder(m.GetValues());
	DUMPC(order);

	for(int i = 0; i < order.GetCount(); i++)
		LOG(m.GetKey(order[i]) << ": " << m[order[i]]);

	Vector<Person> ps = m.PickValues();
	DUMPC(ps);

	ArrayMap<String, Number> am;
	am.Create<Integer>("A").n = 1;
	am.Create<Double>("B").n = 2.1;
	DUMPC(am.GetKeys());
	DUMPC(am.GetValues());

	DUMP(am.Get("A"));
	DUMP(am.Find("B"));
}
