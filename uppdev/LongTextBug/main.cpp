#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	virtual void Paint(Draw& w);
};

void App::Paint(Draw& w)
{
	w.DrawRect(GetSize(), White);
	w.DrawText(10, 10, LoadFile("e:/long.txt"));
}

GUI_APP_MAIN
{
	App().Run();
}

