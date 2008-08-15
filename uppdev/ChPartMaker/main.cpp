#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		w.DrawImage(1, 1, CtrlsImg::SBVU());
		ChPaint(w, 100, 100, 100, 100, CtrlsImg::SBVU());
	}
};

GUI_APP_MAIN
{
	Ctrl::SetSkin(ChStdSkin);
	App().Run();
}
