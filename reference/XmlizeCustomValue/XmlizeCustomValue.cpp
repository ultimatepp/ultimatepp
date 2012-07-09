#include <Core/Core.h>

using namespace Upp;

struct MyCustomValue : ValueType<MyCustomValue, 514> {
	int    x, y, z;

	String   ToString() const                { return "MyCustomValue"; }


	operator Value() const                   { return RichValue<MyCustomValue>(*this); }
	MyCustomValue(const Value& v)            { *this = v.To<MyCustomValue>(); }
	void     Xmlize(XmlIO& xio);

	MyCustomValue()                          {}
};

void MyCustomValue::Xmlize(XmlIO& xio)
{
	xio
		.Attr("x", x)
		.Attr("y", y)
		.Attr("z", z)
	;
}

INITBLOCK {
	Value::Register<MyCustomValue>("MyCustomValue");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	MyCustomValue a;
	a.x = 1;
	a.y = 2;
	a.z = 3;
	Value v = a;
	LOG(StoreAsXML(v, "Test"));
}
