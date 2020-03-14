#include "ScatterCtrl_Demo.h"


void Tab6_Logarithmic::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(6,100);
	scatter.SetMajorUnits(1,20);
	scatter.ShowInfo();
	scatter.SetMouseHandling(false, false);
	
	s1 <<Pointf(log10(10.0),14)<<Pointf(log10(1e2),25)<<Pointf(log10(1e3),39)<<Pointf(log10(1e4),44)<<Pointf(log10(1e5),76);
	scatter.AddSeries(s1).Legend("series1");
	scatter.cbModifFormatX = THISBACK(FormatX);
	scatter.cbModifFormatXGridUnits = THISBACK(FormatXGridUnits);
	scatter.SetGridLinesX = THISBACK(SetGridLinesX);
}

void Tab6_Logarithmic::FormatX(String& s, int , double d)
{
	double val = pow(10, d);
	if (val >= 1 && val <= 1e5)
		s = FormatDoubleExp(val, 0);
}

void Tab6_Logarithmic::FormatXGridUnits(String& s, int , double d)
{
	double val = pow(10, d);
	int valint = fround(val);
	if (abs(val - double(valint)) < 0.0001) {
		while (valint < -9 || valint > 9) 
			valint /= 10;
		if (valint == 1)
			s = FormatDoubleExp(pow(10, d), 0);
	}
}

void Tab6_Logarithmic::SetGridLinesX(Vector<double>& unitsX)
{
	for(int i = 0; scatter.GetXMinUnit() + i*scatter.GetMajorUnitsX() <= scatter.GetXRange(); i++) {
		for (int ii = 0; ii < 9; ++ii)
			unitsX << scatter.GetXMinUnit() + i*scatter.GetMajorUnitsX() + (log10((ii+1)*10)-1)*scatter.GetMajorUnitsX();
	}
}

ScatterDemo *Construct6()
{
	static Tab6_Logarithmic tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Log", Construct6, __FILE__);
}