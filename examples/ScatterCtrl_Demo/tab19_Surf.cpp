#include "ScatterCtrl_Demo.h"


void Tab19_Surf::Init()
{
	CtrlLayout(*this);	
	SizePos();

	numColor <<= 4;
	numColor <<= THISBACK(OnChange);
	opContinuous <<= true;
	opContinuous <<= THISBACK(OnChange);
	dataType = 0;//2;
	dataType << THISBACK(OnChange);
	rainbow.Add(BLUE_YELLOW_RED, "BLUE_YELLOW_RED").Add(RED_YELLOW_BLUE, "RED_YELLOW_BLUE")
		   .Add(GREEN_YELLOW_RED, "GREEN_YELLOW_RED").Add(RED_YELLOW_GREEN, "RED_YELLOW_GREEN")
		   .Add(WHITE_BLACK, "WHITE_BLACK").Add(BLACK_WHITE, "BLACK_WHITE");
	rainbow = scatter.SurfRainbow();
	rainbow << THISBACK(OnChange);
	interpolation.Add(TableInterpolate::NO, "NO").Add(TableInterpolate::BILINEAR, "BILINEAR");
	interpolation = TableInterpolate::BILINEAR;
	interpolation << THISBACK(OnChange);
	zoom = true;
	zoom << THISBACK(OnChange);
		
	scatter.ShowInfo().ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg()
		   .SetMouseHandling(true, true).Responsive(true)
		   .SetRainbowPaletteSize(Size(20, 200))
		   .SetRainbowPaletteAnchor(ScatterDraw::LEGEND_POS::LEFT_TOP)
		   .SetRainbowPalettePos(Point(20, 20))
		   .SetRainbowPaletteFont(StdFont().Height(10))
		   .SurfUnits("kg").SurfUnitsPos(ScatterDraw::UNITS_LEFT)
		   .SurfLegendPos(ScatterDraw::LEGEND_RIGHT);
	
	for (double x = 0; x <= 5; x += 1)	   
		xAxis << x;
	yAxis  << 0 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15;
	zData 	<< 0 << 0	 << 0	  << 0		<< 0	  << 0
			<< 0 << 402	 << 2501  << 2099	<< 48231  << 89315
			<< 0 << 1835 << 9889  << 36124	<< 78603  << 101067
			<< 0 << 4627 << 24211 << 61689	<< 117905 << 151600
			<< 0 << 7170 << 37887 << 96988	<< 185785 << 239028
			<< 0 << 7144 << 39134 << 106627	<< 213241 << 277208
			<< 0 << 6290 << 33302 << 95397	<< 196494 << 256218
			<< 0 << 6931 << 32175 << 84656	<< 165247 << 211670
			<< 0 << 6117 << 27070 << 68657	<< 130224 << 165038
			<< 0 << 3849 << 17985 << 47400	<< 91425  << 116321
			<< 0 << 2390 << 12142 << 33214	<< 65323  << 83722
			<< 0 << 1741 << 9541  << 26098	<< 51917  << 67241
			<< 0 << 1337 << 7783  << 21287 	<< 42703  << 55722;		
	
	data.Init(zData, xAxis, yAxis, TableInterpolate::BILINEAR, false);
	
	Vector<double> vals;
	for (double val = 50000; val <= 250000; val += 50000)
		vals << val;
	isolines = data.GetIsolines(vals, Rectf(0, 16, 5, 0), 0.1, 0.1);	
	
	Vector<Pointf> numbersWhere;
	numbersWhere << Pointf(4.7, 8) << Pointf(1, 14);
	numbersPos = Intersection(numbersWhere, isolines);
	for (int i = 0; i < vals.GetCount(); ++i)
		labels << Format("%.0f kg", vals[i]);
		
	for (double x = -10; x <= 10; x += 1)
		xAxisFun << x;
	for (double y = -10; y <= 10; y += 1)
		yAxisFun << y;
	
	zDataFun.SetCount((xAxisFun.GetCount()-1)*(yAxisFun.GetCount()-1));
	for (int iy = 0; iy < yAxisFun.GetCount()-1; iy++) {
		double y = (yAxisFun[iy] + yAxisFun[iy+1])/2.;
		for (int ix = 0; ix < xAxisFun.GetCount()-1; ix++) {
			double x = (xAxisFun[ix] + xAxisFun[ix+1])/2.;
			zDataFun[ix + iy*(xAxisFun.GetCount() - 1)] = sin(sqrt(x*x + y*y + 64))/sqrt(x*x + y*y + 64);
		}
	}	
	dataFun.Init(zDataFun, xAxisFun, yAxisFun, TableInterpolate::BILINEAR, true);
	
	funData.Init([=](double x, double y) {return sin(sqrt(x*x + y*y + 64))/sqrt(x*x + y*y + 64);}, -10, 10, -10, 10);
	
	OnChange();
}

void Tab19_Surf::OnChange() {
	labelInterpolation.Enable(dataType != 2);
	interpolation.Enable(dataType != 2);
	
	scatter.SurfNumColor(~numColor, ~opContinuous).SurfRainbow((RAINBOW)int(rainbow.GetKey(rainbow.GetIndex())));
	if (dataType == 0)
		Init_DatasetSimple();
	else if (dataType == 1)
		Init_Dataset();
	else
		Init_DataExplicit();
	if (zoom)
		scatter.ZoomToFit(true, true);
	
	scatter.Refresh();
}

void Tab19_Surf::Init_DatasetSimple() {
	scatter.RemoveAllSeries();
	scatter.AddSeries(isolines).Legend("Isoline").Stroke(0.5, White).ShowSeriesLegend(false).NoMark();
	scatter.AddSeries(numbersPos).Stroke(0).AddLabelSeries(labels, 0, 0, StdFont().Bold(), ALIGN_CENTER, White())
								 .ShowSeriesLegend(false).NoMark();
	data.Inter((TableInterpolate::Interpolate)int(interpolation.GetKey(interpolation.GetIndex())));
	scatter.AddSurf(data)
		   .SetRainbowPaletteTextColor(White)
		   .SetSurfMinZ(0).SetSurfMaxZ(300000);
}

void Tab19_Surf::Init_Dataset() {
	scatter.RemoveAllSeries();
	dataFun.Inter((TableInterpolate::Interpolate)int(interpolation.GetKey(interpolation.GetIndex())));
	scatter.AddSurf(dataFun);
	scatter.SetRainbowPaletteTextColor(Black);
	scatter.ZoomToFitZ();
}

void Tab19_Surf::Init_DataExplicit() {
	scatter.RemoveAllSeries();
	scatter.AddSurf(funData);	
	scatter.SetRainbowPaletteTextColor(Black);
	scatter.ZoomToFitZ();
}

ScatterDemo *Construct19()
{
	static Tab19_Surf tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("2D Surface", Construct19, __FILE__);
}