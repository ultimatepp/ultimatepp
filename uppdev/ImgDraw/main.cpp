#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	virtual void Paint0(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, Green);
		ImageDraw iw(sz.cx, sz.cy);
		iw.Alpha().DrawRect(sz, Color(0, 255, 0));
		iw.DrawText(0, 0, "Hello world", Arial(50).Bold());
		iw.Alpha().DrawText(0, 0, "Hello world", Arial(50).Bold(), Color(255, 255, 255));
		w.DrawImage(0, 0, iw);
	}

	virtual void Paint(Draw& w)
	{
		Size sz = GetSize();
		w.DrawRect(sz, White);
		Vector<Point> h;
		h << Point(10, 10) << Point(10, 30) << Point(20, 20)
		  << Point(110, 10) << Point(110, 30) << Point(120, 20);
		int sb[] = { 3, 3 };
		w.DrawPolyPolygon(h, h.GetCount(), sb, 2, Black());
	}
};

GUI_APP_MAIN
{
	App().Run();
}
