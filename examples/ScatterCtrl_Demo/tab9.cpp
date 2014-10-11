#include "ScatterCtrl_Demo.h"


void Tab9::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(100000,40).SetXYMin(0, -20);
	scatter.SetMouseHandling(true).SetMaxRange(500000).SetMinRange(2);
	for (int t = 0; t < 100000; ++t) {
		s1 <<Pointf(t,20*sin(2*M_PI*t/100000));
		s2 <<Pointf(t,15*cos(2*M_PI*t/100000));
	}
	scatter.AddSeries(s1).Legend("series1");
	scatter.AddSeries(s2).Legend("series2");
	scatter.ShowInfo();	
	fastView.WhenAction = THISBACK(OnFastView);
	sequentialX.WhenAction = THISBACK(OnSequentialX);
	fastView = true;
	sequentialX = true;
	OnFastView();
	OnSequentialX();
}

ScatterDemo *Construct9()
{
	static Tab9 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Big dataset", Construct9, __FILE__);
}