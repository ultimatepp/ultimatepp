#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White());

	Font fnt = StdFont(10);
	w.DrawText(100, 200, "Quick brown 1234567890", fnt);
	w.DrawText(100, 200, 900, "Quick brown 1234567890", fnt);

	fnt = Arial(10);
	w.DrawText(300, 200, "Quick brown 1234567890", fnt);
	w.DrawText(300, 200, 900, "Quick brown 1234567890", fnt);
}

GUI_APP_MAIN
{
	MyApp().Run();
}
