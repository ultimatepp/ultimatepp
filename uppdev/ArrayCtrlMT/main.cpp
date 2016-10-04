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
	for(;;) {
		Sleep(1);
		GuiLock __;
		if(IsShutdownThreads())
			break;
		if(list.GetCount() > 10000) {
			if(PromptYesNo("Quit?")) {
				Break();
				return;
			}
			list.Clear();
		}
		list.Add(AsString((int64)Random()) + String('x', 200));
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
