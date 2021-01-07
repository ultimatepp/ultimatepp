#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TutorialImg
#define IMAGEFILE <Gui12/images.iml>
#include <Draw/iml.h>

struct MyAppWindow : TopWindow {
	MenuBar   menu;
	ToolBar   tool;
	StatusBar status;
	
	typedef MyAppWindow CLASSNAME; // so that we can use THISFN shortcut

	void SubBar(Bar& bar) {
		bar.AddMenu("Function", TutorialImg::Fn(), [=] { // AddMenu - only in menu
			PromptOK("Fn activated!");
		}).Help("This invokes MenuFn method of tutorial example");
		bar.Add(TutorialImg::Fn2(), [=] { // does not have image - not in toolbar
			PromptOK("Fn2 activated!");
		}).Help("This invokes BarFn method of tutorial example");
		bar.Add("Exit", TutorialImg::Exit(), [=] { // in both toolbar and menu
			if(PromptOKCancel("Exit MyApp?"))
				Break();
		});
	}

	void MainMenu(Bar& bar) {
		bar.Sub("Menu", THISFN(SubBar));
	}

	MyAppWindow() {
		Title("My application with bars").Sizeable();
		AddFrame(menu);
		AddFrame(TopSeparatorFrame());
		AddFrame(tool);
		AddFrame(status);
		AddFrame(InsetFrame());
		menu.Set(THISFN(MainMenu));
		menu.WhenHelp = status;
		tool.Set([=](Bar& bar) { SubBar(bar); }); // equivalent to THISFN(SubBar)
		tool.WhenHelp = status;
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
