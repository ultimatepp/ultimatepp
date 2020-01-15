#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Thread work;

	void Work();
	
	ArrayCtrl list;
	
	typedef App CLASSNAME;

	App();	
	~App();
};

void Ask(bool *result)
{
	*result = PromptYesNo("Do you want to quit?");
}

void App::Work()
{
	int i = 0;
	while(!Thread::IsShutdownThreads()) {
		Sleep(50);
		if(list.GetCount() > 100) {
			GuiLock __;
			if(PromptYesNo("Do you want to quit?")) {
				Break();
				break;
			}
			list.Clear();
		}
		GuiLock __;
		list.Add(i++);
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
	Thread::ShutdownThreads();
	work.Wait();
}

GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_ELAPSED);
	App app;
	app.Run();
}
