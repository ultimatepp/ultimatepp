#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button exit;

	MyAppWindow() {
		SetRect(0, 0, Zx(100), Zy(100));
		Add(exit.SetLabel("exit").LeftPosZ(10, 64).TopPosZ(10, 24));
		exit << Breaker(999);
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
