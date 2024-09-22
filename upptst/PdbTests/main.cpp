#include <CtrlLib/CtrlLib.h>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace Upp;

typedef uint64 adr_t;

struct TypeInfo : Moveable<TypeInfo> {
	int    type = 0;
	int    ref = 0; // this is pointer or reference
	bool   reference = false; // this is reference
};

typedef TypeInfo Context;

struct Val : Moveable<Val>, TypeInfo {
	bool   array = false;
	bool   rvalue = false; // data is loaded from debugee (if false, data pointed to by address)
	byte   bitpos = 0;
	byte   bitcnt = 0;
	int    reported_size = 0; // size of symbol, can be 0 - unknown, useful for C fixed size arrays
	union {
		adr_t  address;
		int64  ival;
		double fval;
	};
	Context *context = NULL; // needed to retrieve register variables

	Val At(int i) const;
	
#ifdef _DEBUG
	String ToString() const;
#endif

	Val() { address = 0; }
};

Val DeRef(Val val_v)
{
	DDUMP(&val_v);
	val_v.ref--; // todo; DBGHELP problem here
	val_v.rvalue = false;
	return val_v;
}

Val DeRef2(Val& ref_v)
{
	DDUMP(&ref_v);
	return ref_v;
}

Val DeRef3(Val *ptr_v)
{
	DDUMP(ptr_v);
	return *ptr_v;
}

void Test(String val_s)
{
	DUMP(val_s);
}

void Test2(const String& ref_s)
{
	DUMP(ref_s);
}

void Test3(String *ptr_s)
{
	DDUMP(ptr_s);
	DUMP(*ptr_s);
}

void TestF()
{
	std::string ss = "key2";
	std::string ss2 = "key2";
}

GUI_APP_MAIN
{
	{
		std::set<std::string> st;
		for(int i = 0; i < 10000; i++)
			st.insert(AsString(i).ToStd());
	}
	{
		std::map<std::string, std::string> mp;
		for(int i = 0; i < 10000; i++)
			mp[~AsString(i)] = ~AsString(i);
	}
	{
		std::multiset<std::string> mst;
		for(int i = 0; i < 10000; i++)
			mst.insert(AsString(i).ToStd());
	}
	{
		std::multimap<std::string, int> mmp;
		for(int i = 0; i < 10000; i++)
			mmp.insert(std::make_pair(~AsString(i), i));
	}
	{
		std::string ss = "key2";
//		std::string ss2 = "key2";
	}
	{
		Vector<int> x;
		for(int i = 0; i < 100000; i++)
			x.Add(i);
		int *h = x;
		h = NULL;
	}
	{
		Tuple<int, int *> h;
		int q[] = { 1, 2, 3, 4, 5 };
		h.b = q;
		h.a = 123456789;
		h.b = NULL;
	}
	{
		Value a = Rectf(1, 1, 1, 1);
		Value b = Rectf(1, 2, 3, 4);
		
		bool h = a == b;
		DDUMP(h);
	}
	
	{ // memory / reference test
		int a = 0x12345678;
		int& b = a;
		DDUMP(a);
	}
	
	{
		Test("Hello world!");
		Test2("Just a test");
		String h = "OK";
		Test3(&h);
	}
	
	{
		Val v;
		v.ref = 123;
		v.address = 321;
		v.type = 500;
		
		DDUMP(1);
		
		DeRef(v);
		DeRef2(v);
		DeRef3(&v);
	}
	
	
	Value v;
	
	v = 123;
	DDUMP(v);
	
	v = "Hello";
	DDUMP(v);
	
	v = 123.3;
	DDUMP(v);
	
	v = GetSysDate();
	DUMP(v);
	
	v = "??";
	
	v = GetSysTime();
	DDUMP(v);
	
	WString w = "wstring";
	v = w;
	DDUMP(w);
	
	v = true;
	DDUMP(v);
	
	v = (int64)1;
	DDUMP(v);
	
	ValueArray va;
	va << 1 << 2 << 3;
	
	v = va;
	DDUMP(v);
	
	ValueMap m;
	m.Add(1, "one");
	m.Add("two", 2);
	v = m;
	DDUMP(v);
	
	v = ErrorValue();
	DDUMP(v);
	
	v = ErrorValue("HellO!");
	DDUMP(v);
	
	
	Value& ref = v;
	Value *ptr = &v;
	
	LOG(ref);
	
	{
		Point p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Point64 p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Pointf p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Size p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Size64 p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Sizef p(123, 321);
		Value v = p;
		LOG(v);
	}
	{
		Rect r(12, 23, 34, 56);
		Value v = r;
		LOG(r);
	}
	{
		Rect64 r(12, 23, 34, 56);
		Value v = r;
		LOG(r);
	}
	{
		Rectf r(12, 23, 34, 56);
		Value v = r;
		LOG(r);
	}
	{
		Font fnt = Arial(10);
		Value v = fnt;
		LOG(fnt);
		LOG(v);
	}
	{
		Complex x(1, 2);
		Value v = x;
		LOG(x);
	}
	{
		Color c;
		c = Null;
		c = Red;
		Color d = Yellow;
		
		RGBA rc = c;
		
		LOG(c);
		
		Value x = Magenta();
		LOG(x);
	}
	{
		VectorMap<String, int> m;
		for(int i = 0; i < 100000; i++)
			m.Add(AsString(i), i);
	}
	{
		VectorMap<String, int> m;
		m("one", 1)("two", 2);
		
		DDUMP(m);
	}

	{
		std::vector<int> sv;
		sv.push_back(1);
		sv.push_back(2);
		sv.push_back(3);
		
		int i = 1;
	}

	{
		BiVector<int> x;
		x.AddTail(123);
		x.AddTail(124);
		x.AddHead(1);
	}
	
	{
		BiArray<int> x;
		x.AddTail(123);
		x.AddTail(124);
		x.AddHead(1);
	}
	
	{
		std::string x = "test";
		for(int i = 0; i < 10; i++)
			x.append("test");
	}

	{
		WString h = "test";
		std::wstring wx = h.ToStd();
		for(int i = 0; i < 10; i++)
			wx.append((wchar_t*)~h);
	}
	
	{
		Value v1 = GetSysDate();
		ValueMap m { { 1, "one" }, { 2, "two" }};
		Value v2 = m;
	}
}

