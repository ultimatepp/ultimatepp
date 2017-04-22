#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void LeftDown(Point p, dword keyflags) {
		FullScreen(!IsFullScreen());
	}
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawText(0, 0, IsFullScreen() ? "FULLSCREEN" : "OVERLAPPED");
	}
	
	MyApp() {
		Zoomable().Sizeable();
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
