#include "Tutorial.h"

void Map()
{
	/// .`VectorMap`, `ArrayMap`

	/// `VectorMap` is nothing else than a simple composition of `Index` of keys and `Vector`
	/// of values. You can use `Add` methods to put elements into the `VectorMap`:

	struct Person : Moveable<Person> {
		String name;
		String surname;
		
		String ToString() const { return String() << name << ' ' << surname; }
	
		Person(String name, String surname) : name(name), surname(surname) {}
		Person() {}
	};

	VectorMap<String, Person> m;
	
	m.Add("1", Person("John", "Smith"));
	m.Add("2", Person("Carl", "Engles"));

	Person& p = m.Add("3");
	p.name = "Paul";
	p.surname = "Carpenter";
	
	DUMP(m);

	/// `VectorMap` provides read-only access to its `Index` of keys and read-write access to its
	/// `Vector` of values:

	DUMP(m.GetKeys());
	DUMP(m.GetValues());
	
	///
	
	m.GetValues()[2].name = "Peter";
	
	DUMP(m);
	
	/// You can use indices to iterate `VectorMap` contents:

	for(int i = 0; i < m.GetCount(); i++)
		LOG(m.GetKey(i) << ": " << m[i]);
	
	/// Standard `begin` / `end` pair for `VectorMap` is the range of just values (internal Vector)
	/// - it corresponds with `operator[]` returning values:

	for(const auto& p : m)
		DUMP(p);
	
	/// To iterate through keys, you can use `begin`/`end` of internal `Index`:
	
	for(const auto& p : m.GetKeys())
		DUMP(p);
	
	/// Alternatively, it is possible to create 'projection range' of VectorMap that provides
	/// convenient key/value iteration, using `operator~` (note that is also removes 'unlinked'
	/// items, see later):
	
	for(const auto& e : ~m) {
		DUMP(e.key);
		DUMP(e.value);
	}
	
	/// Note that the 'projection range' obtained by `operator~` is temporary value, which
	/// means that if mutating operation is required for values, r-value reference has to be
	/// used instead of plain reference:

	for(const auto& e : ~m)
		if(e.key == "2")
			e.value.surname = "May";
	
	DUMP(m);

	/// You can use Find method to retrieve position of element with required key:

	DUMP(m.Find("2"));

	/// or Get method to retrieve corresponding value:

	DUMP(m.Get("2"));

	/// Passing key not present in `VectorMap` as `Get` parameter is undefined behavior (ASSERT
	/// fails in debug mode), but there exists two parameter version of `Get` that returns second
	/// parameter if the key is not found in VectorMap:

	DUMP(m.Get("33", Person("unknown", "person")));

	/// As with `Index`, you can use `Unlink` to make elements invisible for Find operations:

	m.Unlink(1);
	DUMP(m.Find("2"));

	/// `SetKey` changes the key of the element:

	m.SetKey(1, "33");
	DUMP(m.Get("33", Person("unknown", "person")));

	/// If there are more elements with the same key in `VectorMap`, you can iterate them using
	/// `FindNext` method:

	m.Add("33", Person("Peter", "Pan"));

	int q = m.Find("33");
	while(q >= 0) {
		DUMP(m[q]);
		q = m.FindNext(q);
	}
	
	/// Unlinked positions can be 'reused' using Put method:

	m.UnlinkKey("33");
	m.Put("22", Person("Ali", "Baba"));
	m.Put("44", Person("Ivan", "Wilks"));

	DUMP(m);

	/// `PickValues` / `PickIndex` / `PickKeys` / pick internal `Vector` / `Index` / `Vector`
	/// of `Index`:

	Vector<Person> ps = m.PickValues();
	Vector<String> ks = m.PickKeys();
	
	DUMP(ps);
	DUMP(ks);
	DUMP(m);
	
	/// `VectorMap` pick constructor to create map by picking:
	
	ks[0] = "Changed key";

	m = VectorMap<String, Person>(pick(ks), pick(ps));
	
	DUMP(m);

	/// `ArrayMap` is composition of Index and Array, for cases where Array is better fit for
	/// value type (e.g. they are polymorphic):

	ArrayMap<String, Person> am;
	am.Create<Person>("key", "new", "person");
	
	DUMP(am);
	
	///
}