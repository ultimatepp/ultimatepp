#include "ScatterCtrl_Demo.h"


void Tab7_Operations::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	scatter.SetRange(80, 100);
	
	bAddSeries 			<<= THISBACK(AddSeries);
	bRemoveLastSeries 	<<= THISBACK(RemoveLastSeries);
	bRemoveFirstSeries 	<<= THISBACK(RemoveFirstSeries);
	bRemoveAllSeries 	<<= THISBACK(RemoveAllSeries);
}

void Tab7_Operations::AddSeries()
{
	int num = scatter.GetCount();
	double f = 1 + num*1.1;	
	
	Vector<Pointf> &s = series.Add();
	s << Pointf(10, 3*f) << Pointf(30, 7*f) << Pointf(50, 12*f) << Pointf(70, 10*f);
	scatter.AddSeries(s).Legend(String("series") + AsString(num));	
} 

void Tab7_Operations::RemoveFirstSeries()	
{
	if (scatter.IsEmpty())
		return;
	scatter.RemoveSeries(0);
	series.Remove(0);
}

void Tab7_Operations::RemoveLastSeries()		
{
	if (scatter.IsEmpty())
		return;
	int topIndex = scatter.GetCount()-1;
	scatter.RemoveSeries(topIndex);
	series.Remove(topIndex);
}

void Tab7_Operations::RemoveAllSeries()		
{
	scatter.RemoveAllSeries();
	series.Clear();
}

ScatterDemo *Construct7()
{
	static Tab7_Operations tab;
	return &tab;
}

INITBLOCK {
	RegisterExample("Operations", Construct7, __FILE__);	
}