void Foo() {
	int ii = 123;
	
	auto t = GetSysTime();
	
	Date d = GetSysDate();
	
	int a[] = { 321, 'h', 'e', 'l', 'l', 'l', 1, 2, 3 };

	int i = 123;
	int *pi = &i;
	int **ppi = &pi;
	
	int *ai = a;

	const char *s = "Hello!";
	const char **ps = &s;
	String h = s;
	
	DDUMP(h);
	
	h = "X1293409123407983785238745982730498572039487508809808909";
	
	DDUMP(h);
	
	WString ws = "WString works!";
	
	DDUMP(ws);
	
	ws = h.ToWString();
	
	DDUMP(ws);
	
	Vector<String> v;
	v << "1" << "2" << "3";
	
	DDUMP(v);
	
	Vector<String *> pv;
	pv << &h;
	DDUMP(pv);
	
	{
		Vector<byte *> charv;
		charv << (byte *)"1" << (byte *)"2";
		DDUMP(charv);
	}
	
	Array<int> av;
	av << 1 << 2 << 3;
	DDUMP(av);

	Array<size_t> av2;
	av2 << 1 << 2 << 3;
	DDUMP(av2);

	Array<int64> av64;
	av64 << 1 << 2 << 3;
	DDUMP(av64);

	Array<uint64> avu64;
	avu64 << 1 << 2 << 3;
	DDUMP(avu64);

	{
		Array<short> av3;
		av3 << 1 << 50;
		DDUMP(av3);
	}
	{
		Array<short int> av3;
		av3 << 1 << 50;
		DDUMP(av3);
	}
	
	{
		Index<String> x;
		x << "1";
		DDUMP(x);
	}
	
	{
		ArrayMap<String, int> m;
		m("one", 1)("two", 2);
		
		DDUMP(m);
	}
}
