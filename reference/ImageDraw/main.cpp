#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	Image img;

	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), SWhite);
		w.DrawImage(0, 0, img);
	}

	App() {
		ImageDraw w(100, 100);
		w.DrawRect(0, 0, 100, 100, SGray);
		w.DrawEllipse(0, 0, 100, 100, SGreen);
		w.DrawText(0, 0, "ImageDraw!", Arial(13).Bold(), SYellow);
		img = w;
		Sizeable();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
