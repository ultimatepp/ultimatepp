#include <Core/Core.h>

template <class T = void()>
struct Test {
	T *fn;

	void Fire() { fn(1, 2); }
};

template <class T, class Fn>
struct TestT {
	T  *ptr;
	Fn *fn;

	void Fire() { fn(1, 2); }
};

void tt(int i, int j)
{
	LOG(i << ' ' << j);
}

struct PP {
	void tt(int x, int y);
};

CONSOLE_APP_MAIN
{
	Test<void (int x, int y)> test;
	test.fn = &tt;
	test.Fire();
}
