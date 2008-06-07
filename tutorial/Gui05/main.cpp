#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Point  p;
	String text;

	virtual void LeftDown(Point pos, dword flags) {
		p = pos;
		Refresh();
	}

	virtual void MouseMove(Point pos, dword flags) {
		text = Format("[%d:%d]", pos.x, pos.y);
		Refresh();
	}

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SWhite);
		w.DrawText(p.x, p.y, text, Arial(20), Magenta);
	}

	MyAppWindow() {
		Title("My application").Zoomable().Sizeable();
		p.x = p.y = 0;
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
