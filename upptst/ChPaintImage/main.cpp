#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	void Paint(Upp::Draw& w) override {
		w.DrawRect(GetSize(), White());
		Draw9Slice(w, RectC(100, 100, 100, 100), CtrlsImg::OkBh(), DPI(5));
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
