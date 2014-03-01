#include <Core/Core.h>

using namespace Upp;

template <class T>
struct Data {
	T val;
	
	void Xmlize(XmlIO xml) {
		xml("value", val);
	}
};

template <class T>
void Test(const T& x) {
	LOG("----------------------------------------");
	LOG("Xmlize: " << x << " [" << typeid(T).name() << "]");
	Data<T> data;
	data.val = x;
	String xml = StoreAsXML(data, "XmlizeTest");
	LOG("Xml: ");
	LOG(xml);
	Data<T> data2;
	ASSERT(LoadFromXML(data2, xml));
	LOG("Loaded: " << data2.val);
	ASSERT(data2.val == x);
}

struct DataV {
	Value val;
	
	void Xmlize(XmlIO xml) {
		xml("Value", val);
	}
};

template <class T>
void TestV(const T& x) {
	Test(x);
	LOG("-----");
	DataV data;
	data.val = x;
	String xml = StoreAsXML(data, "XmlizeValue");
	LOG("Value Xml: ");
	LOG(xml);
	DataV data2;
	ASSERT(LoadFromXML(data2, xml));
	LOG("Loaded: " << data2.val);
	ASSERT(data2.val == data.val);
}

template <class T>
struct DefData {
	T val;
	T def;
	
	void Xmlize(XmlIO xml) {
		xml("value", val, def);
	}
};

template <class T>
void TestDef(const T& x, const T& def) {
	Test(x);
	LOG("-----");
	DefData<T> data;
	data.def = def;
	LoadFromXML(data, "<empty/>");
	DLOG("Retrieved default value: " << data.val);
	ASSERT(data.val == def);
	data.val = x;
	String xml = StoreAsXML(data, "XmlizeValue");
	LOG("Value Xml: ");
	LOG(xml);
	DefData<T> data2;
	ASSERT(LoadFromXML(data2, xml));
	LOG("Loaded: " << data2.val);
	ASSERT(data2.val == data.val);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	TestV(String("Just a test!"));
	TestV(WString("Just a test!"));
	TestV((int) 123);
	Test((dword) 0xffff0000);
	TestV(12.3456789);
	Test((int16)20000);
	TestV((int64)I64(0xffff000025689));
	Test((int16)123);
	
	TestV(GetSysTime());
	TestV(GetSysDate());
	
	TestV(Point(1, 2));
	Test(Point16(1, 2));
	TestV(Point64(1, 2));
	TestV(Pointf(1, 2));

	TestV(Size(3, 4));
	Test(Size16(3, 4));
	TestV(Size64(3, 4));
	TestV(Sizef(3, 4));

	TestV(Rect(23, 12, 56, 88));
	Test(Rect16(23, 12, 56, 88));
	TestV(Rect64(23, 12, 56, 88));
	TestV(Rectf(23, 12, 56, 88));

	TestV(Magenta());
	
	TestDef(Magenta(), Blue());
	TestDef(String("Hello"), String("Hi"));
	TestDef(3.14, 2.78);
	TestDef(123, 321);
	
	LOG("*************** OK");
}
