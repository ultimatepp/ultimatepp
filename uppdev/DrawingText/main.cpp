#include <CtrlLib/CtrlLib.h>

using namespace Upp;

String text = "        Test";

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
		Size sz = GetTextSize(text, Arial(20));
		DrawingDraw w(2 * sz.cx, 2 * sz.cx);
		for(int i = 0; i < 9; i++)
			w.DrawText(sz.cx, sz.cx, 300 * i, text, Arial(20));
		dw = w;
	}
};

GUI_APP_MAIN
{
	App().Run();
}
