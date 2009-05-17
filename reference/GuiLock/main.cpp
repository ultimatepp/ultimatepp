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

void App::Work()
{
	int q = 0;
	while(!Thread::IsShutdownThreads()) {
		{
			GuiLock __;
			if(list.GetCount() > 1000) {
				if(PromptYesNo("Quit?"))
					Break();
				list.Clear();
			}
			list.Add((int64)Random());
		}
		Sleep(1);
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
	App app;
	app.Run();
}
