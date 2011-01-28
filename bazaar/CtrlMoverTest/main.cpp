#include "CtrlMoverTest.h"

void CtrlMoverTest::ToInfo(const String& s)
{
	info.Insert(info.GetLength(), s + "\n");
	info.SetCursor(info.GetLength());
}

void CtrlMoverTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
}

void CtrlMoverTest::OnMissed(Point p, dword keyflags)
{
	String inf = ("Missed!");
	ToInfo(inf);
}

void CtrlMoverTest::VisitCB()
{
	hk.Visit(*this);
}
void CtrlMoverTest::ClearCB()
{
	hk.Clear();
}
void CtrlMoverTest::EnableCB()
{
	hk.Enable();
}
void CtrlMoverTest::DisableCB()
{
	hk.Disable();
}

CtrlMoverTest::CtrlMoverTest()
{
	CtrlLayout(*this, "Window title");

	CtrlLayout(ft);
	ft.Height(40);
	AddFrame(ft);
	
	ft.visit <<= THISBACK(VisitCB);
	ft.clear <<= THISBACK(ClearCB);
	ft.enable <<= THISBACK(EnableCB);
	ft.disable <<= THISBACK(DisableCB);

	//hk.WhenLeftDown = THISBACK(OnSelect);
	//hk.WhenMissed = THISBACK(OnMissed);
	hk.Visit(*this);
}

GUI_APP_MAIN
{
	CtrlMoverTest().Run();
}

