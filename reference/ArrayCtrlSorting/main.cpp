#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	ArrayCtrl list;
	list.AddColumn("Trivial").Sorting();

	list.AddColumn("With function")
	.SortingBy( // sort with sorting function
		[](int a, int b) -> int {
			int q = SgnCompare(a % 100, b % 100);
			if(q) return q;
			return SgnCompare(a, b);
		}
	)
	.SortDefault(); // set his column to be the initial sorting column;
	
	list.AddColumn("Line comparison").SortingLined( // row indices passed to predicate
		[&list](int i, int j) -> bool {
			return int(list.Get(i, 0)) + int(list.Get(i, 1)) // sort by sum of first two columns
			       < int(list.Get(i, 0)) + int(list.Get(i, 1));
		}
	);

	for(int i = 0; i < 300; i++)
		list.Add((int)Random(1000), (int)Random(10000), 0);

	TopWindow app;
	app.Add(list.SizePos());
	app.Sizeable();
	list.DoColumnSort(); // sort by current sorting column (which is "With function" column because of SortDefault
	app.Run();
}
