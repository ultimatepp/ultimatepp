#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	void Paint(Draw& w) {
		PaintingPainter sw(GetSize());
		sw
		.Move(0, 0)
		.Line(0, 0).Line(0, 0).Line(0, 0).Line(0, 0)
		.Line(0, 0)
		.Arc(sqrt(-1.0), 0, 0, 0, 0)
//		.Move(dInf, dInf * tInf)
		.Line(0, 0)
		.Fill(Black());
	}
};

GUI_APP_MAIN
{
	DDUMP(sqrt(-1.0));
	DDUMP(IsNaN(sqrt(-1.0)));
	App().Run();
}
