#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class HelloWorld : public TopWindow {
	MenuBar menu;
	StatusBar status;

public:
	typedef HelloWorld CLASSNAME;

	HelloWorld();
};

HelloWorld::HelloWorld()
{
	Title("Hello world!");
	AddFrame(menu);
	AddFrame(status);
	menu.Set([=](Bar& bar) {
		menu.Sub("File", [=](Bar& bar) {
			bar.Add("About..", [=] {
				PromptOK("{{1@5 [@9= This is the]::@2 [A5@0 Ultimate`+`+ Hello world sample}}");
			});
			bar.Separator();
			bar.Add("Exit", [=] { Close(); });
		});
	});
	status = "Welcome to the Ultimate++ !";
}

GUI_APP_MAIN
{
	HelloWorld().Run();
}
