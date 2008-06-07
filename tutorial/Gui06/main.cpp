#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	virtual void Close() {
		delete this;
	}

	virtual void LeftDown(Point pos, dword flags) {
		(new MyAppWindow)->OpenMain();
	}

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SWhite);
		w.DrawText(0, 0, "Click the view area to open next window!", Arial(20));
	}

	MyAppWindow() {
		Title("My multiwindowed application").Zoomable().Sizeable();
	}
};

GUI_APP_MAIN
{
	(new MyAppWindow)->OpenMain();
	Ctrl::EventLoop();
}
