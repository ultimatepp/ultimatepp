#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	Drawing dw;

	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, SWhite);
		w.DrawDrawing(sz, dw);
		w.DrawDrawing(0, 0, 200, 200, dw);
		w.DrawDrawing(0, sz.cy - 100, 100, 100, dw);
	}

	App()
	{
		Sizeable();

		DrawingDraw w(100, 100);
		for(int i = 0; i < 10; i++)
			w.DrawLine(50, 50,
			           int(50 + 50 * sin(i * M_PI / 10)),
			           int(50 + 50 * cos(i * M_PI / 10)),
			           2, SLtGray);
		Size sz = GetTextSize("DRAWING", Courier(16).Italic());
		w.DrawText((100 - sz.cx) / 2, (100 - sz.cy) / 2, "DRAWING", Courier(16).Italic());
		dw = w;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
