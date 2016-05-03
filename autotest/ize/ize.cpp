#include <Core/Core.h>

using namespace Upp;

template <class T>
void Test(T& data)
{
	LOG("-----------------");
	T data2;
	String bin = StoreAsString(data);
	LoadFromString(data2, bin);
	data2.Shrink();
	ASSERT(StoreAsJson(data) == StoreAsJson(data2));
	ASSERT(StoreAsString(data) == StoreAsString(data2));
	
	String xml = StoreAsXML(data);
	DUMP(xml);
	LoadFromXML(data2, xml);
	data2.Shrink();
	ASSERT(StoreAsJson(data) == StoreAsJson(data2));
	ASSERT(StoreAsString(data) == StoreAsString(data2));
	
	String json = StoreAsJson(data);
	DUMP(json);
	data2.Clear();
	LoadFromJson(data2, json);
	data2.Shrink();
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

template <class T>
void TestFixedMap()
{
	T data;

	data.Add(1, typeid(T).name());
	data.Add(2, "123");
	data.Add(3, "433");
	
	data.Finish();
	
	Test(data);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	Test< Vector<String> >();
	Test< WithDeepCopy< Vector<String> > >();

	Test< Array<String> >();
	Test< WithDeepCopy< Array<String> > >();

	Test< Index<String> >();
	Test< WithDeepCopy< Index<String> > >();

	Test< InVector<String> >();
	Test< WithDeepCopy<InVector<String> > >();

	Test< InArray<String> >();
	Test< WithDeepCopy<InArray<String> > >();

	Test< SortedIndex<String> >();
	Test< WithDeepCopy<SortedIndex<String> > >();

	TestMap< VectorMap<int, String> >();
	TestMap< WithDeepCopy< VectorMap<int, String> > >();

	TestMap< ArrayMap<int, String> >();
	TestMap< WithDeepCopy< ArrayMap<int, String> > >();

	TestMap< SortedVectorMap<int, String> >();
	TestMap< WithDeepCopy< SortedVectorMap<int, String> > >();

	TestMap< SortedArrayMap<int, String> >();
	TestMap< WithDeepCopy< SortedArrayMap<int, String> > >();

	TestFixedMap< FixedVectorMap<int, String> >();
	TestFixedMap< WithDeepCopy< FixedVectorMap<int, String> > >();

	TestFixedMap< FixedArrayMap<int, String> >();
	TestFixedMap< WithDeepCopy< FixedArrayMap<int, String> > >();

	LOG("========= EVERYTHING OK ==========");
}
