#include <CtrlLib/CtrlLib.h>

using namespace Upp;

const int mg = 6;

struct MyApp : TopWindow {
	Size msz;
	virtual void Paint(Draw& w) {
		Size sz =  GetSize();
		w.DrawRect(GetSize(), White());
		const int at = 64 * mg + 20;
		w.DrawImage(5, 0, Magnify(RescaleFilter(RescaleImg::test(), msz, FILTER_BILINEAR), mg, mg));
		w.DrawImage(sz.cx - msz.cx * mg, 0, Magnify(RescaleFilter(RescaleImg::test(), msz, Null), mg, mg));

//		w.DrawImage(5, at, Magnify(RescaleFilter(RescaleImg::test(), 32, 32, FILTER_BILINEAR), mg, mg));
//		w.DrawImage(at, at, Magnify(Rescale(RescaleImg::test(), 32, 32), mg, mg));
	}
	
	virtual void MouseMove(Point p, dword keyflags)
	{
		msz = p / mg;
		Refresh();
	}
	
	
	MyApp() {
		msz = Size(0, 0);
		Maximize();
	}
};

GUI_APP_MAIN
{
//	RescaleFilter(RescaleImg::test(), 32, 32, FILTER_BILINEAR);
	MyApp().Run();
}
