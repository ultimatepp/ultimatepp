#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	Drawing dw;

	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, SWhite);
		w.DrawDrawing(sz, dw);
	}

	App()
	{
		Sizeable();
		DrawingDraw w(1000, 1000);
		for(int i = 0; i < 10; i++)
			w.DrawText(500, 500, 300 * i, "        Test", Arial(20));
		dw = w;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
