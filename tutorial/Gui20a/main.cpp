#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button exit;

	void Exit() {
		Break(999);
	}

	MyAppWindow() {
		SetRect(0, 0, 100, 100);
		Add(exit.SetLabel("exit").LeftPosZ(10, 64).TopPosZ(10, 24));
		exit << [=] { Exit(); };
	}
};

GUI_APP_MAIN
{
	MyAppWindow().Run();
}
