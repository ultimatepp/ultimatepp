#include "GridCtrlTest.h"

void MakeDropList(One<Ctrl>& ctrl)
{
	ctrl.Create<DropList>()
	   .Add(Null, "")
	   .Add(0, "None")
	   .Add(1, "Minimal")
	   .Add(2, "Full");	   
}

void MakeButton(One<Ctrl>& ctrl)
{
	ctrl.Create<Button>();
	ctrl->WantFocus();
}

void MakeCheck(One<Ctrl>& ctrl)
{
	ctrl.Create<Option>();
	ctrl->WantFocus();
}

void MakeEdit(One<Ctrl>& ctrl)
{
	ctrl.Create<EditInt>();
	ctrl->WantFocus();
}

void TestArrayCtrl::Init()
{
	arr.HeaderTab(0).SetRatio(10);
	arr.HeaderObject().Absolute();
	arr.MultiSelect();
	arr.Inserting();
	arr.SetLineCy(DPI(20));

	const int total_cols = 15;
	const int total_rows = 10;
	
	for(int i = 0; i < total_cols; i++)
	{
		String name = Format("Column %d", i + 1);
		int size = rand() % 40 + 50;
		arr.AddColumn((const char *)name, size);
	}
	
	arr.SetCount(50);

	arr.ColumnAt(1).Ctrls(MakeDropList);
	arr.ColumnAt(2).Ctrls(MakeButton);
	arr.ColumnAt(3).Ctrls(MakeEdit);
	arr.ColumnAt(1).Edit(e0);
	arr.ColumnAt(2).Edit(e1);
	
	Add(arr.SizePos());
}
