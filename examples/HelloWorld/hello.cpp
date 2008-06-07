#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class HelloWorld : public TopWindow {
	MenuBar menu;
	StatusBar status;

	void FileMenu(Bar& bar);
	void MainMenu(Bar& bar);
	void About();

public:
	typedef HelloWorld CLASSNAME;

	HelloWorld();
};

void HelloWorld::About()
{
	PromptOK("{{1@5 [@9= This is the]::@2 [A5@0 Ultimate`+`+ Hello world sample}}");
}

void HelloWorld::FileMenu(Bar& bar)
{
	bar.Add("About..", THISBACK(About));
	bar.Separator();
	bar.Add("Exit", THISBACK(Close));
}

void HelloWorld::MainMenu(Bar& bar)
{
	menu.Add("File", THISBACK(FileMenu));
}

HelloWorld::HelloWorld()
{
	AddFrame(menu);
	AddFrame(status);
	menu.Set(THISBACK(MainMenu));
	status = "Welcome to the Ultimate++ !";
}

GUI_APP_MAIN
{
	SetLanguage(LNG_ENGLISH);
	HelloWorld().Run();
}
