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
	
	void Simple();

	std::vector<String> Bar2(const std::vector<Item>& price, String s);
	std::vector<Change> Bar(const std::vector<Item>& price, String s);
	std::vector<Foo::Change> Bar(const std::vector<std::vector<Item>>& price, String s);
	std::vector<void (*)(int x, int y)> Bar(const std::vector<std::vector<Item>>& price, int    x);

	typedef void (*PaintHook)(String s);

	static  Vector<PaintHook>& painthook();

	static  void   InstallPaintHook(PaintHook hook, String s);
	static  void   DeinstallPaintHook(PaintHook hook, String s);
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
