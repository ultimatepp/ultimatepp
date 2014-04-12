#include <Core/Core.h>

using namespace Upp;

template <class T>
void Test(Value v, bool exc)
{
	try {
		LOG(v << " -> " << typeid(T).name());
		T x = v;
		ASSERT(x == v);
		ASSERT(!exc);
	}
	catch(ValueTypeError e) {
		LOG("*** " << e);
		ASSERT(exc);
	}
	LOG("--------------------------------");
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Test<int>(123, false);
	Test<int>(123.0, false);
	Test<int>(true, false);
	Test<int>((int64)1, false);

	Test<int>("ahoj", true);
	Test<int>(WString("ahoj"), true);

	Test<Color>(Color(1, 2, 3), false);
	Test<String>(Color(1, 2, 3), true);
	Test<int>(Color(1, 2, 3), true);
	
	Test<Rectf>(Rectf(1, 2, 3, 4), false);
	Test<Complex>(Rectf(1, 2, 3, 4), true);
	Test<int>(Rectf(1, 2, 3, 4), true);
	Test<String>(Rectf(1, 2, 3, 4), true);
	Test<WString>(Rectf(1, 2, 3, 4), true);
	
	LOG("===================== OK");
}
