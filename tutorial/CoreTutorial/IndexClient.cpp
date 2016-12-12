#include "Tutorial.h"

void IndexClient()
{
	/// .Index and client types

	/// In order to store elements to `Index`, they type must be `Moveable`, have deep copy and
	/// defined the `operator==` and a `GetHashValue` function or method to compute the hash
	/// code. It is recommended to use `CombineHash` to combine hash values of types that
	/// already provide `GetHashValue`:

	struct Person : Moveable<Person> {
		String name;
		String surname;
	
		unsigned GetHashValue() const          { return CombineHash(name, surname); }
		bool operator==(const Person& b) const { return name == b.name && surname == b.surname; }
	
		Person(String name, String surname) : name(name), surname(surname) {}
		Person() {}
	};

	Index<Person> p;
	p.Add(Person("John", "Smith"));
	p.Add(Person("Paul", "Carpenter"));
	p.Add(Person("Carl", "Engles"));

	DUMP(p.Find(Person("Paul", "Carpenter")));
	
	///
}