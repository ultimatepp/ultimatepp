#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Label label;
	
	MyApp() {
		Sizeable().Zoomable();

		Add(label.LeftPos(0, 1000).TopPos(0, 200));
		label.SetFont(Arial(200));
		
		Thread::Start([=] {
			int ii = 0;
			for(;;) {
				Sleep(100);
				GuiLock __;
				if(IsShutdownThreads())
					break;
				label = AsString(ii++);
			}
		});
	}
	~MyApp() {
		ShutdownThreads();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
