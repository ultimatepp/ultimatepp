#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	Label x;
	TopWindow y;
	y.Add(x.LeftPos(10, 100).TopPos(10, 100));
	x = "\1This is a very very very very very very very very very very long line";
	y.Run();
}
