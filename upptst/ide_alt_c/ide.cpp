#include <Core/Core.h>

using namespace Upp;

#include <Core/Core.h>

#include <algorithm>
#include <vector>
#include <set>
#include <utility>

struct Foo {
	struct Change { int foo;
		int Test();
	};
	struct Item { int foo; };

	std::vector<String> Bar2(const std::vector<Item>& price, String s);
	std::vector<Change> Bar(const std::vector<Item>& price, String s);
	std::vector<Foo::Change> Bar(const std::vector<std::vector<Item>>& price, String s);
	std::vector<void (*)(int x, int y)> Bar(const std::vector<std::vector<Item>>& price, int x);
};

namespace Test {
struct Foo {
	struct Change { int foo;
		int Test();
	};
	struct Item { int foo; };

	std::vector<String> Bar2(const std::vector<Item>& price, String s);
	std::vector<Change> Bar(const std::vector<Item>& price, String s);
	std::vector<Foo::Change> Bar(const std::vector<std::vector<Item>>& price, String s);
	std::vector<void (*)(int x, int y)> Bar(const std::vector<std::vector<Item>>& price, int x);
};

};

CONSOLE_APP_MAIN
{
}
