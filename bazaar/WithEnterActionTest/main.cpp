#include "WithEnterActionTest.h"

void WithEnterActionTest::Change()
{
	edrd <<= ~ed;
}

void WithEnterActionTest::Change2()
{
	edrd <<= ~ed2;
}

WithEnterActionTest::WithEnterActionTest()
{
	CtrlLayout(*this, "Window title");
	Add(ed.HSizePos(10,10).TopPos(10,20));
	ed <<= THISBACK(Change);
	ed2 <<= THISBACK(Change2);
	
	ed.Key(K_ENTER, 1);
}

GUI_APP_MAIN
{
	WithEnterActionTest().Run();
}

