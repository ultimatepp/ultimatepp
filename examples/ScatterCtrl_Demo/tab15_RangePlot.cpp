#include "ScatterCtrl_Demo.h"


void TabRangePlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu();

	for (double size = 0; size <= 10; size += 2.) {
		Vector<double> &data = s1.Add();
		double val = size * 10.;
		data << size << val << (val - 10) << (val - 4) << (val + 4) << (val + 10); 		
	}
	idsRange << 2 << 3 << 4 << 5;
	static Vector<int> idVoid;
	scatter.AddSeries(s1, 1, 0, idsRange, idVoid, idVoid).Legend("Time").Units("sec")
		   .MarkStyle<RangePlot>(RangePlot::MIN_AVG_MAX).NoPlot();
	
	scatter.SetLabelY("Size");
	scatter.FitToData(true, 0.2);
}

ScatterDemo *Construct15()
{
	static TabRangePlot tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("RangePlot", Construct15, __FILE__);
}