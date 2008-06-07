#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void LocalMenu(Bar& bar) {
		bar.Add("Exit", THISBACK(Exit));
	}

	void RightDown(Point, dword) {
		MenuBar::Execute(THISBACK(LocalMenu));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with local menu").Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
