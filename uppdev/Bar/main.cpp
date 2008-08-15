#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <Bar/test.iml>
#include <Draw/iml.h>

struct MyApp : TopWindow {
	EditDate date;
	ToolBar  toolbar;

	void Set() {
		date <<= GetSysDate();
	}

	void Tool(Bar& bar) {
		bar.Add("Test1", TestImg::test(), THISBACK(Set)).Label("TEST!");
		bar.Separator();
		bar.Add(date, 300);
	}

	typedef MyApp CLASSNAME;

	MyApp() {
		toolbar.MaxIconSize(Size(16, 16));
		toolbar.Set(THISBACK(Tool));
		AddFrame(toolbar);
		Sizeable();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
