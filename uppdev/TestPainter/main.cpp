#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	void Paint(Draw& w) {
		DrawPainter sw(w, GetSize());
		sw
		.Move(0, 0)
		.Line(0, 0).Line(0, 0).Line(0, 0).Line(0, 0)
		.Line(0, 0)
		.Arc(0, 0, 0, 0, 0)
//		.Move(dInf, dInf * tInf)
		.Line(0, 0)
		.Fill(Black());
	}
};

GUI_APP_MAIN
{
	App().Run();
}
