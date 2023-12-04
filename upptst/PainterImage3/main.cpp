#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterImage3/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterImage3/test.iml>
#include <Draw/iml_source.h>

struct MyApp : TopWindow {
	DropList dl;
	Pointf p1 = Pointf(0, 0);
	Pointf p2 = Pointf(200, 0);
	Pointf p3 = Pointf(200, 200);
	
	void Paint(Draw& w) override {
		Size sz = GetSize();
		DrawPainter iw(w, GetSize());
		iw.Clear(SWhite());
		iw.Rectangle(0, 0, sz.cx, sz.cy).Fill(TestImg::naso(), p1, p2);
	}

	void LeftDown(Point p, dword keyflags) override
	{
		p1 = p;
		Refresh();
	}

	void RightDown(Point p, dword keyflags) override
	{
		p2 = p;
		Refresh();
	}
	
	MyApp() {
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
