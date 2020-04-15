#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Widget : public Ctrl {
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), LtCyan());
	}
	
	void MouseMove(Point p, dword) {
		ViewDraw draw(this, p.x - 16, p.y - 16, 32, 32);
		DrawPainter sw(draw, 32, 32);
		sw.Clear(White());
		sw.Move(0, 0).Line(31, 31);
		sw.Move(0, 31).Line(31, 0);
		sw.Stroke(5, Red());
	}
};


GUI_APP_MAIN
{
	TopWindow win;
	Widget h;
	win.Add(h.LeftPosZ(10, 300).TopPosZ(10, 300));
	win.Run();
}
