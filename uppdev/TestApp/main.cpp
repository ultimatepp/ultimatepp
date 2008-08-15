#include "TestApp.h"



TestApp::TestApp()
{
	CtrlLayout(*this, "Window title");
//	grid.AddColumn("Test").Edit(editstring);
	b1.SetLabel("Button1");
	b2.SetLabel("Butto2");
	b2.Show();
	gridremove.WhenAction = THISBACK(Remove);
	toolbar.Wrap();
	toolbar.Set(THISBACK(sigBar));
}


void TestApp::sigBar(Bar& bar)
{
	bar.Add(b1, 60, 27);
//	bar.Gap(INT_MAX);
	bar.GapRight();
	bar.Add(b2, 60, 27);
}
GUI_APP_MAIN
{
	TestApp app;
	app.Sizeable().Zoomable().Run();
}
