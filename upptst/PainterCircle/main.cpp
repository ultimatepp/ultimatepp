#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct MyApp : public TopWindow {
	void Paint(Draw& draw) override {
		DrawPainter w(draw, GetSize());
		w.Clear(White());
	//	w.Circle(1400 + 760, 750, 600).Stroke(150, Blue());
		w.Scale(150);
		DLOG("============================ HERE");
		w.Circle(5, 5, 4).Stroke(1, Blue());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
