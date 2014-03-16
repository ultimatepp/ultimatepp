#include <Core/Core.h>

using namespace Upp;

struct Base {
	virtual String Get() = 0;
	virtual ~Base() {}
};

struct Derived1 : Base {
	virtual String Get() { return "Derived1"; }
};

struct Derived2 : Base {
	virtual String Get() { return "Derived2"; }
};

void MakeDerived1(One<Base>& t)
{
	t.Create<Derived1>();
}

void MakeDerived2(One<Base>& t)
{
	t.Create<Derived2>();
}

VectorMap<int, void (*)(One<Base>&)> factories;

INITBLOCK {
	factories.Add(0, MakeDerived1);
	factories.Add(1, MakeDerived2);
};

void Create(One<Base>& t, int what)
{
	(*factories.Get(what))(t);
}

CONSOLE_APP_MAIN
{
	One<Base> s;
	
	DUMP((bool)s);
	
	s.Create<Derived1>();

	DUMP((bool)s);
	DUMP(s->Get());
	
	s.Create<Derived2>();
	DUMP((bool)s);
	DUMP(s->Get());
	
	s.Clear();
	DUMP((bool)s);

	Create(s, 0);
	DUMP(s->Get());
}
