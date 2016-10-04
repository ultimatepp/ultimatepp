#include <Core/Core.h>
using namespace Upp;

struct A
{
	void Xmlize(XmlIO &xml) { xml ("v",v); } //<-- exception on 2nd call

	Value v;
};

CONSOLE_APP_MAIN
{
	A a;
	a.v = "test\ntest";
	StoreAsXMLFile(a, "XmlizeTest", "xmlizeTtest");
	DDUMP(a.v);
	a.v = 0;
	LoadFromXMLFile(a, "xmlizeTtest");
	DDUMP(a.v);
}
