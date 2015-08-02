#include "ScatterCtrl_Demo.h"


void Tab12_Linked::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter1.SetRange(1000, 40).SetXYMin(0, -20);
	scatter1.SetMouseHandling(true).SetMaxRange(5000).SetMinRange(2);
	scatter2.SetRange(1000, 40).SetXYMin(0, -20);
	scatter2.SetMouseHandling(true).SetMaxRange(5000).SetMinRange(2);
	scatter3.SetRange(1000, 40).SetXYMin(0, -20);
	scatter3.SetMouseHandling(true).SetMaxRange(5000).SetMinRange(2);
	for (int t = 0; t < 1000; ++t) {
		s1 << Pointf(t, 20*sin(6*M_PI*t/500));
		s2 << Pointf(t, 20*sin(6*1.1*M_PI*t/500));
		s3 << Pointf(t, 20*sin(6*1.2*M_PI*t/500));
	}
	scatter1.AddSeries(s1).Legend("Series 1").NoMark().Stroke(2, LtRed());
	scatter2.AddSeries(s2).Legend("Series 2").NoMark().Stroke(2, LtBlue());
	scatter3.AddSeries(s3).Legend("Series 3").NoMark().Stroke(2, LtGreen());
	scatter1.ShowInfo();	
	scatter2.ShowInfo();	
	scatter3.ShowInfo();	
	link = true;
	OnLink();
	link.WhenAction = THISBACK(OnLink);
}

void Tab12_Linked::OnLink() {
	if (link) {
		scatter1.SetTitle("Plots are linked together");
		scatter2.LinkedWith(scatter1);
		scatter3.LinkedWith(scatter1);
	} else {
		scatter1.SetTitle("Plots are not linked now");
		scatter2.Unlinked();
		scatter3.Unlinked();
	}
}

ScatterDemo *Construct12()
{
	static Tab12_Linked tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Linked controls", Construct12, __FILE__);
}
