#include <Core/Core.h>

#include <string>

namespace upp {

template <class T>
struct ImTest {
//	friend void Foo(T& x) {}
};

struct string { int x; };

};

template <class T>
struct ImTest2 {
public:
	friend void Foo(T& x) {}
};

using namespace upp;
using namespace std;

CONSOLE_APP_MAIN
{
//	upp::ImTest<int> alfa;
	upp::string x;
	std::string y;

//	ImTest2<double> beta;
//	int x;
///	Foo(x);
//	double y;
//	Foo(y);
}
