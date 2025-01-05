#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Label label[3];
	TimeCallback tm;
	
	MyApp() {
		Sizeable().Zoomable();
	
	#if 0
		tm.Set(-100, [=] {
			static int ii;
			label[0] = AsString(ii++);
		});
	#endif
		
		for(int i = 0; i < 3; i++) {
			Add(label[i].LeftPos(0, 1000).TopPos(i * 200, 200));
			label[i].SetFont(Arial(200));
		}
		
		Thread::Start([=] {
			int ii = 0;
			for(;;) {
				Sleep(100);
				GuiLock __;
				DDUMP(ii);
				if(IsShutdownThreads())
					break;
				label[1] = AsString(ii++);
				
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
