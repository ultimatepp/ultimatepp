#include "ArrayCtrlActions.h"

ArrayCtrlActions::ArrayCtrlActions()
{
	CtrlLayout(*this, "Window title");
	
	action.AddColumn("Action");
	
	list.AddColumn("List");
	
	for(int i = 0; i < 4; i++)
		list.Add(i);
	
	list.Removing();
	list.Inserting();
	
	auto Log = [=](const char *s) { action.Add(String() << action.GetCount() << ": " << s); action.GoEnd(); };
	list.WhenAction << [=] { Log("WhenAction"); };
	list.WhenArrayAction << [=] { Log("WhenArrayAction"); };
	list.WhenCursor << [=] { Log("WhenCursor"); };
	list.WhenSel << [=] { Log("WhenSel"); };
}

GUI_APP_MAIN
{
	ArrayCtrlActions().Run();
}
