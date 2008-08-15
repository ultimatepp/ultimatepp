#include <Core/Core.h>
#include <vector>
#include <map>

using namespace Upp;
using namespace std;

//#define TEST_STL
//#define TEST_UPP

struct Foo : Moveable<Foo> {
	int    a, b;

	Foo(int a, int b) : a(a), b(b) {}
	Foo() {}
};

CONSOLE_APP_MAIN
{
	{
		std::string key = "key";
		vector<int> fa;
		fa.push_back(10);
		map<string, int> fb;
		fb[key] = 10;
#ifdef TEST_STL
		vector<Foo> x;
		x.push_back(Foo(1, 2));

		map<int, Foo> map1;
		map1[20].a = 10;

		map<std::string, Foo> map2;
		map2[key].a = 10;
#endif
	}
	{
		String key = "key";
		Vector<int> fa;
		fa.Add(10);
		VectorMap<String, int> fb;
		fb.GetAdd(key) = 10;
#ifdef TEST_UPP
		Vector<Foo> x;
		x.Add(Foo(1, 2));

		VectorMap<int, Foo> map1;
		map1.GetAdd(20).a = 10;

		VectorMap<String, Foo> map2;
		map2.GetAdd(key).a = 10;
#endif
	}
}
