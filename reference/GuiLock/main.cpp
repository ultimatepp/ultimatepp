#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Thread work;
	
	ArrayCtrl list;
	
	typedef App CLASSNAME;

	App();
	~App();
};

App::App()
{
	list.AddColumn("Test");
	Add(list.SizePos());
	work.Run([=] {
		for(;;) {
			Sleep(1);
			GuiLock __; // After locking GuiLock __, access is allowed to GUI object except opening/closing windows
			if(IsShutdownThreads())
				break;
			if(list.GetCount() > 100) {
				bool quit;
				Ptr<Ctrl> p = this;
				Call([=, &quit] {
					if(p) // check whether App still exists (e.g. can be closed by close button)
						quit = PromptYesNo("Quit?");
				});
				// quit = PromptYesNo("Quit?"); // (but Prompt has this ability already implemented, so direct call would work too)
				if(quit) {
					Break();
					return;
				}
				list.Clear();
			}
			list.Add((int64)Random());
		}
	});
}

App::~App()
{
	ShutdownThreads();
}

GUI_APP_MAIN
{
	App app;
	app.Run();
}
