#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Image image;

	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), Cyan());
		w.DrawImage(10, 10, image);
	}

	MyApp() {
		ImageDraw iw(100, 40);
		iw.Alpha().DrawRect(0, 0, 100, 40, GrayColor(0));
		iw.Alpha().DrawEllipse(0, 0, 100, 40, GrayColor(255));
		iw.DrawEllipse(0, 0, 100, 40, Yellow());
		iw.DrawText(26, 10, "Image", Arial(16).Bold());
		image = iw;
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
