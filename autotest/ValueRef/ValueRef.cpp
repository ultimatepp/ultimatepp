#include <Core/Core.h>

using namespace Upp;

bool OK;

#define CHECK(x, ref) { \
String val = AsString(x); \
bool ok = val == ref; \
LOG("CHECK(" << #x << ", " << AsCString(val) << ");"); \
ASSERT(ok); }

CONSOLE_APP_MAIN
{
	OK = true;

	StdLogSetup(LOG_COUT|LOG_FILE);

	ValueMap m;
	m("key1", "value1");
	m("key2", "value2");
	
	Value vm = m;
	Value vm2 = m;
	vm.At(1) = "123";
	
	CHECK(vm, "{ key1: value1, key2: 123 }");
	CHECK(vm2, "{ key1: value1, key2: value2 }");

	Value json;
	Value& h = json("monday");
	h("morning")(0) = "a";
	h("morning")(1) = "x";
	h("evening") = "supper";
	CHECK(json, "{ monday: { morning: [a, x], evening: supper } }");
	
	ValueArray va;
	va.Add("Node");

	Value v = va;

	Value& v1 = va.At(1);

	v1 = 123;
	
	CHECK(va, "[Node, 123]");
	
	Value v2;
	v2.At(0) = 1;
	Value v3 = v2;
	v2.At(1) = 2;
	CHECK(v2, "[1, 2]");
	CHECK(v3, "[1]");

	{
		Value va;
		CHECK(va, "");
		va << 1;
		CHECK(va, "[1]");
		va << 1 << "dva" << 3.0;
		CHECK(va, "[1, 1, dva, 3]");
	}
	
	{
		Value m;
		m.At(0) = "zero";
		m("hello") = "one";
		CHECK(m, "{ 0: zero, hello: one }");
	}
	
	ASSERT(OK);
	
	LOG("============================== OK");
}
