#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropTime ctrl;
	TopWindow win;
	win << ctrl.TopPos(10).LeftPos(10, 500);
	win.Run();
}
