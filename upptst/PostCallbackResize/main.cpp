#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Label label;
	TimeCallback tm;
	int          ii = 0;
	
	MyApp() {
		Sizeable().Zoomable();
	
		Add(label.LeftPos(0, 1000).TopPos(0, 200));
		label.SetFont(Arial(200));
		
		tm.Set(-100, [=] { label = AsString(ii++); });
	}
	~MyApp() {
		ShutdownThreads();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
