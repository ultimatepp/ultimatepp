#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TutorialImg
#define IMAGEFILE <Gui10/images.iml>
#include <Draw/iml.h>

struct MyAppWindow : TopWindow {
	MenuBar menu;

	void Exit() {
		if(PromptOKCancel("Exit MyApp?"))
			Break();
	}

	void SubMenu(Bar& bar) {
		bar.Add("Exit", TutorialImg::Exit(), THISBACK(Exit));
	}

	void MainMenu(Bar& bar) {
		bar.Add("Menu", THISBACK(SubMenu));
	}

	typedef MyAppWindow CLASSNAME;

	MyAppWindow() {
		Title("My application with menu and images").Sizeable();
		AddFrame(menu);
		menu.Set(THISBACK(MainMenu));
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
