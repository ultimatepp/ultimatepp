#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn("int").Sorting();
	list.AddColumn("int64").Sorting();
	list.AddColumn("double").Sorting();
	list.AddColumn("String").Sorting();
	list.AddColumn("Date").Sorting();
	list.AddColumn("Time").Sorting();

#ifdef _DEBUG
	for(int i = 0; i < 2000; i++)
#else
	for(int i = 0; i < 200000; i++)
#endif
		list.Add((int)Random(), (int64)Random64(), Randomf(), AsString(Random()),
		         GetSysTime() + (int64)Random(100000), GetSysDate() + Random(10000));
	
	TopWindow win;
	win.Sizeable().Zoomable();
	win.Add(list.SizePos());
	win.Run();
}
