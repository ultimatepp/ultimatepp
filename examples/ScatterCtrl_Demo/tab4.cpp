#include "ScatterCtrl_Demo.h"


void Tab4::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(13, 50)
		   .SetMajorUnits(1, 10)
		   .SetXYMin(0, -20);
	scatter.SetMouseHandling(true).ShowContextMenu();
	
	s1 <<Pointf(1,-6)<<Pointf(2,-4)<<Pointf(3,2)<<Pointf(4,8)<<Pointf(5,8)<<Pointf(6,15)<<Pointf(7,20)<<Pointf(8,25)<<Pointf(9,14)<<Pointf(10,10)<<Pointf(11,3)<<Pointf(12,-11);
	scatter.AddSeries(s1).Legend("2005").Units("ºC");
	
	s2 <<Pointf(1,-12)<<Pointf(2,-7)<<Pointf(3,0)<<Pointf(4,2)<<Pointf(5,10)<<Pointf(6,18)<<Pointf(7,25)<<Pointf(8,26)<<Pointf(9,15)<<Pointf(10,12)<<Pointf(11,7)<<Pointf(12,-6);
	scatter.AddSeries(s2).Legend("2006").Units("ºC").MarkStyle<CircleMarkPlot>();
	
	scatter.Graduation_FormatX(ScatterCtrl::MON)
		   .SetLabelY("Temperature")
		   .SetMarkWidth(1, 4)
		   .SetMinXmin(0).SetMaxXmax(13);
}

ScatterDemo *Construct4()
{
	static Tab4 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Formatting", Construct4, __FILE__);
}