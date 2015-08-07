#include "ScatterCtrl_Demo.h"


void TabBubblePlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu();

	for (double size = 0; size <= 10; size += 2.) {
		Vector<double> &data = s1.Add();
		data << size << (10 + 10*Random(30)*size) << Random(80); 		
	}
	idsBubble << 2;
	static Vector<int> idVoid;
	scatter.AddSeries(s1, 0, 1, idVoid, idVoid, idsBubble).Legend("Importance").MarkStyle<BubblePlot>()
		   .NoPlot().MarkColor(Green()).MarkBorderColor(LtRed()).MarkBorderWidth(3);
	
	scatter.SetLabelY("Size");
	scatter.FitToData(true, 0.2);
}

ScatterDemo *Construct16()
{
	static TabBubblePlot tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("BubblePlot", Construct16, __FILE__);
}