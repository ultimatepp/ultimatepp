#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	DataPusher p;
	TopWindow win;
	p <<= AttrText("123").Center();
	p.SetDisplay(StdRightDisplay());
	win.Add(p.SizePos());
	win.Run();
}
