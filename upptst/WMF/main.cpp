#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	String hw;
	Drawing dw1;

	void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White());
		Drawing iw = LoadWMF("U:/germany.wmf");
		w.DrawDrawing(GetSize(), iw);
	}
	
	App() { Sizeable(); }
};

GUI_APP_MAIN
{
	App().Run();
}
