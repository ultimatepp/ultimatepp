#include <Core/Core.h>

using namespace Upp;

template <class T>
void Test(T& data)
{
	T data2;
	String xml = StoreAsXML(data);
	DUMP(xml);
	LoadFromXML(data2, xml);
	ASSERT(StoreAsJson(data) == StoreAsJson(data2));
	ASSERT(StoreAsString(data) == StoreAsString(data2));
	
	String json = StoreAsJson(data);
	DUMP(json);
	data2.Clear();
	LoadFromJson(data2, json);
	ASSERT(StoreAsXML(data) == StoreAsXML(data2));
	ASSERT(StoreAsString(data) == StoreAsString(data2));
}

template <class T>
void Test()
{
	T data;

	data.Add(typeid(T).name());
	data.Add("123");
	data.Add("433");
	
	Test(data);
}

template <class T>
void TestMap()
{
	T data;

	data.Add(1, typeid(T).name());
	data.Add(2, "123");
	data.Add(3, "433");
	
	Test(data);
}

CONSOLE_APP_MAIN
{
	Test< Vector<String> >();
	Test< WithDeepCopy< Vector<String> > >();
	Test< Vector<String> >();
	Test< WithDeepCopy< Vector<String> > >();

	Test< Array<String> >();
	Test< WithDeepCopy< Array<String> > >();
	Test< Array<String> >();
	Test< WithDeepCopy< Array<String> > >();

	Test< Index<String> >();
	Test< WithDeepCopy< Index<String> > >();
	Test< Index<String> >();
	Test< WithDeepCopy< Index<String> > >();

	Test< ArrayIndex<String> >();
	Test< WithDeepCopy< ArrayIndex<String> > >();
	Test< ArrayIndex<String> >();
	Test< WithDeepCopy< ArrayIndex<String> > >();

	TestMap< VectorMap<int, String> >();
	TestMap< WithDeepCopy< VectorMap<int, String> > >();
	TestMap< VectorMap<int, String> >();
	TestMap< WithDeepCopy< VectorMap<int, String> > >();
	
	LOG("========= EVERYTHING OK ==========");
}
