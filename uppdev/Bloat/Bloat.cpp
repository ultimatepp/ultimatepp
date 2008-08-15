#include <Core/Core.h>
#include <vector>
#include <map>

using namespace Upp;
using namespace std;

//#define TESTSTL

#ifdef TESTSTL

struct Foo1 {
	int    a, b;

	Foo1(int a, int b) : a(a), b(b) {}
	Foo1() {}
};

struct Foo2 {
	int    a;
	float  b;

	Foo2(int a, int b) : a(a), b(b) {}
	Foo2() {}
};


CONSOLE_APP_MAIN
{
	std::string key = "key";

	vector<int> fa;
	fa.push_back(10);
	map<string, int> fb;
	fb[key] = 10;

	std::vector<Foo1> x;
	x.push_back(Foo1(1, 2));
	std::map<std::string, Foo1> map1;
	map1[key].a = 10;
	std::map<std::string, Foo2> map2;
	map2[key].a = 10;

}

#else

struct Foo1 : Moveable<Foo1> {
	int    a, b;

	Foo1(int a, int b) : a(a), b(b) {}
	Foo1() {}
};

struct Foo2 : Moveable<Foo2> {
	int    a;
	float  b;

	Foo2(int a, int b) : a(a), b(b) {}
	Foo2() {}
};

int main(int, const char **)
{
	std::string k = "key";

	vector<int> fa;
	fa.push_back(10);
	map<string, int> fb;
	fb[k] = 10;

	Vector<Foo1> x;
	x.Add(Foo1(1, 2));
	String key = "key";
	VectorMap<String, Foo1> map1;
	map1.GetAdd(key).a = 10;
	VectorMap<String, Foo2> map2;
	map2.GetAdd(key).a = 10;
}

#endif
