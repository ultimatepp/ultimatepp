#include <CtrlLib/CtrlLib.h>
using namespace Upp;

struct TestStruct
{
	struct TestV : Moveable<TestV>
	{
		double  a;
		Value   b;
		String ToString() const
		{
			return Format("a=%d, b=%d", a,b);
		}
		
		void Jsonize(JsonIO &json)
		{
			json
				("a", a)
				("b", b)
			;
		}
	};
	
	void Add(double a, Value b)
	{
		TestV v;
		v.a = a;
		v.b = b;
		static int ii;
		map.Add("SomeKey" + AsString(++ii), v);
	}
	
	void Jsonize(JsonIO &json)
	{
		StringMap(json, map); // <- string map
	}
	VectorMap<String,TestV> map;
};

GUI_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Vector<Value> a, b;
	a.Add();
	String json = StoreAsJson(a);
	RDUMP(json);
	LoadFromJson(b, json);
	RDUMPC(b);
	ASSERT(b.GetCount() == 1 && IsNull(b[0]));

	String xml = StoreAsXML(a, "test");
	RDUMP(xml);
	LoadFromXML(b, xml);
	RDUMPC(b);
	ASSERT(b.GetCount() == 1 && IsNull(b[0]));
	
	TestStruct test, test2;
	test.Add(0, 0);
	test.Add(-1.2345, Date(2007, 12, 12));
	
	LoadFromJson(test2, StoreAsJson(test));

	RLOG(StoreAsJson(test));
	RLOG("===================");
	RLOG(StoreAsJson(test2));
	ASSERT(StoreAsJson(test) == StoreAsJson(test2));
	RLOG("===================");
	StoreAsJsonFile(test, NULL, true);
	LoadFromJsonFile(test2);	
	ASSERT(StoreAsJson(test) == StoreAsJson(test2));
	String fn = ConfigFile("h");
	StoreAsJsonFile(test, fn, true);
	RDUMP(LoadFile(fn));
	LoadFromJsonFile(test2);	
	ASSERT(StoreAsJson(test) == StoreAsJson(test2));

	TestStruct test3;
	ASSERT(!LoadFromJson(test2, "{\"SomeKey1\":{\"a\":\"X\",\"b\":2},\"SomeKey2\":{\"a\":1,\"b\":2}}"));
	
	ASSERT(!LoadFromJsonFile(test2, "c:aksjdfhkjaskjdfkhasdf"));

	RLOG("Everything is OK.");
}
