#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : TopWindow {
	virtual void LeftDown(Point, dword) {
		Image m = OverrideCursor(CtrlImg::exclamation());
		Sleep(1000);
		OverrideCursor(m);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
