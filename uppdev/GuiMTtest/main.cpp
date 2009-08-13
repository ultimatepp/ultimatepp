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
	for(;;) {
		Sleep(1);
		GuiLock __;
		if(Thread::IsShutdownThreads())
			break;
		if(list.GetCount() > 100)
			list.Clear();
		list.Add((int64)Random());
	}
}

App::App()
{
	list.AddColumn("Test");
	list.AddColumn("Test2", 20);
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
