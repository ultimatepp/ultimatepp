#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	WithDropChoice<EditString> edit;
	
	win.Add(edit.TopPos(0, 20).LeftPos(0, 100));
	edit.AddList("Ahoj!");
	edit.DropWidth(500);
	
	win.Run();
}
