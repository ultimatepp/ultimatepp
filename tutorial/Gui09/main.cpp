#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void RightDown(Point, dword) override {
		int result = Null;
		MenuBar menu;
		for(int i = 0; i < 10; i++)
			menu.Add(AsString(i), [=, &result] { result = i; });
		menu.Separator();
		menu.Add("Exit", [=] { Exit(); });
		menu.Execute();
		if(!IsNull(result))
			PromptOK("You have selected " + AsString((int)result));
	}

	MyAppWindow() {
		Title("My application with context menu").Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
