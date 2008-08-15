#include "AssistTest.h"

AssistTest::AssistTest()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	AssistTest().Run();
}
