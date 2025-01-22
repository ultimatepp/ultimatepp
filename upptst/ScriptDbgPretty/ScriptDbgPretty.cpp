#include <Core/Core.h>

using namespace Upp;

struct MyStruct {
	int first[10];
	int second = 123456;
};

template <class T>
struct MyVector : Vector<T> {};

template <class T>
struct MyArray : Array<T> {};

template <class K, class V>
struct MyVectorMap : VectorMap<K, V> {};

struct MyWString : WString {};

CONSOLE_APP_MAIN
{
	MyWString ws;
	ws.Cat("Hello world!");
	DLOG("BREAKPOINT HERE");

	MyVector<String> hv;
	MyArray<String> ha;
	MyVectorMap<int, String> vm;
	MyStruct ms;

	for(int i = 0; i < 10000; i++) {
		hv << FormatIntRoman(i, true);
		ha << FormatIntRoman(i, false);
		vm.Add(i, FormatIntRoman(i, true));
	}
	
	DLOG("BREAKPOINT HERE");
}
