#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TutorialImg
#define IMAGEFILE <Gui12/images.iml>
#include <Draw/iml.h>

struct MyAppWindow : TopWindow {
	MenuBar   menu;
	ToolBar   tool;
	StatusBar status;

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
		bar.AddMenu("Function", TutorialImg::Fn(), THISBACK(MenuFn))
		   .Help("This invokes MenuFn method of tutorial example");
		bar.Add(TutorialImg::Fn2(), THISBACK(BarFn))
		   .Help("This invokes BarFn method of tutorial example");
		bar.Add("Exit", TutorialImg::Exit(), THISBACK(Exit));
	}

	void MainMenu(Bar& bar) {
		bar.Add("Menu", THISBACK(SubBar));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with bars").Sizeable();
		AddFrame(menu);
		AddFrame(TopSeparatorFrame());
		AddFrame(tool);
		AddFrame(status);
		AddFrame(InsetFrame());
		menu.Set(THISBACK(MainMenu));
		menu.WhenHelp = status; // callback cast to fix it for older CLANG version in C++11
		tool.Set(THISBACK(SubBar)); 
		tool.WhenHelp = status; // callback cast to fix it for older CLANG version in C++11
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
