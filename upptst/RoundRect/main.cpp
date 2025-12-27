#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	void Paint(Draw& w) override {
		DrawPainter p(w, GetSize());
		p.Clear(White());
		p.Scale(22);
		p.Translate(5, 5);
		DLOG("===============");
#if 1
		p.RoundedRectangle(-2, -2, 100, 100, 5, 5);
		// RoundedRectangle(p, -2, -2, 100, 100, 5, 5);
		p.Stroke(5, Blue());
#endif
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
