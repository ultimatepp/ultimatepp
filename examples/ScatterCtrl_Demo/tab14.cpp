#include "ScatterCtrl_Demo.h"


void TabUserEquation::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	equation <<= "25 + 10*sin(0.5*x + 5)";
	fromX <<= 0;
	toX <<= 100;
	fromY <<= 0;
	toY <<= 50;
	
	update.WhenAction = THISBACK(OnUpdate);
	scatter.SetMouseHandling(true, true);
	OnUpdate();
}

void TabUserEquation::OnUpdate() 
{
	if (fromX >= toX || fromY >= toY) {
		Exclamation("Wrong limits");
		return;	
	}
	
	userEquation.Init("User equation", equation);
	
	scatter.RemoveAllSeries();
	scatter.AddSeries(userEquation).Legend(userEquation.GetFullName()).NoMark().Stroke(2);
	scatter.SetXYMin(fromX, fromY);
	scatter.SetRange(toX - fromX, toY - fromY);
	scatter.FitToData(true);
}

ScatterDemo *ConstructUserEquation()
{
	static TabUserEquation tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("User equation", ConstructUserEquation, __FILE__);
}
