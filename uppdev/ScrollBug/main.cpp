#include "ScrollBug.h"

ScrollBug::ScrollBug()
{
	CtrlLayout(*this, "Window title");

	tab.Add(edit.SizePos(), "Edit");
	tab.Add("Empty");

	for(int i = 0; i < 16000; i++)
		edit.Insert(0, Format("Line %d\n", 16000 - i));

	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	Ctrl::ShowRepaint = 30;
	ScrollBug().Run();
}
