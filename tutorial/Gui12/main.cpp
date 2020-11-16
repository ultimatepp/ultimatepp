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
		bar.AddMenu("Function", TutorialImg::Fn(), [=] { MenuFn(); })
		   .Help("This invokes MenuFn method of tutorial example");
		bar.Add(TutorialImg::Fn2(), [=] { BarFn(); })
		   .Help("This invokes BarFn method of tutorial example");
		bar.Add("Exit", TutorialImg::Exit(), [=] { Exit(); });
	}

	void MainMenu(Bar& bar) {
		bar.Sub("Menu", [=](Bar& bar) {
			SubBar(bar);
		});
	}

	MyAppWindow() {
		Title("My application with bars").Sizeable();
		AddFrame(menu);
		AddFrame(TopSeparatorFrame());
		AddFrame(tool);
		AddFrame(status);
		AddFrame(InsetFrame());
		menu.Set([=](Bar& bar) { MainMenu(bar); });
		menu.WhenHelp = status; // callback cast to fix it for older CLANG version in C++11
		tool.Set([=](Bar& bar) { SubBar(bar); });
		tool.WhenHelp = status; // callback cast to fix it for older CLANG version in C++11
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
