#include "CtrlFinderTest.h"

void CtrlFinderTest::ToInfo(const String& s)
{
	info.Insert(info.GetLength(), s + "\n");
	info.SetCursor(info.GetLength());
}

void CtrlFinderTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
}

void CtrlFinderTest::OnMissed(Point p, dword keyflags)
{
	String inf = ("Missed!");
	ToInfo(inf);
}

void CtrlFinderTest::VisitCB()
{
	hk.Visit(*this);
}
void CtrlFinderTest::ClearCB()
{
	hk.Clear();
}
void CtrlFinderTest::EnableCB()
{
	hk.Enable();
}
void CtrlFinderTest::DisableCB()
{
	hk.Disable();
}

CtrlFinderTest::CtrlFinderTest()
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
	CtrlFinderTest().Run();
}

