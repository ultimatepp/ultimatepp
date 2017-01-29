#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	
	list.AddColumn("Column1").Sorting(); // default sorting
	list.AddColumn("Column2") // we provide compare function that sorts based on string length
		.Sorting([](const Value& a, const Value& b) -> int {
			return SgnCompare(AsString(a).GetCount(), AsString(b).GetCount());
		});
	
	list.Add("Not sorted");

	for(int i = 0; i < 100; i++)
		list.Add((int)Random(), (int)Random(), (int)Random());
	
	list.SortingFrom(1);
	
	TopWindow win;
	win.Add(list.SizePos());
	
	win.Run();
}
