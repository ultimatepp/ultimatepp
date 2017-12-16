#include <Core/Core.h>

using namespace Upp;

struct Test : Moveable<Test> {
	Test(const Vector<int>& a, int b) : a(clone(a)), b(b) {}
	Test(Vector<int>&& a, int b) : a(pick(a)), b(b) {}
	
	Vector<int> a;
	int         b;
};

CONSOLE_APP_MAIN
{
	{
		Array<Test> h;
		Vector<int> v;
		v.Add(12);
		// Copy-constructor
		h.Create<Test>(v, 22);
		DDUMP(v.GetCount());
		ASSERT(v.GetCount() == 1);
		// Move-constructor
		h.Create<Test>(pick(v), 22);
		DDUMP(v.GetCount());
		ASSERT(v.GetCount() == 0);
		v.Add(21);
		h.Create<Test>(Vector<int>(v, 0), 22);
		DDUMP(v.GetCount());
		ASSERT(v.GetCount() == 1);
	}
	{
		Vector<Test> h;
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
		h.Create(Vector<int>(v, 0), 22);
		DDUMP(v.GetCount());
		ASSERT(v.GetCount() == 1);
	}
}
