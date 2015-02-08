#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct PainterTest : public TopWindow {
	virtual void Paint(Draw& w) {
		DrawPainter p(w, GetSize());

		p.Clear(White());
		p.RoundedRectangle(0, 0, 300, 300, 40, 15);
		p.Fill(Pointf(0.5, 0.3), Blue(), Red(),
		       Xform2D::Scale(150, 100) * Xform2D::Translation(150, 150),
		       GRADIENT_PAD);
		
	}
};

GUI_APP_MAIN
{
	PainterTest().Run();
}
