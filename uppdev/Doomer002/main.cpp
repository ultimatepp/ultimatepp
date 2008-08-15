#include <CtrlLib/CtrlLib.h>



GUI_APP_MAIN
{
	TopWindow win;
	LineEdit edit;
	edit.NoHorzScrollbar();
	win.Add(edit.HSizePos().TopPos(0, 20));
	win.Run();
}
