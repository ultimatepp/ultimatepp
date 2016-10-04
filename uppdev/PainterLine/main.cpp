#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct PainterLine : public TopWindow {
	int wd;
	Point tg;
/*
	virtual Image CursorImage(Point p, dword keyflags)
	{
		return Null;
	}
*/
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
		w.DrawRect(GetSize(), White());
		{
			ImageBuffer ib(100, 100);
			BufferPainter sw(ib);
			sw.PreClip();
			sw.Scale(1.1);
			sw.Clear(White());
			sw.Move(-50, -50).Line(tg.x, tg.y).Dash("10 5").Stroke(5, Blue());
			w.DrawImage(50, 50, ib);
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

