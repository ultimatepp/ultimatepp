#include "CtrlPosTest2.h"

CtrlPosTest2::CtrlPosTest2()
{
	CtrlLayout(vis);
	SetRect(vis.GetRect()); //same size as the child
	Add(vis.SizePos());
	
	Sizeable().Zoomable();

	hk.Visit(vis); //will add itself to vis->GetParent --> to this TopWindow with same vis.GetPos to cover it perfectly.
}

GUI_APP_MAIN
{
	CtrlPosTest2().Run();
}

