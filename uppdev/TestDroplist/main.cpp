#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	DropList dl;
	for(int i = 0; i < 100; i++)
		dl.Add(i);
	dl <<= 20;
//	dl.SetReadOnly();
//	dl.DropWidth(120);
	TopWindow w;
	StaticRect sr;
	sr.Color(LtCyan);
	w.Add(sr.SizePos());
	w.Add(dl.LeftPos(10, 150).TopPos(0, 20));
	dl.Tip("Tip!");
//	dl.NoBackground();
	w.Run();
}
