#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Draw& w) {
		w.DrawRect(GetSize(), White());
		static const int d[] = { PEN_NULL, PEN_SOLID, PEN_DASH, PEN_DOT, PEN_DASHDOT, PEN_DASHDOTDOT, PEN_SOLID, 1, 2, 3, 10 };
		for(int i = 0; i < __countof(d); i++) {
			w.DrawLine(10, i * 10, 200, i * 10, d[i], Red());
			w.DrawLine(300, i * 10, 400, i * 10 + 15, d[i], Red());
			w.DrawLine(450 + i * 10, 10, 450 + i * 10, 200, d[i], Red());
		}
	}

	typedef MyApp CLASSNAME;

	MyApp() {
	}
};

GUI_APP_MAIN
{
	MyApp().Sizeable().Run();
}
