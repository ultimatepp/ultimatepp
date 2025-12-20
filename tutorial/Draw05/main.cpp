#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Image image;

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), Cyan());
		w.DrawImage(10, 10, image);
	}

	MyApp() {
		ImageDraw iw(200, 80);
		iw.Alpha().DrawRect(0, 0, 200, 80, GrayColor(0));
		iw.Alpha().DrawEllipse(0, 0, 200, 80, GrayColor(255));
		iw.DrawEllipse(0, 0, 200, 80, Yellow());
		iw.DrawText(50, 15, "Image", Arial(32).Bold());
		image = iw;
	}
};

GUI_APP_MAIN
{
	MyApp app;
	app.Title("ImageDraw").Zoomable().Sizeable();
	app.SetRect(0, 0, 220, 100);
	app.Run();
}
