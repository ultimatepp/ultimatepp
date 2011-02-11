#include "WithEnterActionTest.h"

static int ii = 0;

void WithEnterActionTest::Change()
{
	edrd <<= ~ed;
	ei <<= ++ii;
}

void WithEnterActionTest::Change2()
{
	edrd <<= ~ed2;
}

WithEnterActionTest::WithEnterActionTest()
{
	CtrlLayout(*this, "Window title");
	Add(ed.HSizePos(8,8).TopPos(10,20));
	ed <<= THISBACK(Change);
	ed2 <<= THISBACK(Change2);
	
	ed.Key(K_ENTER, 1);
}

GUI_APP_MAIN
{
	WithEnterActionTest().Run();
}

