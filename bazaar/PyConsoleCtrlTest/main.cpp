#include "PyConsoleCtrlTest.h"

PyConsoleCtrlTest::PyConsoleCtrlTest()
{
	CtrlLayout(*this, "Window title");
	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	PyConsoleCtrlTest().Run();
}

