#include <CtrlLib/CtrlLib.h>

using namespace Upp;

Image img;

struct MyApp : TopWindow
{
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), LtGray());
		w.DrawImage(0, 0, img);
		w.DrawRect(DPI(400), DPI(40), DPI(200), DPI(80), Red());
		w.DrawText(DPI(400), DPI(40), "Here", Arial(DPI(80)), Yellow());
	}
	
	void LeftDown(Point, dword) override {
		static int ii;
		ViewDraw iw(this);
		iw.DrawRect(GetSize(), LtGray());
		iw.DrawRect(DPI(400), DPI(40), DPI(200), DPI(80), LtBlue());
		iw.DrawEllipse(100, 100, 100, 100, Green());
		iw.DrawText(DPI(400), DPI(40), AsString(ii++), Arial(DPI(80)), Yellow());
	}
};

GUI_APP_MAIN
{
	Size isz(DPI(200), DPI(200));
	ImageDraw iw(isz);
	iw.DrawRect(isz, White());
	iw.DrawText(10, 10, "Hello world");
	iw.DrawImage(10, 50, CtrlImg::exclamation());
	img = iw;
	
	MyApp().Run();
}
