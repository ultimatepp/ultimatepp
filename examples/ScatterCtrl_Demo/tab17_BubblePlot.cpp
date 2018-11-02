#include "ScatterCtrl_Demo.h"


void TabBubblePlot::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetMouseHandling(true, true).ShowContextMenu();

	for (double size = 0; size <= 10; size += 2.) {
		Vector<double> &data = s1.Add();
		double x = size;
		double y = (10 + 10*Random(30)*size);
		double diameter = Random(80);
		data << x << y << diameter;
		labels << Format("%.0f cm", diameter); 		
	}
	idsBubble << 2;
	static Vector<int> idVoid;
	scatter.AddSeries(s1, 0, 1, idVoid, idVoid, idsBubble).Legend("Importance").MarkStyle<BubblePlot>()
		   .MarkColor(Green()).MarkBorderColor(LtRed()).MarkBorderWidth(3)
		   .AddLabelSeries(labels, 0, 0, StdFont().Height(15).Bold(), ALIGN_CENTER);
		   
	scatter.SetLabelY("Size").Responsive();
	scatter.ZoomToFit(true, true, 0.2);
}

ScatterDemo *Construct17()
{
	static TabBubblePlot tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("BubblePlot", Construct17, __FILE__);
}