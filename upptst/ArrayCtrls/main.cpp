#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl ac;
	ac.AddColumn("Test").Ctrls<EditString>();
	
	for(int i = 0; i < 10; i++)
		ac.Add(i);

	ac.ShowLine(5, true);
	ac.ShowLine(5, true);

	TopWindow win;
	win << ac.SizePos();
	win.Run();
}
