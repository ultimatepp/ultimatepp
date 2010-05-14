#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	LineEdit edit;
	edit <<= LoadFile(ConfigFile("data.txt"));
	TopWindow win;
//	win.SetRect(100, 100, 100, 100);
	win.Sizeable();
	win.Add(edit.HSizePos(20, 20).VSizePos(20, 20));
	win.Open();
	Ctrl::EventLoop();
	SaveFile(ConfigFile("data.txt"), ~edit);
}

// 156 0x9c ""