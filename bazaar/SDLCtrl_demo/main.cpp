#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <SDLCtrl_demo/demo.lay>
#include <CtrlCore/lay.h>

void Run_Demo1();
void Run_Basic();

struct SDLCtrl_Demo : public WithMain<TopWindow> {
typedef SDLCtrl_Demo CLASSNAME;	
	void OnRun() {
		String demo = grid.Get(0);
		if (demo == "demo1")
			Run_Demo1();
		else if (demo == "basic")
			Run_Basic();
	}
	SDLCtrl_Demo() {
		CtrlLayout(*this, "SDLCtrl Examples");
		
		butRun.WhenAction = THISBACK(OnRun);
		
		grid.AddColumn("Demo", 10);
		grid.AddColumn("Description", 60);
		grid.Add("demo1", "Based in unodgs SDL demo, including SDLCtrl and fullscreen support");
		grid.Add("basic", "Basic demo");
		grid.SetLineCy(int(1.4*StdFont().GetCy()));
		grid.WhenLeftDouble = THISBACK(OnRun);
	}
};

GUI_APP_MAIN
{
	SDLCtrl_Demo().Run();
}
