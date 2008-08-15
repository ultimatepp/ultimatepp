#include "transerr.h"



TransTest::TransTest()
{
	CtrlLayout(*this, "Transparent paint test");

	CtrlLayout(tab1);
	tab.Add(tab1.SizePos(), "Tab 1");

	CtrlLayout(tab2);
	tab.Add(tab2.SizePos(), "Tab 2");
}

GUI_APP_MAIN
{
	TransTest().Run();
}
