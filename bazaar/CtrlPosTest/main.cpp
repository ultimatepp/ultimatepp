#include "CtrlPosTest.h"

void CtrlPosTest::ToInfo(const String& s)
{
	info.Insert(info.GetLength(), s + "\n");
	info.SetCursor(info.GetLength());
}

void CtrlPosTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
}

void CtrlPosTest::OnMissed(Point p, dword keyflags)
{
	String inf = ("Missed!");
	ToInfo(inf);
}

void CtrlPosTest::VisitCB()
{
	hk.Visit(*this);
}
void CtrlPosTest::ClearCB()
{
	hk.Clear();
}
void CtrlPosTest::EnableCB()
{
	hk.Enable();
}
void CtrlPosTest::DisableCB()
{
	hk.Disable();
}

CtrlPosTest::CtrlPosTest()
{
	CtrlLayout(*this, "Window title");

	CtrlLayout(ft);
	ft.Height(40);
	AddFrame(ft);
	
	ft.visit <<= THISBACK(VisitCB);
	ft.clear <<= THISBACK(ClearCB);
	ft.enable <<= THISBACK(EnableCB);
	ft.disable <<= THISBACK(DisableCB);

	hk.WhenLeftDown = THISBACK(OnSelect);
	hk.WhenMissed = THISBACK(OnMissed);
	hk.Visit(*this);
}

GUI_APP_MAIN
{
	CtrlPosTest().Run();
}

