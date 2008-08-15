#include "ArrayCtrlEdit.h"

using namespace Upp;

GUI_APP_MAIN
{
	EditString es1, es2;
	ArrayCtrl list;
	list.AddColumn(t_("hun")).Edit(es1);
	list.AddColumn(t_("fin")).Edit(es2);
	list.Appending().AppendLine();
	list.AutoAppending();
	list.DoAppend();
	TopWindow win;
	win.Add(list.SizePos());
	win.Run();
	win.Sizeable();
}
