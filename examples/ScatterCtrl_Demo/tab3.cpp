#include "ScatterCtrl_Demo.h"


void funct1p(double& y, double x, double a, double c)	{y = a*(x*x)+c;}
void funct3p(double& y, double x, double m)				{y = m*x;}

Pointf opara1(double t) {return Pointf(0.5*cos(2*M_PI*t), 0.5*sin(2*M_PI*t));}

void para1(Pointf& xy, double t) {xy = Pointf(cos(2*M_PI*t), sin(2*M_PI*t));}
void para2(Pointf& xy, double t) {xy = Pointf(0.5*cos(6*M_PI*t)+3*t-1, 1.5+0.5*sin(6*M_PI*t));}
void para3(Pointf& xy, double t) {xy = Pointf(3+(0.5+t)*cos(6*M_PI*t), (0.5+t)*sin(6*M_PI*t));}
void para4(Pointf& xy, double t) {xy = Pointf(0,-0.25+0.5*t);}
void para5(Pointf& xy, double t) {xy = Pointf(-0.25+0.5*t, 0);}

void para1p(Pointf& xy, double t, double a, double b) 	{xy = Pointf(a*cos(2*M_PI*t),b*sin(2*M_PI*t));}
void para3p(Pointf& xy, double t, double a) 			{xy = Pointf(a+(0.5+t)*cos(6*M_PI*t),(0.5+t)*sin(6*M_PI*t));}

void Tab3::Init()
{
	CtrlLayout(*this);	
	HSizePos().VSizePos();
	scatter.SetRange(7, 4);
	scatter.SetMajorUnits(0.5, 0.5);
	scatter.SetXYMin(-2, -2);
	
	scatter.AddSeries(&opara1, 50).Legend("Circle").NoMark();
	scatter.AddSeries(STDBACK(para1), 100).Legend("Circle").NoMark();
	scatter.AddSeries(STDBACK(para2), 100).Legend("Coil").NoMark();
	scatter.AddSeries(STDBACK(para3), 150).Legend("Spiral").NoMark();
	scatter.AddSeries(STDBACK1(para3p, 2.5), 150, 0, 1).Legend("Spiral").NoMark();
	scatter.AddSeries(STDBACK(para4), 2).Legend("VLine").NoMark();
	scatter.AddSeries(STDBACK(para5), 2).Legend("HLine").NoMark();
	scatter.AddSeries(STDBACK2(para1p, 4.0, 0.25), 50, 0, 1).Legend("Circle2").NoMark();
	scatter.AddSeries(STDBACK2(para1p, 2.0, 0.5), 50, 0, 1).Legend("Circle2").NoMark();
}

ScatterDemo *Construct3()
{
	static Tab3 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample(t_("Parametric Functions"), Construct3, __FILE__);
}