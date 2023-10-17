#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());

		w.DrawRect(10, 10, 60, 80, Green());

		w.DrawLine(100, 10, 160, 80, 0, Black());
		w.DrawLine(160, 10, 100, 80, 4, Red());
		w.DrawLine(160, 40, 100, 50, PEN_DOT, Red());

		w.DrawEllipse(210, 20, 80, 60, Blue());

		w.DrawEllipse(310, 20, 80, 60, LtBlue(), 5, Red());

		w.DrawArc(RectC(410, 20, 80, 60), Point(10, 10), Point(450, 80), 3, Cyan);

		Vector<Point> p;
		p << Point(30, 110) << Point(60, 180) << Point(10, 150) << Point(70, 150);
		w.DrawPolyline(p, 4, Black);

		p.Clear();
		p << Point(130, 110) << Point(160, 180) << Point(110, 150) << Point(170, 120)
		  << Point(130, 110);
		w.DrawPolygon(p, Blue);

		p.Clear();
		p << Point(230, 110) << Point(260, 180) << Point(210, 150) << Point(270, 120)
		  << Point(230, 110);
		w.DrawPolygon(p, Cyan, 5, Magenta);

		p.Clear();
		p << Point(330, 110) << Point(360, 180) << Point(310, 150) << Point(370, 120)
		  << Point(330, 110);
		w.DrawPolygon(p, Cyan, 5, Magenta, I64(0xaa55aa55aa55aa55));

		w.DrawImage(40, 240, CtrlImg::save());
		w.DrawImage(110, 210, 80, 80, CtrlImg::save());
		w.DrawImage(240, 240, CtrlImg::save(), Blue);
		w.DrawImage(310, 210, 80, 80, CtrlImg::save(), Blue);

		w.DrawText(20, 330, "Hello world!");
		w.DrawText(120, 330, "Hello world!", Arial(15).Bold());
		w.DrawText(220, 330, "Hello world!", Roman(15).Italic(), Red);
		w.DrawText(320, 380, 400, "Hello world!", Courier(15).Underline());
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
