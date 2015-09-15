#include "SvoValue.h"

void DumpNumber(const Value& v)
{
	RDUMP((int)v);
	RDUMP((double)v);
	RDUMP((int64)v);
	RDUMP((bool)v);
}

int xx;

Value Opt0();

void Opt() {
	Value v = Opt0();
	xx = v;
}

void CheckString()
{
	Value v = "ahoj";
	for(int i = 0; i < 2; i++) {
		String s = v;
		RDUMP(s);
		ASSERT(s == "ahoj");
		WString ws = v;
		RDUMP(ws);
		ASSERT(ws == WString("ahoj"));
		v = ws;
	}
	v = String("ahoj");
	Value w = WString("ahoj");
	ASSERT(v == w);
	RDUMP(GetHashValue(v));
	RDUMP(GetHashValue(w));
	ASSERT(GetHashValue(v) == GetHashValue(w));
}

void CheckDateTime()
{
	Time tm = GetSysTime();
	Date dt = tm;
	
	Value c;
	Value v = tm;
	RDUMP(v);
	ASSERT((Date)v == dt);
	Date xx = v;
	ASSERT(xx == dt);
	c = v;
	RDUMP(c);
	ASSERT((Date)c == dt);
	Value cv = v;
	RDUMP(cv);
	ASSERT((Date)cv == dt);
	
	Value v2 = tm;
	RDUMP(v2);
	ASSERT(v2 == v);
	c = v;
	RDUMP(c);
	ASSERT((Date)c == dt);
	ASSERT(c == tm);
	
	v = dt;
	tm = ToTime(dt);
	v2 = tm;
	
	ASSERT(v == v2);
	ASSERT(GetHashValue(v) == GetHashValue(v2));

	ASSERT(IsDateTime(v));
	ASSERT(IsDateTime(c));

	ASSERT(tm == v);
	ASSERT(tm == v2);
	ASSERT(dt == v);
	ASSERT(dt == v2);
}

void CheckValueMap()
{
	RLOG("------------------------------");
	RLOG("CheckValueMap");
	Value x = 123;
	Value y = x;
	ValueMap h;
	h.Add("0", 123);
	RDUMP(h["0"]);
	h.Add("1", Date(2001, 12, 1));
	h.Add("2", "test");
	
	Value v = h;
	ASSERT(v.GetCount() == 3);
	RDUMP(v["0"]);
	ASSERT(v["0"] == 123);
	ASSERT(v["1"] == Date(2001, 12, 1));
	ASSERT(v["2"] == "test");
	ASSERT(IsValueMap(v));
	ASSERT(IsValueArray(v));
	
	ValueMap hh = v;
	ASSERT(hh == h);
	hh.Add(123, "foo");
	ASSERT(hh.GetCount() == 4);
	ASSERT(v.GetCount() == 3);
	ASSERT(hh[123] == "foo");
	
	ValueArray va = v;
	ASSERT(va.GetCount() == 3);
	ASSERT(va[0] == 123);
	ASSERT(va[1] == Date(2001, 12, 1));
	ASSERT(va[2] == "test");
	
	ValueMap m;
	// 0         1         2         3         4
	m("A", "a")("B", "b")("A", "a")("Z", "z")("B", "b");
	ASSERT(m.Find("A") == 0);
	ASSERT(m.FindNext(0) == 2);
	ASSERT(m.Find("B") == 1);
	ASSERT(m.FindNext(1) == 4);
}

void OtherChecks()
{
	Value c;
	
	ASSERT(c.IsVoid());
	RDUMP(c.IsVoid());

	Value x = "Ahoj";
	String xx = x;
	RDUMP(xx);
	ASSERT(xx == "Ahoj");
	
	Value xw = WString("Ahoj");
	RDUMP(xw);
	RDUMP(xw == x);
	
	Value xc = x;
	RDUMP(xc);
	c = xc;
	RDUMP(c);
	
	Value y = 123;
	int yy = y;
	RDUMP(yy);

	Value xn = (int)Null;
	RDUMP(IsNull(xn));
	RDUMP(IsNull(yy));
	
	Value yc = y;
	RDUMP(y);
	
	c = y;
	RDUMP(c);
	
	Value v2 = 123.0;
	Value v3 = 123;
	Value v4 = 125;
	
	RDUMP(v2 == y);
	RDUMP(v3 == y);
	RDUMP(v4 == y);
	RDUMP(v4 == v2);

	ASSERT(v2 == v3);
	ASSERT(v2 != v4);
	ASSERT(v4 != x);	
	
	Value uu = Uuid::Create();
	RDUMP(uu);
	Value uuc = uu;
	RDUMP(uuc);

	{
		Color c = Blue;
		Value v = c;
		RDUMP(v);
		Value v2 = v;
		c = v2;
		RDUMP(c);
	}
	
	Value txt = "text";
	ASSERT(txt.GetCount() == 0);
	ASSERT(txt["hhh"].IsError());
	ASSERT(txt[3].IsError());
	
	ASSERT(txt != GetSysDate());
	ASSERT(txt == "text");
	ASSERT(txt == WString("text"));
	ASSERT(txt != 123);
	ASSERT(txt == String("text"));
	
	Value n = 123;
	ASSERT(n != "ahoj");
	ASSERT(n == 123);
	ASSERT(n == 123.0);

	Value d = 123.0;
	ASSERT(d != "ahoj");
	ASSERT(d == 123);
	ASSERT(d == 123.0);
	
	Value cc = Blue();
	ASSERT(cc != 123);
	ASSERT(cc == Blue());
	ASSERT(cc != "Blue");
}
