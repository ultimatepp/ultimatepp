#include <Core/Core.h>

using namespace Upp;

struct Foo {
	float x;
	
	void Jsonize(JsonIO& io) { io("x", x); }
	void Xmlize(XmlIO& io) { io("x", x); }
};


CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	auto Test = [&](float x) {
		DLOG("===========");
		DDUMP(x);
		DDUMP(IsNull(x));
		Value v = x;
		DDUMP(v);
		DDUMP(IsNumber(v));
		DDUMP((int)v);
		DDUMP((int64)v);
		DDUMP((bool)v);
		DDUMP((double)v);
		
		DDUMP(v == x);
		DDUMP(v == 1);
		DDUMP(v > Value(1));
		
		DDUMP(AsJSON(v));
		
		Foo t, u, w;
		t.x = x;
		DDUMP(StoreAsJson(t));
		LoadFromJson(u, StoreAsJson(t));
		ASSERT(t.x == u.x);
		DDUMP(StoreAsXML(t));
		LoadFromXML(w, StoreAsXML(t));
		ASSERT(t.x == w.x);
		
		DDUMP(ScanFloat(AsString(x)));
		ASSERT(x == ScanFloat(AsString(x)));
		ASSERT(v == ScanFloat(AsString(x)));
	};
	
	Test(0);
	Test(0.23);
	Test(1);
	Test(1.23);
	Test(51.23);
	Test(Null);
	
	CheckLogEtalon();
}
