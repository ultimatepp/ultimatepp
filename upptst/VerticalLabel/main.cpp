#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	Label l;
	l = "This is a label";
	win << l.SizePos();
	l.SetOrientation(ORIENTATION_VERTICAL_LEFT);
	win.Run();
}
