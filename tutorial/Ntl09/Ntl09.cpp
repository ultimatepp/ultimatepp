#include <Core/Core.h>

using namespace Upp;

struct Person : Moveable<Person> {
	String name;
	String surname;

	unsigned GetHashValue() const          { return CombineHash(name, surname); }
	bool operator==(const Person& b) const { return name == b.name && surname == b.surname; }

	Person(String name, String surname) : name(name), surname(surname) {}
	Person() {}
};

CONSOLE_APP_MAIN
{
	Index<Person> p;
	p.Add(Person("John", "Smith"));
	p.Add(Person("Paul", "Carpenter"));
	p.Add(Person("Carl", "Engles"));

	DUMP(p.Find(Person("Paul", "Carpenter")));
}
