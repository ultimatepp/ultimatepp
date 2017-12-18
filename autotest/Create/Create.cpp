#include <Core/Core.h>

using namespace Upp;

struct Test : Moveable<Test> {
	Test(const Vector<int>& a, int b) : a(clone(a)), b(b) {}
	Test(Vector<int>&& a, int b) : a(pick(a)), b(b) {}
	Test() {}
	
	Vector<int> a;
	int         b;
};

template <class T>
void TestCreate()
{
	T h;
	DLOG("---- Create " << typeid(T).name());
	Vector<int> v;
	v.Add(12);
	// Copy-constructor
	h.Create(v, 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
	// Move-constructor
	h.Create(pick(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 0);
	v.Add(21);
	h.Create(clone(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
}

template <class T>
void TestCreateT()
{
	T h;
	DLOG("---- Create<Test> " << typeid(T).name());
	Vector<int> v;
	v.Add(12);
	// Copy-constructor
	h.template Create<Test>(v, 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
	// Move-constructor
	h.template Create<Test>(pick(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 0);
	v.Add(21);
	h.template Create<Test>(clone(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
}

template <class T>
void TestCreateMap()
{
	T h;
	DLOG("---- Create<Test> " << typeid(T).name());
	Vector<int> v;
	v.Add(12);
	// Copy-constructor
	h.template Create<Test>("a", v, 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
	// Move-constructor
	h.template Create<Test>("a", pick(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 0);
	v.Add(21);
	h.template Create<Test>("a", clone(v), 22);
	DDUMP(v.GetCount());
	ASSERT(v.GetCount() == 1);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	TestCreate<One<Test>>();
	TestCreateT<One<Test>>();
	TestCreateT<Any>();
	TestCreate<Vector<Test>>();
	TestCreateT<Array<Test>>();
	TestCreateT<InArray<Test>>();
	TestCreateMap<ArrayMap<String, Test>>();
	TestCreateMap<FixedArrayMap<String, Test>>();
	TestCreateMap<SortedArrayMap<String, Test>>();
	
	LOG("======== OK");
}
