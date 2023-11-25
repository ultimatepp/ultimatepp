#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Point  p;
	String text;

	void LeftDown(Point pos, dword flags) override {
		p = pos;
		Refresh();
	}

	void MouseMove(Point pos, dword flags) override {
		text = Format("[%d:%d]", pos.x, pos.y);
		Refresh();
	}

	void Paint(Draw& w) override {
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
