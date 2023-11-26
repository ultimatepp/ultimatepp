#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), SWhite());
		w.DrawText(20, 20, "Hello world!", Arial(30), Magenta);
	}

	MyAppWindow() {
		Title("My application").Zoomable().Sizeable();
	}
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.SetRect(0, 0, 400, 100);
	app.Run();
}
