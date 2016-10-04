#include "testcrash.h"



testcrash::testcrash()
{
	CtrlLayout(*this, "Window title");
	PostCallback(THISBACK(testing));
}

void testcrash::testing()
{
	mList.AddColumn("TEST");
}

GUI_APP_MAIN
{
	testcrash().Run();
}

