#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) override {
		w.DrawRect(GetSize(), White());
		w.DrawImage(100, 100, CtrlImg::undo(), Red());
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
