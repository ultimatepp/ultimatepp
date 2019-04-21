#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	virtual void Paint(Draw& w);
};

void MyApp::Paint(Draw& w)
{
	w.DrawRect(GetSize(), SColorPaper());
	w.DrawText(0, 0, "Hello world");
}

GUI_APP_MAIN
{
	MyApp().Run();
}
