#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void RightDown(Point, dword) override {
		MenuBar::Execute(
			[=](Bar& bar) {
				bar.Add("Exit", [=] { Exit(); });
			}
		);
	}

	MyAppWindow() {
		Title("My application with local menu").Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
