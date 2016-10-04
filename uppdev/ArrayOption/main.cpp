#include <CtrlLib/CtrlLib.h>

GUI_APP_MAIN
{
	EditInt e;
	ArrayCtrl ctrl;
	ArrayOption option;
	option.AddColumn(ctrl, "Test");
	ctrl.AddColumn("Test2").Edit(e);
	for(int i = 0; i < 21; i++)
		ctrl.Add(i & 1, 23 * i);
	TopWindow w;
	w.Add(ctrl.SizePos());
	w.Run();
}
