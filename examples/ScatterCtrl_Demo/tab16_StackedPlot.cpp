#include "ScatterCtrl_Demo.h"

void TabStackedPlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg();
	
	for (int x = 0; x <= 10; ++x) {
		s1 << Pointf(x, Random(50));
		s2 << Pointf(x, Random(50));
		s3 << Pointf(x, Random(50));
	}
	ds1.Init(s1);
	ds2.Init(s2);
	ds3.Init(s3);
	stacked.Add(ds1).Add(ds2).Add(ds3);
	
	scatter.AddSeries(stacked.Get(2)).Legend("s3").Fill();
	scatter.AddSeries(stacked.Get(1)).Legend("s2").Fill();
	scatter.AddSeries(stacked.Get(0)).Legend("s1").Fill();
									
	scatter.SetLabelY("Data").SetLabelX("time [seg]")
		   .ShowButtons().Responsive();
	
	type = 0;
	type.WhenAction = THISBACK(OnPlot);
	
	OnPlot();
}

void TabStackedPlot::OnPlot()
{
	stacked.Set100(type == 1);
	scatter.ZoomToFit(true, type != 1, 0.2);
	if (type == 1) {
		scatter.SetXYMin(-2, -10);
		scatter.SetRange(14, 120);
		scatter.SetMajorUnits(2, 10);
		scatter.SetMinUnits(-2, -10);
	} else {
		scatter.SetXYMin(-2, Null);
		scatter.SetRange(14, Null);
		scatter.SetMajorUnits(2, Null);
		scatter.SetMinUnits(-2, Null);
	}
}
	
ScatterDemo *Construct16()
{
	static TabStackedPlot tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("StackedPlot", Construct16, __FILE__);
}