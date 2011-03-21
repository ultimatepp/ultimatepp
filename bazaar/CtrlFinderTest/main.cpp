#include "CtrlFinderTest.h"

void CtrlFinderTest::ToInfo(const String& s)
{
	vis.info.Insert(vis.info.GetLength(), s + "\n");
	vis.info.SetCursor(vis.info.GetLength());
}

void CtrlFinderTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
}

void CtrlFinderTest::VisitCB()
{
	hk.Visit(vis);
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

void CtrlFinderTest::DeepCB()
{
	hk.flags &= ~CtrlFinder::DEEP;
	hk.flags |= (~ft.deep)?(CtrlFinder::DEEP):(0);
}

void CtrlFinderTest::FrameCB()
{
	hk.flags &= ~CtrlFinder::FRAME;
	hk.flags |= (~ft.frame)?(CtrlFinder::FRAME):(0);
}

void CtrlFinderTest::ViewCB()
{
	hk.flags &= ~CtrlFinder::VIEW;
	hk.flags |= (~ft.view)?(CtrlFinder::VIEW):(0);
}

CtrlFinderTest::CtrlFinderTest()
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
	ft.frame <<= THISBACK(FrameCB);
	ft.view <<= THISBACK(ViewCB);

	ft.deep <<= true;
	DeepCB();

	ft.frame <<= true;
	FrameCB();

	ft.view <<= true;
	ViewCB();

	hk.WhenLeftDown = THISBACK(OnSelect);
	hk.Visit(vis);
}

GUI_APP_MAIN
{
	CtrlFinderTest().Run();
}

