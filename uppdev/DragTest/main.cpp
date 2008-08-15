#include "DragTest.h"



DragTest::DragTest()
{
	Add(ctrl);
	ctrl.VSizePos(100, 0).HSizePosZ(0, 0);
}

void DragCtrl::LeftDrag(Point p, dword keyflags)
{
	PromptOK("Drag start");
}

void DragCtrl::RightDown(Point p, dword keyflags)
{
	MenuBar::Execute(THISBACK(OnBar));
}

void DragCtrl::OnBar(Bar &bar)
{
	bar.Add("Context Item", THISBACK(OnBarItem));
}

void DragCtrl::OnBarItem()
{
}

GUI_APP_MAIN
{
	DragTest().Run();
}
