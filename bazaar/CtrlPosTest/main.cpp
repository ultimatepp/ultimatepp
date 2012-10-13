#include "CtrlPosTest.h"

void CtrlPosTest::ToInfo(const String& s)
{
	vis.info.Insert(vis.info.GetLength(), s + "\n");
	vis.info.SetCursor(vis.info.GetLength());
}

void CtrlPosTest::OnSelect(Ctrl& c, Point p, dword keyflags)
{
	String inf = "Selected: ";
	inf << String(typeid(c).name());
	ToInfo(inf);
}

void CtrlPosTest::VisitCB()
{
	hk.SetSource(&vis);
}
void CtrlPosTest::ClearCB()
{
	hk.ClearSource();
}
void CtrlPosTest::EnableCB()
{
	hk.Enable();
	hk.SetFocus();
}
void CtrlPosTest::DisableCB()
{
	hk.Disable();
}

void CtrlPosTest::DeepCB()
{
	hk.flags &= ~CtrlFinder::DEEP;
	hk.flags |= (~ft.deep)?(CtrlFinder::DEEP):(0);
}

void CtrlPosTest::FrameCB()
{
	hk.flags &= ~CtrlFinder::FRAME;
	hk.flags |= (~ft.frame)?(CtrlFinder::FRAME):(0);
}

void CtrlPosTest::ViewCB()
{
	hk.flags &= ~CtrlFinder::VIEW;
	hk.flags |= (~ft.view)?(CtrlFinder::VIEW):(0);
}

CtrlPosTest::CtrlPosTest()
{
	SetRect(Size(400,400));
	Sizeable().Zoomable();

	CtrlLayout(vis);

	CtrlLayout(sb);
	sb.Width(sb.GetSize().cx);
	vis.AddFrame(sb);
	vis.pc.Add(es.HCenterPos(50).VCenterPos(20));
	st.SetText("STATIC");
	vis.pc.Add(st.LeftPos(10,50).TopPos(20,20));

	CtrlLayout(ft);
	ft.Height(ft.GetSize().cy);
	AddFrame(ft);
	
	CtrlLayout(bot);
	bot.Height(50);
	vis.pc.AddFrame(bot);

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

	Add(sc.SizePos());
	sc.AddPane(vis);
	sc.WhenScrolled = callback(&hk, &CtrlPos::Update);

	hk.WhenLeftSelect = THISBACK(OnSelect);
	hk.SetSource(&vis);
	hk.SetFocus();
}

void CtrlPosTest::Activate()
{
	hk.SetFocus();	
}

GUI_APP_MAIN
{
	CtrlPosTest().Run();
}

