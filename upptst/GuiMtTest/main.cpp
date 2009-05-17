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
	int q = 0;
	while(!Thread::IsShutdownThreads()) {
		if(++q > 200) {
#if 0
			bool result;
			Call(callback1(Ask, &result));
			if(result) {
				Ctrl::Lock __(*this);
				Break();
			}
#else
			if(PromptYesNo("Do you want to quit?")) {
				Ctrl::Lock __(*this);
				Break();
			}
#endif
			q = 0;
		}
		for(int i = 0; i < 101; i++) {
			Ctrl::Lock __(list);
			list.Set(i, 0, (int)Random());
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
