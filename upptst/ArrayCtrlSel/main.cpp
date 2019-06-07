#include "ArrayCtrlSel.h"

ArrayCtrlSel::ArrayCtrlSel()
{
	CtrlLayout(*this, "Window title");
	list.MultiSelect();
	list.AddColumn("Column");
	for(int i = 0; i < 10; i++)
		list.Add(i);
	list.WhenSel = [=] {
		counter.SetLabel(AsString(serial++));
		String ssel, sselected;
		for(int i = 0; i < list.GetCount(); i++) {
			if(list.IsSel(i))
				MergeWith(ssel, ", ", AsString(i));
			if(list.IsSelected(i))
				MergeWith(sselected, ", ", AsString(i));
		}
		sel.SetLabel(ssel);
		selected.SetLabel(sselected);
	};
}

GUI_APP_MAIN
{
	ArrayCtrlSel().Run();
}
