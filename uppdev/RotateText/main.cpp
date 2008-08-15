#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	virtual void Paint(Draw& w) {
		Size sz = GetSize();
		w.DrawRect(sz, White);
		for(int i = 0; i < 3600; i += 300)
			w.DrawText(sz.cx / 2, sz.cy / 2, i, "Rotated text");
	}
};

GUI_APP_MAIN
{
	App().Run();
}
