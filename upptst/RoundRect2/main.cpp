#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// The reason: stroke width should affect cubic tolerance
// the line arc is way to small, so it really contains just 2 points or so...

struct MyApp : public TopWindow {
	void Paint(Draw& w) override {
		DrawPainter p(w, GetSize());
		p.Clear(White());
		p.Scale(22);
		DLOG("-----------------------------");
		p.Move(10, 10);
		p.Arc(10, 10, 0.1, M_PI, M_PI);
//		p.RoundedRectangle(2, 2, 100, 100, 0.1)
//		p.Stroke(5, Blue());
		p.Stroke(0.1, Blue());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
