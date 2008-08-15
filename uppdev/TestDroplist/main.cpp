#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	DropList dl;
	for(int i = 0; i < 100; i++)
		dl.Add(i);
	TopWindow w;
	w.Add(dl.HSizePos().TopPos(0, 20));
	dl.Tip("Tip!");
	w.Run();
}
