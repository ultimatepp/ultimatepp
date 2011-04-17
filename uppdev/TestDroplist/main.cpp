#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropList dl;
	for(int i = 0; i < 100; i++)
		dl.Add(i);
	dl <<= 20;
	dl.SetReadOnly();
	TopWindow w;
	w.Add(dl.LeftPos(10, 150).TopPos(0, 20));
	dl.DropWidth(120);
	dl.Tip("Tip!");
	w.Run();
}
