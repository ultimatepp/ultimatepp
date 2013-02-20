#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct PainterTest : public TopWindow {
	virtual void Paint(Draw& w) {
		DrawPainter p(w, GetSize()/*, MODE_NOAA*/);

		p.Clear(White());
		p.Text(0, 0, "A", Roman(50));
		p.Fill(Black());
		
		p.Translate(100, 100);
		p.Rotate(1);
		p.Fill(Blue());

		p.Translate(200, -200);
		p.Scale(1, -1);
		p.Fill(Magenta());
		
		p.Scale(1, -1);
		p.Translate(-200, 200);
		
		p.Rotate(-1);
		p.Fill(Red());
		
		p.Translate(-100, -100);
		p.Fill(Green());
	}
};

GUI_APP_MAIN
{
	PainterTest().Run();
}

