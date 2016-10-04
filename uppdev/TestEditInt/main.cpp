#include "TestEditInt.h"

using namespace Upp;

#define LAYOUTFILE <TestEditInt/TestEditInt.lay>
#include <CtrlCore/lay.h>

class TestEditInt : public WithTestEditIntLayout<TopWindow>
{
	public:
		typedef TestEditInt CLASSNAME;
		TestEditInt();
};


TestEditInt::TestEditInt()
{
	CtrlLayout(*this, "Window title");
	edt.OnSides();
//	edt.OnSides();
}

GUI_APP_MAIN
{
	TestEditInt().Run();
}
