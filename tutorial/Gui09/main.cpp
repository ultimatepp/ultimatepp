#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void RightDown(Point, dword) {
		CallbackArgTarget<int> result;
		MenuBar menu;
		for(int i = 0; i < 10; i++)
			menu.Add(AsString(i), result[i]);
		menu.Separator();
		menu.Add("Exit", THISBACK(Exit));
		menu.Execute();
		if(!IsNull(result))
			PromptOK("You have selected " + AsString((int)result));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with context menu").Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
