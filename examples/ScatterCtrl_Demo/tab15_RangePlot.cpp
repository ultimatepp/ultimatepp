#include "ScatterCtrl_Demo.h"

void TabRangePlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg();

	for (double time = 0; time <= 10; time += 2.) {
		Vector<double> &data = s1.Add();
		double size = time * 10.;
		data << time << ((size - 10) + Random(50)) << ((size - 4) + Random(50)) << 
						((size + 4) + Random(50)) << ((size + 10) + Random(50)); 		
	}
	idsRange << 1 << 2 << 3 << 4;
	static Vector<int> idVoid;
	scatter.AddSeries(s1, 0, Null, idVoid, idsRange, idVoid).Legend("Size").Units("m")
		   .MarkStyle<RangePlot>(RangePlot::MIN_AVG_MAX);
	
	sdata << Pointf(0, 10) << Pointf(1, 14) << Pointf(2, 20) << Pointf(3, 30) << Pointf(4, 23) << Pointf(5, 25)
		   << Pointf(6, 3) << Pointf(7, 21) << Pointf(8, 37) << Pointf(9, 32) << Pointf(10, 28);
	for (int i = 0; i < sdata.GetCount(); ++i) {
		smax << Pointf(sdata[i].x, sdata[i].y + 5 + Random(10));
		smin << Pointf(sdata[i].x, sdata[i].y - 5 - Random(10));
	}
	dmax.Init(smax);
	dmin.Init(smin);
	range.Init(dmax, dmin);
	scatter.AddSeries(range).Legend("Range").Closed(true).Stroke(0).Fill(LtCyan()).Opacity(0.5).NoMark();
	scatter.AddSeries(smax).Legend("Max").Stroke(1, Blue()).NoMark();
	scatter.AddSeries(smin).Legend("Min").Stroke(1, Blue()).NoMark();
	scatter.AddSeries(sdata).Legend("Data").Stroke(4, Blue()).NoMark();
									
	scatter.SetLabelY("Size").SetLabelX("time [seg]");
	scatter.ShowButtons();
	scatter.ZoomToFit(true, true, 0.2);
}

ScatterDemo *Construct15()
{
	static TabRangePlot tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("RangePlot", Construct15, __FILE__);
}