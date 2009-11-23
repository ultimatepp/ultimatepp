#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct PainterLine : public TopWindow {
	int wd;
	Point tg;

	virtual Image CursorImage(Point p, dword keyflags)
	{
		return Null;
	}

	virtual void LeftDown(Point p, dword keyflags) {
		wd = wd + 1;
		if(wd > 10)
			wd = 0;
		Refresh();
	}
	
	virtual void MouseMove(Point p, dword keyflags)
	{
		tg = p;
		Refresh();
	}
	
	virtual void Paint(Draw& w) {
		{
			DrawPainter g(w, GetSize()/*, MODE_NOAA*/);
			g.DrawRect(GetSize(), White());
			g.DrawRect(100, 100, 200, 150, LtGray());
			g.DrawLine(100, 100, tg.x, tg.y, wd);
		}
		w.Offset(0, 200);
		w.DrawRect(100, 100, 200, 150, LtGray());
		w.DrawLine(100, 100, tg.x, tg.y, wd);
		w.End();		
	}
	
public:
	typedef PainterLine CLASSNAME;
	PainterLine() { wd = 0; }
};

GUI_APP_MAIN
{
	PainterLine().Run();
}

