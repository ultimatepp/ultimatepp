#include "ScrollBug.h"

ScrollBug::ScrollBug()
{
	CtrlLayout(*this, "Window title");
	list.AddColumn("Test");
	for(int i = 0; i < 1000; i++)
		list.Add(i);
}

GUI_APP_MAIN
{
	Ctrl::ShowRepaint(30);
	ScrollBug().Run();
}
