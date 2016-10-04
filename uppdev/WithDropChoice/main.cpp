#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	WithDropChoice<EditString> x;
	EditString y;
	TopWindow w;
	w.Add(x.LeftPos(10, 100).TopPos(10, 19));
	w.Add(y.LeftPos(110, 100).TopPos(10, 19));
	
	w.Run();
}

