#include "DoubleClick.h"

DoubleClick::DoubleClick()
{
	CtrlLayout(*this, "Window title");

	array.AddColumn("ID");
	array.AddColumn("Name");
	array.Set(0, 0, 1);
	array.Set(0, 1, "Daniel");
	array.Set(1, 0, 2);
	array.Set(1, 1, "Michael");
	array.Set(2, 0, 3);
	array.Set(2, 1, "John");
	array.Set(3, 0, 4);
	array.Set(3, 1, "Matthew");
	array.SetLineCy(1, 40);

	array.WhenLeftDouble = THISBACK(LeftDouble);
	array.WhenSelection = THISBACK(ShowSelected);
	array.Removing();

	show <<= THISBACK(ShowSelected);
}

void DoubleClick::LeftDouble()
{
	ShowSelected();
}

void DoubleClick::ShowSelected()
{
	String txt;
	if(array.GetSelectCount() == 0)
		txt = "No selection";
	else
		for(int i = 0; i < array.GetCount(); i++)
			if(array.IsSelected(i))
				txt << Format("%d - %s\n", array.Get(i, 0), array.Get(i, 1));
	edit <<= txt;
}

GUI_APP_MAIN
{
	DoubleClick().Run();
}
