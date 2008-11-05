#include "CtrlLibTest.h"

ChildWnd::ChildWnd()
{
	CtrlLayout(*this, "Child Window");
	btn1.Hide();
	btn2.Hide();		
}

CtrlLibTest::CtrlLibTest()
{
	CtrlLayout(*this, "Main Window");
	btn1 <<= THISBACK(OnPush1);
	btn2 <<= THISBACK(OnPush2);
}

void CtrlLibTest::OnPush1()
{
	wnd.Open(this);
	wnd.TopMost();
}

void CtrlLibTest::OnPush2()
{
	PromptOK("I'm invisible!");
}

GUI_APP_MAIN
{
	CtrlLibTest().Run();
}
