#include "ArrayCtrlEdit.h"

using namespace Upp;

Id ALFA("ALFA");
Id BETA("BETA");

GUI_APP_MAIN
{
	DataPusher es1;
	EditString es2;
	ArrayCtrl list;
	list.AddColumn(BETA, "huh");
	list.AddColumn(t_("fin")).Edit(es2);
	list.Appending().AppendLine();
	list.Add(123, "Hello", "kuku");
	list.Add(123, "Hello", "kuku");
	list.GoEnd();
	EditString edit;
	list.SetCtrl(0, 0, edit);
//	list.StartEdit();
	DDUMP(list.Get(0, BETA));
	TopWindow win;
	win.Add(list.SizePos());
	win.Run();
	win.Sizeable();
}

