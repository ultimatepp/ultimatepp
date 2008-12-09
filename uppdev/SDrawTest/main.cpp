#include <CtrlLib/CtrlLib.h>
#include <SDraw/SDraw.h>

using namespace Upp;

struct App : TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		SDraw agd(sz.cx, sz.cy);
		agd.SetBackground(agg::rgba(0, 1.0, 1.0, false));
		agd.DrawLine(0,0,100,100,10);
		agd.SetBrushColor(agg::rgba(0, 0, 0));
		agd.DrawEllipse(0, 0, sz.cx, sz.cy, 3);
		
		w.DrawImage(0, 0, agd.Render());
	}
};

GUI_APP_MAIN
{
	App().Run();
}

