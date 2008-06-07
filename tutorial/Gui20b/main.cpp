#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyApp : TopWindow {
	Button exit;

	MyApp() {
		SetRect(0, 0, 100, 100);
		Add(exit.SetLabel("exit").LeftPosZ(10, 64).TopPosZ(10, 24));
		exit <<= Breaker(999);
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
