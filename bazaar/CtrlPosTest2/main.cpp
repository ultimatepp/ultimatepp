#include "CtrlPosTest2.h"

CtrlPosTest2::CtrlPosTest2()
{
	CtrlLayout(*this);
	Sizeable().Zoomable();

	Add(sc.SizePos());

	CtrlLayout(vis);

	sc.AddPane(vis);
	sc.WhenScrolled = callback(&hk, &CtrlPos::UpdateRefresh);
	hk.SetSource(&vis); //will add itself to vis->GetParent --> to this TopWindow with same vis.GetPos to cover it perfectly.
	hk.SetFocus();
}

void CtrlPosTest2::Activate()
{
	hk.SetFocus();	
}

GUI_APP_MAIN
{
	CtrlPosTest2().Run();
}

