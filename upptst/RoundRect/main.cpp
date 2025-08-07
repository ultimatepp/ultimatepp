#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : public TopWindow {
	void Paint(Draw& w) override {
		DrawPainter p(w, GetSize());
		p.Clear(White());
		p.Scale(22);
		p.Translate(5, 5);
		DLOG("===============");
		p.LineJoin(LINEJOIN_ROUND);
		p.RoundedRectangle(-2, -2, 100, 100, 5).Stroke(5, Blue());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
