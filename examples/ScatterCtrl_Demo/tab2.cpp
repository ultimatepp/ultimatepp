#include "ScatterCtrl_Demo.h"


double funct1(double x)	{return 3;}
double funct2(double x)	{return (x*x-5);}
double funct3(double x)	{return (-x*x-5);}

void   vfunct1(double& y, double x) {y = 0;}
void   vfunct2(double& y, double x) {y = x*x;}
void   vfunct3(double& y, double x) {y = -x*x;}

void Tab2::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(12, 60).SetXYMin(-6, -30).SetMajorUnits(2, 10);
	scatter.SetMouseHandling(true);

	scatter.AddSeries(&funct1).Legend("y = 3").NoMark();
	scatter.AddSeries(&funct2).Legend("y = x^2-5").NoMark();
	scatter.AddSeries(&funct3).Legend("y = -x^2-5").NoMark();
	
	scatter.AddSeries(&vfunct1).Legend("y = 0").NoMark();
	scatter.AddSeries(&vfunct2).Legend("y = x^2").NoMark();
	scatter.AddSeries(&vfunct3).Legend("y = -x^2").NoMark();
}

ScatterDemo *Construct2()
{
	static Tab2 tab;
	return &tab;
}

INITBLOCK {	
	RegisterExample("Explicit Functions", Construct2, __FILE__);
}