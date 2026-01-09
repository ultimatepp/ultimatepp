#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	String text = "Press any arrow key...";

	bool Key(dword key, int count) override {
		switch(key) {
		case K_UP:
			text = "Up ⬆";
			Refresh();
			return true;
		case K_DOWN:
			text = "Down ⬇";
			Refresh();
			return true;
		case K_LEFT:
			text = "Left ⬅";
			Refresh();
			return true;
		case K_RIGHT:
			text = "Right ⮕";
			Refresh();
			return true;
		default:
			return TopWindow::Key(key, count);
		}
	}

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), SWhite);
		w.DrawText(45, 20, text, Arial(20), Magenta);
	}

	MyAppWindow() {
		Title("My application").Zoomable();
		SetRect(0, 0, 400, 100);
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
