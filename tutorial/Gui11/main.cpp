#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TutorialImg
#define IMAGEFILE <Gui11/images.iml>
#include <Draw/iml.h>

struct MyAppWindow : TopWindow {
	MenuBar menu;
	ToolBar tool;

	void MenuFn() {
		PromptOK("Fn activated!");
	}

	void BarFn() {
		PromptOK("Fn2 activated!");
	}

	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void SubBar(Bar& bar) {
		bar.AddMenu("Function", TutorialImg::Fn(), [=] { MenuFn(); });
		bar.Add(TutorialImg::Fn2(), [=] { BarFn(); });
		bar.Add("Exit", TutorialImg::Exit(), [=] { Exit(); });
	}

	void MainMenu(Bar& bar) {
		bar.Sub("Menu", [=](Bar& bar) { SubBar(bar); });
	}

	MyAppWindow() {
		Title("My application with bars").Sizeable();
		AddFrame(menu);
		AddFrame(tool);
		menu.Set([=](Bar& bar) { MainMenu(bar); });
		tool.Set([=](Bar& bar) { SubBar(bar); });
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
