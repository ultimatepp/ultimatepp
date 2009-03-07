#include <Core/Core.h>

using namespace Upp;

struct ArrayOneHelper {
	Array<T> array;
	One<T>   one;

public:
	operator One<T>&();
	
	~ArrayOneHelper() { array.Add(one.Detach()); }
};

CONSOLE_APP_MAIN
{
	Test(array.AddOne());
}
