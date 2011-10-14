#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropList dl;
	TopWindow win;
	win.SetRect(0, 0, 400, 50);
	win.Add(dl.HSizePos(4, 4).TopPos(10, Ctrl::STDSIZE));
	dl.Add(0, AttrText("This is bold").Bold());
	dl.Add(1, AttrText("This is red").Ink(Red()));
	dl.Add(2, AttrText("This has icon").SetImage(CtrlImg::print()));
	dl.Add(3, AttrText("This is centered").Center());
	dl.SetLineCy(max(18, GetStdFontCy()));
	dl <<= 0;
	win.Run();
}
