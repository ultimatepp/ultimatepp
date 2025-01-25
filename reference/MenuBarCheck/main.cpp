#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	typedef MyApp CLASSNAME;

	bool check = false;

	void RightDown(Point p, dword) {
		MenuBar bar;
		bar.Add("Check", [=] { check = !check; }).Check(check);
		bar.Execute();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
