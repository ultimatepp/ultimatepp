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
		bar.AddMenu("Function", TutorialImg::Fn(), THISBACK(MenuFn));
		bar.Add(TutorialImg::Fn2(), THISBACK(BarFn));
		bar.Add("Exit", TutorialImg::Exit(), THISBACK(Exit));
	}

	void MainMenu(Bar& bar) {
		bar.Add("Menu", THISBACK(SubBar));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with bars").Sizeable();
		AddFrame(menu);
		AddFrame(tool);
		menu.Set(THISBACK(MainMenu));
		tool.Set(THISBACK(SubBar));
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
