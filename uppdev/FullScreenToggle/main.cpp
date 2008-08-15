#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	bool fullscreen;

	virtual void LeftDown(Point, dword) {
		Close();
		DUMP(IsOpen());
		FullScreen(fullscreen = !fullscreen);
		SetRect(100, 100, 400, 400);
		OpenMain();
	}
	
	App() { fullscreen = false; }
};

GUI_APP_MAIN
{
	App app;
	app.OpenMain();
	Ctrl::EventLoop();
}

