#include "LTest.h"

using namespace Upp;

LTest::LTest()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	LTest().Run();
}

