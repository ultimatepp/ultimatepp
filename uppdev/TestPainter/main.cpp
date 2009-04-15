#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	void Paint(Draw& w) {
		DUMP(w.GetPagePixels());
		DUMP(GetSize());
		DrawPainter sw(w, GetSize());
		sw.Clear(White());
		sw.Rectangle(0, 0, 100, 100).Stroke(2, Red());
		sw.Text(0, 0, "", Arial(20)).Fill(Blue());
	}
};

GUI_APP_MAIN
{
	App().Run();
}
