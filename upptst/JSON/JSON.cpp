#include <Core/Core.h>
using namespace Upp;

struct TestStruct
{
	struct TestV : Moveable<TestV>
	{
		int a;
		int b;
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
	
	void Add()
	{
		TestV v;
		v.a = 1;
		v.b = 2;
		static int ii;
		map.Add("SomeKey" + AsString(++ii), v);
	}
	
	void Jsonize(JsonIO &json)
	{
		StringMap(json, map); // <- string map
	}
	VectorMap<String,TestV> map;
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	TestStruct test, test2;
	test.Add();
	test.Add();
	
	LoadFromJson(test2, StoreAsJson(test));

	LOG(StoreAsJson(test));
	LOG("===================");
	LOG(StoreAsJson(test2));
	ASSERT(StoreAsJson(test) == StoreAsJson(test2));
	
	TestStruct test3;
	ASSERT(!LoadFromJson(test2, "{\"SomeKey1\":{\"a\":\"X\",\"b\":2},\"SomeKey2\":{\"a\":1,\"b\":2}}"));
}
