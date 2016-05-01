#include "ScatterCtrl_Demo.h"


void Tab8_Secondary::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(13, 50, 20).SetMajorUnits(1, 10).SetXYMin(0,-20, 1000);
	scatter.SetMouseHandling(true, true);

	s3 <<Pointf(1,-12)<<Pointf(2,-17)<<Pointf(3,-13)<<Pointf(4,15)<<Pointf(5,18)<<Pointf(6,25)<<Pointf(7,27)<<Pointf(8,20)<<Pointf(9,5)<<Pointf(10,-10)<<Pointf(11,24)<<Pointf(12,17);
	double barWidth = 0.4;
	scatter.AddSeries(s3).Legend("Speed").Units("km/h").PlotStyle<BarSeriesPlot>(barWidth).NoMark().Stroke(2, Gray()).Dash(LINE_SOLID).Fill(LtGray());
	
	s1<<Pointf(1,-6)<<Pointf(2,-4)<<Pointf(3,2)<<Pointf(4,8)<<Pointf(5,8)<<Pointf(6,15)<<Pointf(7,20)<<Pointf(8,30)<<Pointf(9,14)<<Pointf(10,10)<<Pointf(11,3)<<Pointf(12,-11);
	scatter.AddSeries(s1).Legend("Temperature").Units("ºC").Stroke(3, Null).Dash(LINE_DASHED).MarkStyle<SquareMarkPlot>();
	
	s2 <<Pointf(1,1008)<<Pointf(2,1012)<<Pointf(3,1016)<<Pointf(4,1012)<<Pointf(5,1008)<<Pointf(6,1016)<<Pointf(7,1012)<<Pointf(8,1004)<<Pointf(9,1000)<<Pointf(10,1001)<<Pointf(11,1000)<<Pointf(12,1002);
	scatter.AddSeries(s2).Legend("Pressure").Units("mBar").Stroke(3, Null).Dash("15 6 6 6 3 6 6 6").SetDataPrimaryY(false);
	scatter.SetDrawY2Reticle();
	
	scatter.cbModifFormatX = THISBACK(MyFormat);
	scatter.cbModifFormatDeltaX = THISBACK(MyFormatDelta);
	scatter.SetMaxRange(40).SetMinRange(2, 20)
		   .SetLabelY("Temperature")
	       .SetLabelY2("Pressure")
		   .SetMarkWidth(1, 4);
	scatter.ShowInfo()
		   .ShowContextMenu();	
}

ScatterDemo *Construct8()
{
	static Tab8_Secondary tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Secondary Y", Construct8, __FILE__);
}