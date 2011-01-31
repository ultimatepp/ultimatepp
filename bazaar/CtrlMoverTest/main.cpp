#include "CtrlMoverTest.h"

void CtrlMoverTest::ToInfo(const String& s)
{
	vis.info.Insert(vis.info.GetLength(), s + "\n");
	vis.info.SetCursor(vis.info.GetLength());
}

void CtrlMoverTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
	hk.OnCtrlLeft(c, p, keyflags);
}

void CtrlMoverTest::OnMissed(Point p, dword keyflags)
{
	String inf = ("Missed!");
	ToInfo(inf);
	hk.OnMissed(p, keyflags);
}

void CtrlMoverTest::VisitCB()
{
	hk.Visit(vis);
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

void CtrlMoverTest::DeepCB()
{
	hk.deep = ~ft.deep;
}

void CtrlMoverTest::IgnoreFrameCB()
{
	hk.ignoreframe = ~ft.ignoreframe;
}

CtrlMoverTest::CtrlMoverTest()
{
	CtrlLayout(vis);
	SetRect(vis.GetRect());
	Add(vis.SizePos());
	
	Sizeable().Zoomable();

	CtrlLayout(sb);
	sb.Width(sb.GetSize().cx);
	vis.AddFrame(sb);
	vis.pc.Add(es.HCenterPos(50).VCenterPos(20));

	CtrlLayout(ft);
	ft.Height(ft.GetSize().cy);
	AddFrame(ft);
	
	ft.visit <<= THISBACK(VisitCB);
	ft.clear <<= THISBACK(ClearCB);
	ft.enable <<= THISBACK(EnableCB);
	ft.disable <<= THISBACK(DisableCB);
	ft.deep <<= THISBACK(DeepCB);
	ft.ignoreframe <<= THISBACK(IgnoreFrameCB);

	ft.deep <<= true;
	DeepCB();

	ft.ignoreframe <<= true;
	IgnoreFrameCB();

	hk.WhenLeftDown = THISBACK(OnSelect);
	hk.WhenMissed = THISBACK(OnMissed);
	hk.Visit(vis);
}

GUI_APP_MAIN
{
	CtrlMoverTest().Run();
}

