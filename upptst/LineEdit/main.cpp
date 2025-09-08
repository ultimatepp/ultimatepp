#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	LineEdit edit;
	edit.NoCutLine();
	win.Add(edit.SizePos());
	win.Run();
}
