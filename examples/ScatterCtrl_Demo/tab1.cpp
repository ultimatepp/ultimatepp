#include "ScatterCtrl_Demo.h"

enum {
	SERIE2
};

void Tab1::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	s1 << Pointf(10, 26) << Pointf(20, 37) << Pointf(30, 31) << Pointf(40, 33) << Pointf(50, 28);
	scatter.AddSeries(s1).Legend("Series 1").Opacity(0.3).Fill().MarkBorderColor();
	
	s2y[0] = 22; s2y[1] = 33; s2y[2] = 27; s2y[3] = 29; s2y[4] = 24;
	scatter.AddSeries(s2y, 5, 10, 10).Legend("Series 2").Id(SERIE2).PlotStyle<LineSeriesPlot>()
					.Dash("").MarkColor(Red()).Fill().Opacity(0.5).MarkBorderColor();
	
	s3y[0] = 18; s3y[1] = 29; s3y[2] = 23; s3y[3] = 25; s3y[4] = 20;
	s3x[0] = 10; s3x[1] = 20; s3x[2] = 30; s3x[3] = 40; s3x[4] = 50;
	scatter.AddSeries(s3x, s3y, 5).Legend("Series 3").MarkBorderColor();
	
	s4y << 14 << 25 << 19 << 21 << 16;
	s4x << 10 << 20 << 30 << 40 << 50;
	scatter.AddSeries(s4x, s4y).Legend("Series 4").PlotStyle<StaggeredSeriesPlot>().Dash("").NoMark().Fill().Stroke(3, LtRed());
	
	s5.Add(10, 10);
	s5.Add(20, 21);
	s5.Add(30, 15);
	s5.Add(40, 16);
	s5.Add(50, 12);
	int barWidth = 4;
	scatter.AddSeries(s5).Legend("Series 5").PlotStyle<BarSeriesPlot>(barWidth).Dash("").NoMark().Fill();
	
	scatter.SetRange(60, 50).SetMajorUnits(10, 10);
	scatter.ShowInfo().ShowContextMenu().ShowPropertiesDlg().SetPopText("h", "v", "v2").SetMouseHandling(true, true);
	scatter.SetLegendPos(Point(20, 20));
}

ScatterDemo *Construct1()
{
	static Tab1 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Basic", Construct1, __FILE__);
}
