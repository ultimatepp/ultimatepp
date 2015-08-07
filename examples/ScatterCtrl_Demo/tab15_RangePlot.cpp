#include "ScatterCtrl_Demo.h"


void TabRangePlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu();

	for (double time = 0; time <= 10; time += 2.) {
		Vector<double> &data = s1.Add();
		double size = time * 10.;
		data << time << ((size - 10) + Random(50)) << ((size - 4) + Random(50)) << 
						((size + 4) + Random(50)) << ((size + 10) + Random(50)); 		
	}
	idsRange << 1 << 2 << 3 << 4;
	static Vector<int> idVoid;
	scatter.AddSeries(s1, 0, Null, idVoid, idsRange, idVoid).Legend("Size").Units("m")
		   .MarkStyle<RangePlot>(RangePlot::MIN_AVG_MAX);//.NoPlot();
	
	scatter.SetLabelY("Size").SetLabelX("time [seg]");
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