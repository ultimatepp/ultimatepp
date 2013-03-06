#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct PainterTest : public TopWindow {
	virtual void Paint(Draw& w) {
		DrawPainter p(w, GetSize()/*, MODE_NOAA*/);

		p.Clear(White());
		p.Move(100, 100);
		p.Line(200, 100);
		p.Line(300, 300);
		p.Fill(Pointf(100, 100), Blue(), Pointf(300, 300), Red());
	}
};

GUI_APP_MAIN
{
	PainterTest().Run();
}

