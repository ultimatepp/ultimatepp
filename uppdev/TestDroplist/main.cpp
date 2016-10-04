#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
//	GUI_PopUpEffect_Write(GUIEFFECT_NONE);
	DropList dl;
	for(int i = 0; i < 100; i++)
		dl.Add(i);
	TopWindow w;
	w.Add(dl.LeftPos(10, 150).TopPos(0, 100));
	w.Run();
}
