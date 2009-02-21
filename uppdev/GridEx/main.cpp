#include "GridEx.h"



GridEx::GridEx()
{
	CtrlLayout(*this, "Window title");
	grid.SetToolBar();
	grid.Searching();
	grid.AddColumn("Grid Test With One Big Column -------------------------------");
	grid.Inserting().Removing().Chameleon();
	grid.Appending().Editing();
}

GUI_APP_MAIN
{
	GridEx().Run();
}

