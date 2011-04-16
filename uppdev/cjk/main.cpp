#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w) {
		w.DrawRect(GetSize(), White);
		w.DrawText(10, 10, "ultimate++是一个性能优良的C++GUI库");
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}

