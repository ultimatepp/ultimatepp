#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Drawing drawing;

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		w.DrawDrawing(10, 10, 50, 60, drawing);
		w.DrawDrawing(100, 10, 150, 100, drawing);
		w.DrawDrawing(10, 110, 300, 300, drawing);
	}

	MyApp() {
		DrawingDraw iw(200, 200);
		iw.DrawEllipse(10, 10, 180, 100, Cyan());
		iw.DrawImage(100, 100, CtrlImg::exclamation());
		iw.DrawRect(20, 100, 30, 30, Blue);
		drawing = iw;
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
