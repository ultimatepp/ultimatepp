#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;

	bool check;

	void Check() { check = !check; }

	void RightDown(Point p, dword) {
		MenuBar bar;
		bar.Add("Check", THISBACK(Check)).Check(check);
		bar.Execute();
	}
	
	MyApp() {
		check = false;
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
