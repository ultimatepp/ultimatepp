#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <ChScrollbars/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <ChScrollbars/test.iml>
#include <Draw/iml_source.h>

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawImage(10, 10, TestImg::scrollbar());
		
		Image sb = WithHotSpot(TestImg::scrollbar(), CH_SCROLLBAR_IMAGE, 0);
		Image vsb = WithHotSpot(RotateClockwise(TestImg::scrollbar()), CH_SCROLLBAR_IMAGE, 0);
		ChPaint(w, 10, 120, 200, DPI(16), sb);
		for(int i = 0; i < 40; i += 4) {
			ChPaint(w, 10, 160 + i * DPI(5), 4 * i, DPI(16), sb);
			ChPaint(w, 360 + i * DPI(5), 10, DPI(16), 4 * i, vsb);
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
