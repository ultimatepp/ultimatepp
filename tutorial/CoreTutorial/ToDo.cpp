#include "Tutorial.h"

#if 0
	return CombineCompare(a.NDX, b.NDX)(a.V_TEXT, b.V_TEXT)(a.V_NUMBER)(b.V_NUMBER);

	Vector<int> id = clone(_id);
	
	SplitTo(ln.Mid(4), '@', path, ln);
	

CONSOLE_APP_MAIN
{
	Event<const Vector<int>&> h = [](const Vector<int>& x) { DUMP(x); };

	Event<> ev;

	{
		Vector<int> x = { 1, 2 };
		ev = [=, x = pick(x)] { h(x); };
	}
	
	ev();
}
#endif

