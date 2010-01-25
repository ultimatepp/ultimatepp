#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

namespace Upp {
Image DownScale(const Image& img, int nx, int ny);
};

struct App : TopWindow {
	void Paint(Draw& w) {
		Size sz = GetSize();
		ImageDraw idw(200, 200);
		idw.DrawRect(0, 0, 200, 200, White);
		idw.DrawText(0, 0, "(Hi)", Serif(100));
		

		Image m = idw;
		
		{
			DrawPainter sw(w, GetSize());
			sw.Clear(White());
			sw.DrawImage(100, 50, m, Rect(30, 30, 100, 100));
			sw.DrawImage(100, 200, 20, 20, m, Rect(50, 50, 100, 100));
			sw.DrawImage(0, 500, m);
			sw.DrawImage(20, 20, 300, 30, CtrlImg::exclamation(), Blue());
		}
		
		w.DrawImage(300, 50, m, Rect(30, 30, 100, 100));
		w.DrawImage(300, 200, 20, 20, m, Rect(50, 50, 100, 100));
		w.DrawImage(300, 500, m);
	}
};

GUI_APP_MAIN
{
	App().Sizeable().Run();
}
