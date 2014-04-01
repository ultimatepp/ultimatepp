#include "ScatterCtrl_Demo.h"

void Tab12::Init()
{
	CtrlLayout(*this);	
	HSizePos().VSizePos();
	
	s1 << Pointf(50, 10) << Pointf(100, 20) << Pointf(150, 30) << Pointf(200, 20) << Pointf(250, 10);
	scatter.AddSeries(s1).Legend("Series 1").Fill();
	
	scatter.SetXYMin(50, 0).SetRange(200, 40).SetMajorUnits(50, 10).SetPolar();
	scatter.ShowInfo().ShowContextMenu().ShowPropertiesDlg().SetPopText("h", "v", "v2").SetMouseHandling(true, true);
}

ScatterDemo *Construct12()
{
	static Tab12 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Polar plot", Construct12, __FILE__);
}
