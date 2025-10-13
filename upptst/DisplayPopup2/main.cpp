#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn("Test");
	list.Add("Simple");
	for(int i = 0; i < 10; i++)
		list.Add("Long " + String('X', i * 100));
	DropList dl;
	dl.Add("Simple");
	for(int i = 0; i < 10; i++)
		dl.Add("Long " + String('X', i * 100));
	TopWindow win;
	win.Add(dl.TopPosZ(0).LeftPosZ(0, 100));
	win.Add(list.VSizePosZ(Zx(20), 0).LeftPosZ(0, 100));
	win.Run();
}
