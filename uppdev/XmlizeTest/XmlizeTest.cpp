#include <Core/Core.h>

using namespace Upp;

template <class T>
struct MyVector : public Vector<T> {
	void Foo() {}
	
	void Xmlize(XmlIO xml) {
		Upp::Xmlize(xml, *this);
	}
};

void CONSOLE_APP_MAIN0()
{
	MyVector<int> x;
	x.Add(10);
	DDUMP(StoreAsXML(x, "test"));
	LOG("XXX");
}

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
	a.v = "test";
	StoreAsXMLFile(a, "XmlizeTest", "xmlizeTtest");
	a.v = 0;
	LoadFromXMLFile(a, "xmlizeTtest");
}
