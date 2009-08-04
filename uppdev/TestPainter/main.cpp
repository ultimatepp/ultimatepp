#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

struct App : TopWindow {
	void Paint(Draw& w) {
		Font fnt = Roman(20);
		String txt = "IČ Z";
		Vector<int> dx;
		for(int i = 0; i < txt.GetCount(); i++)
			dx.Add(fnt['W']);
		{
			DrawPainter sw(w, GetSize());
			sw.Clear(White());
			for(int i = 0; i < 20; i++) {
				sw.DrawRect(100 + 50 * i, 100, 4, 4, i & 1 ? LtRed() : LtBlue());
				sw.DrawText(100 + 50 * i, 100, 300 * i, txt, fnt, i & 1 ? Red() : Blue());
				sw.DrawRect(100 + 50 * i, 200, 4, 4, i & 1 ? LtRed() : LtBlue());
				sw.DrawText(100 + 50 * i, 200, 300 * i, txt, fnt, i & 1 ? Red() : Blue(), dx);
			}
		}
		for(int i = 0; i < 20; i++) {
			w.DrawRect(100 + 50 * i, 300, 4, 4, i & 1 ? LtRed() : LtBlue());
			w.DrawText(100 + 50 * i, 300, 300 * i, txt, fnt, i & 1 ? Red() : Blue());
			w.DrawRect(100 + 50 * i, 400, 4, 4, i & 1 ? LtRed() : LtBlue());
			w.DrawText(100 + 50 * i, 400, 300 * i, txt, fnt, i & 1 ? Red() : Blue(), dx);
		}
	}
};

GUI_APP_MAIN
{
	App().Run();
}
