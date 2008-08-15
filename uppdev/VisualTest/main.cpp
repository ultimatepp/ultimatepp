#include "VisualTest.h"

void Test()
{
	PromptOK("Test");
}


VisualTest::VisualTest()
{
	CtrlLayoutOKCancel(*this, "Window title");
	list.HeaderObject().Absolute();
	for(int i = 0; i < 5; i++)
		list.AddColumn(AsString(i), 100).HeaderTab().WhenAction = callback(Test);
	for(int i = 0; i < 100; i++) {
		list.Add(i);
		droplist.Add(i);
	}
	tabs.Add().Text("A");
	tabs.Add().Text("B");
//	tabs.Add().Text("BVB");
//	tabs.Add().Text("Baas");
//	tabs.Add().Text("Basdasd");
//	tabs.Add().Text("Basdasdasd");
//	tabs.Add().Text("Basdasdasdasdasd");
	Sizeable().Zoomable();
	disabled.Disable();
	pi.Set(44, 0);
	slider.Range(100);
	slider <<= THISBACK(PI);
	bo.SetImage(CtrlImg::cut());
	exit.Exit();
	bo.Disable();
	SetTimeCallback(-50, THISBACK(PI));
	dc.AddList("Test");
}

GUI_APP_MAIN
{
//	Ctrl::SetSkin(ChStdSkin);
	VisualTest().Run();
	FileSel fs;
//	fs.ExecuteOpen();
}
