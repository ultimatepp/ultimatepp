#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Thread work;

	void Work();
	void AskQuit(bool *quit);
	
	ArrayCtrl list;
	
	typedef App CLASSNAME;

	App();
	~App();
};

void App::AskQuit(bool *quit)
{
	*quit = PromptYesNo("Quit?");
}

void App::Work()
{
	for(;;) {
		Sleep(1);
		GuiLock __;
		if(IsShutdownThreads())
			break;
		if(list.GetCount() > 100) {
			bool quit;
			Call(PTEBACK1(AskQuit, &quit)); // This is the generic way for any GUI (dlg)
			// quit = PromptYesNo("Quit?"); // But Prompt has this ability already implemented
			if(quit) {
				Break();
				return;
			}
			list.Clear();
		}
		list.Add((int64)Random());
	}
}

App::App()
{
	list.AddColumn("Test");
	Add(list.SizePos());
	work.Run(THISBACK(Work));
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
