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
		for(int i = 0; i < 100; i++) {
			Ctrl::Lock __(list);
			list.Set(i, 0, (int64)Random());
		}
		Sleep(10);
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
