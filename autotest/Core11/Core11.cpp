#include <Core/Core.h>

using namespace Upp;

template <class T>
void TestTransfer()
{
	T a, b;
	b.Add(ValueTypeOf<T>());

	a = pick(b);
	ASSERT(b.GetCount() == 0);
	ASSERT(a.GetCount());

	b = clone(a);
	ASSERT(a.GetCount());
	ASSERT(b.GetCount());

	T c = pick(b);
	T d = clone(c);
	
	Swap(c, d);
	
	ASSERT(b.GetCount() == 0);
	ASSERT(c.GetCount());
	ASSERT(d.GetCount());
}

template <class T>
void TestTransfers()
{
	TestTransfer< Vector<T> >();
	TestTransfer< Array<T> >();
	TestTransfer< Index<T> >();
	TestTransfer< VectorMap<T, T> >();
	TestTransfer< ArrayMap<T, T> >();

	TestTransfer< FixedVectorMap<T, T> >();
	TestTransfer< FixedArrayMap<T, T> >();

	TestTransfer< InVector<T> >();
	TestTransfer< InArray<T> >();
	TestTransfer< SortedIndex<T> >();
	TestTransfer< SortedVectorMap<T, T> >();
	TestTransfer< SortedArrayMap<T, T> >();
}

template <class InMap, class Val>
void TestInMap()
{
	InMap m;
	
	m.Add(1, "hello");
	ASSERT(m.GetCount() == 1);
	ASSERT(m[0] == "hello");
	ASSERT(m.Get(1) == "hello");

	m.Add(2, "world");
	ASSERT(m.GetCount() == 2);
	ASSERT(m[0] == "hello");
	ASSERT(m[1] == "world");
	ASSERT(m.Get(1) == "hello");
	ASSERT(m.Get(2) == "world");
	
	ASSERT(m.FindAdd(1) == 0);
	ASSERT(m.FindAdd(2) == 1);
	ASSERT(m.FindAdd(3) == 2);
	
	ASSERT(m.FindAdd(0) == 0);
	
	ASSERT(m.FindAdd(4, "four") == 4);
	ASSERT(m.GetCount() == 5);

	ASSERT(m[4] == "four");
	
	ASSERT(m.GetAdd(3) == "");
	ASSERT(m.GetAdd(2) == "world");
	ASSERT(m.GetAdd(5) == "");
	ASSERT(m.GetCount() == 6);

	m.GetAdd(6) = "six";
	ASSERT(m.GetCount() == 7);
	ASSERT(m.Get(6) == "six");
	
	m.GetAdd(7, "seven");
	ASSERT(m[7] == "seven");
	ASSERT(m.Get(7) == "seven");
	
	ASSERT(m.Get(8, "no") == "no");
	
	ASSERT(m.FindPtr(99) == NULL);
	ASSERT(*m.FindPtr(7) == "seven");
	
	m.Shrink();
	
	InMap h;
	h.Add(1, "1");
	ASSERT(h.GetCount() == 1);
	h.Clear();
	ASSERT(h.GetCount() == 0);
	
	const Val& vv = m.GetValues();
	ASSERT(vv.GetCount() == vv.GetCount());
	ASSERT(vv[7] == "seven");
	
	InMap mm = pick(m);
	ASSERT(m.GetCount() == 0);
	ASSERT(mm.GetCount());

	m = clone(mm);
	ASSERT(m.GetCount());
	ASSERT(mm.GetCount());

	mm = pick(m);
	m = pick(mm);
	mm.Clear();
	
	mm.Swap(m);
	m.Swap(mm);
	
	ASSERT(mm.IsEmpty());
	ASSERT(mm.GetCount() == 0);
	ASSERT(m.GetCount());
	ASSERT(mm.GetCount() == 0);

	ASSERT(m.FindLowerBound(6) == 6);
	ASSERT(m.FindUpperBound(6) == 7);
	ASSERT(m.Find(6) == 6);

	m.Add(6, "another six");
	ASSERT(m.GetCount() == 9);
	ASSERT(m.FindNext(6) == 7);
	ASSERT(m.FindLast(6) == 7);
	ASSERT(m.FindPrev(7) == 6);

	ASSERT(m.GetKey(7) == 6);

	m.Remove(0);
	ASSERT(m.GetCount() == 8);
	m.RemoveKey(6);
	ASSERT(m.GetCount() == 6);
	m.Remove(1, 2);
	ASSERT(m.GetCount() == 4);
}

struct NoClone : Moveable<NoClone> {
	Vector<int> a;
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	TestTransfers<int>();
	TestTransfers<String>();
	
	TestInMap< SortedVectorMap<int, String>, InVector<String> >();
	TestInMap< SortedArrayMap<int, String>, InArray<String> >();
	
	SortedArrayMap<int, NoClone> x;
	x.Create<NoClone>(1).a.Add(1);

	{
		SortedVectorMap<int, NoClone> mm;
		mm.GetAdd(1).a.Add(12);
		mm.GetAdd(1).a.Add(13);
	}

	XmlNode n;
	n = ParseXML("<a>xxx</a><b>xxx</b>");
	String r;
	for(auto& i: n)
		r << i.GetText() << ';';
	ASSERT(r == "a;b;");

	r.Clear();
	ValueArray va = { 1, 2, 3, 4 };
	for(auto i: va)
		r << i << ";";
	ASSERT(r == "1;2;3;4;");
	
	Value v = va;
	r.Clear();
	for(auto i: v)
		r << i << ";";
	ASSERT(r == "1;2;3;4;");

	LOG("=========== OK");
}
