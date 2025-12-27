#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// The reason: stroke width should affect cubic tolerance
// the line arc is way to small, so it really contains just 2 points or so...

struct MyApp : public TopWindow {
	void Paint(Draw& w) override {
		DrawPainter p(w, GetSize());
		p.Clear(White());
		auto Strokes = [&] {
			DLOG("== 18");
			p.Stroke(18, Green());
			DLOG("== 15");
			p.Stroke(15, Red());
			DLOG("== 5");
			p.Stroke(5, Blue());
			DLOG("== 1");
			p.Stroke(1, Yellow());
			DLOG("== 0.1");
			p.Stroke(0.1, Cyan());
		};
		for(int pass = 0; pass < 4; pass++) {
			DLOG("----------------------------- 1");
			p.RoundedRectangle(10, 10, 20, 30, 1);
			Strokes();
			DLOG("----------------------------- 0.1");
			p.RoundedRectangle(50, 10, 20, 30, 0.1);
			Strokes();
			DLOG("----------------------------- 0.01");
			p.RoundedRectangle(90, 10, 20, 30, 0.01);
			Strokes();
			DLOG("----------------------------- 0.001");
			p.RoundedRectangle(130, 10, 20, 30, 0.001);
			Strokes();
			p.Offset(0, 50);
			p.Scale(2.8);
		}
	}
};

GUI_APP_MAIN
{
	MyApp().Zoomable().Run();
}
