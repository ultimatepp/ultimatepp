#include "ScatterCtrl_Demo.h"


Pointf opara1(double t) {return Pointf(0.5*cos(2*M_PI*t), 0.5*sin(2*M_PI*t));}

void para1(Pointf& xy, double t) {xy = Pointf(cos(2*M_PI*t), sin(2*M_PI*t));}
void para2(Pointf& xy, double t) {xy = Pointf(0.5*cos(6*M_PI*t)+3*t-1, 1.5+0.5*sin(6*M_PI*t));}
void para3(Pointf& xy, double t) {xy = Pointf(3+(0.5+t)*cos(6*M_PI*t), (0.5+t)*sin(6*M_PI*t));}
void para4(Pointf& xy, double t) {xy = Pointf(0, -0.25+0.5*t);}
void para5(Pointf& xy, double t) {xy = Pointf(-0.25+0.5*t, 0);}

void para1p(Pointf& xy, double t, double a, double b) 	{xy = Pointf(a*cos(2*M_PI*t),b*sin(2*M_PI*t));}
void para3p(Pointf& xy, double t, double a) 			{xy = Pointf(a+(0.5+t)*cos(6*M_PI*t),(0.5+t)*sin(6*M_PI*t));}

void Tab3_ParametricFunctions::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(7, 4);
	scatter.SetMajorUnits(0.5, 0.5);
	scatter.SetXYMin(-2, -2);
	
	scatter.AddSeries(&opara1, 20).Legend("Circle 1");//.NoMark();
	scatter.AddSeries(STDBACK(para1), 100).Legend("Circle 2").NoMark();
	scatter.AddSeries(STDBACK(para2), 100).Legend("Coil").NoMark();
	scatter.AddSeries(STDBACK(para3), 150).Legend("Spiral 1").NoMark();
	scatter.AddSeries(STDBACK1(para3p, 2.5), 150, 0, 1).Legend("Spiral 2").NoMark();
	scatter.AddSeries(STDBACK(para4), 2).Legend("VLine").NoMark();
	scatter.AddSeries(STDBACK(para5), 2).Legend("HLine").NoMark();
	scatter.AddSeries(STDBACK2(para1p, 4.0, 0.25), 50, 0, 1).Legend("Ellipse 1").NoMark();
	scatter.AddSeries(STDBACK2(para1p, 2.0, 0.5), 50, 0, 1).Legend("Ellipse 2").NoMark();
}

ScatterDemo *Construct3()
{
	static Tab3_ParametricFunctions tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Parametric Functions", Construct3, __FILE__);
}