#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct PosDisplay : Display {
	Color color;

	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const {
		int i = q;
		w.DrawRect(0, 0, i, r.Height(), color);
		w.DrawRect(i, 0, r.Width() - i, r.Height(), SWhite);
	}
};

struct App : TopWindow {
	StatusBar   status;
	InfoCtrl    pos;
	InfoCtrl    x, y;
	PosDisplay  dx, dy;

	virtual void MouseMove(Point p, dword)
	{
		pos.Set(0, AsString(p.x), Zx(40));
		pos.Set(1, AsString(p.y), Zx(40));
		x.Set(PaintRect(dx, 100 * p.x / GetSize().cx));
		y.Set(PaintRect(dy, 100 * p.y / GetSize().cy));
	}

	virtual void LeftDown(Point, dword)
	{
		status.Temporary("Left mouse button pressed!");
	}

	virtual void RightDown(Point, dword)
	{
		ProgressInfo f(status);
		f.Text("Progress:");
		for(int i = 0; i < 50; i++) {
			f.Set(i, 50);
			Sleep(100);
		}
	}

	App() {
		Sizeable();
		SetFrame(FieldFrame());
		AddFrame(status);
		status.AddFrame(pos.RightZ(100));
		status.AddFrame(x.Left(100));
		status.AddFrame(y.Left(100));
		dx.color = SLtRed();
		dy.color = SLtGreen();
	}
};

GUI_APP_MAIN
{
	App().Run();
}
