#include "ArrayCtrlSort.h"

class ArrayCtrlSort : public TopWindow {
	ArrayCtrl  a;
	EditDouble ed1, ed2;

public:
	typedef ArrayCtrlSort CLASSNAME;
	ArrayCtrlSort();
};

struct CvSum : public Convert {
	virtual Value Format(const Value& q) const {
		ValueArray va = q;
		return (int)va[0] + (int)va[1];
	}
};

ArrayCtrlSort::ArrayCtrlSort()
{
	a.AddColumn("first", "First").Edit(ed1).Sorting();
	a.AddColumn("second", "Second").Edit(ed2);
	a.AddColumnAt("first", "Format").Add("second").SetFormat("%d + %d = ").Sorting();
	a.AddColumnAt("first", "Sum")
	 .Add("second")
	 .SetConvert(Single<CvSum>());
	for(int i = 0; i < 100; i++)
		a.Add(rand() % 20, rand() % 10);
	Add(a.SizePos());
	a.ColumnSort(3, StdValueOrder());
}

GUI_APP_MAIN
{
	ArrayCtrlSort().Run();
}
