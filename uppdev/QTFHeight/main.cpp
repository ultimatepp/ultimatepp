#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	TopWindow win;
	RichTextCtrl rt;
	String str = "[3A^test^ Graphical introduction]";
	rt = str;
	rt.SetFrame(BlackFrame());
	rt.NoSb();
	win.Add(rt.LeftPos(10, rt.GetWidth() + 2).TopPos(rt.GetHeight(INT_MAX) + 2));
	win.Run();
}