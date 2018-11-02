#include "ScatterCtrl_Demo.h"

void Tab_UserPaint::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	double a = 1, b = 0, c = 1;
	for (double x = -6; x < 6; x += 0.1)
		s1 << Pointf(x, a*exp(-sqr(x - b)/(2*c*c))); 
	scatter.AddSeries(s1).Legend("Gauss").Opacity(0.3).Fill().NoMark();
	
	scatter.ShowInfo().ShowContextMenu().ShowPropertiesDlg().ShowProcessDlg().
			SetMouseHandling(true, true);
	scatter.SetLegendPos(Point(20, 20)).SetLegendAnchor(ScatterDraw::RIGHT_BOTTOM);
	scatter.ZoomToFit(true, true);
	scatter.WhenPainter = THISBACK(OnPainter);
	scatter.WhenDraw = THISBACK(OnDraw);
}	

ScatterDemo *Construct18()
{
	static Tab_UserPaint tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("User paint", Construct18, __FILE__);
}
