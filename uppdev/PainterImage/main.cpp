#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

namespace Upp {
Image DownScale(const Image& img, int nx, int ny);
};

struct App : TopWindow {
	void Paint(Draw& w) {
		Size sz = GetSize();
		DrawPainter sw(w, GetSize());
		sw.Clear(White());
		sw.Rectangle(-10000, -10000, 20000, 20000)
		  .Circle(300, 100, 95)
		  .EvenOdd().Clip();
		sw.DrawText(0, 0, "XXXXXXXXXX", Arial(300));
	}
};

GUI_APP_MAIN
{
	App().Sizeable().Run();
}
