#include "WithEnterActionTest.h"

void WithEnterActionTest::Change()
{
	edrd <<= ~ed;
}

WithEnterActionTest::WithEnterActionTest()
{
	CtrlLayout(*this, "Window title");
	Add(ed.HSizePos(10,10).TopPos(10,20));
	ed <<= THISBACK(Change);
}

GUI_APP_MAIN
{
	WithEnterActionTest().Run();
}

