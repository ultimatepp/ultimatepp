#include <Core/Core.h>

using namespace Upp;

template <class T>
struct MyVector : public Vector<T> {
	void Foo() {}
	
	void Xmlize(XmlIO xml) {
		Upp::Xmlize(xml, *this);
	}
};

CONSOLE_APP_MAIN
{
	MyVector<int> x;
	x.Add(10);
	DDUMP(StoreAsXML(x, "test"));
	LOG("XXX");
}
