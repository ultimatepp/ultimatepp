#include "ScatterDraw.h"

NAMESPACE_UPP

#define Membercall(fun)	(this->*fun)
		
double DataSource::Min(Getdatafun getdata) 
{
	double minVal = -DOUBLE_NULL;
	for (int i = 0; i < GetCount(); ++i)
		if (minVal > Membercall(getdata)(i))
			minVal = Membercall(getdata)(i);
	return minVal;		
}

double DataSource::Max(Getdatafun getdata) 
{
	double maxVal = DOUBLE_NULL;
	for (int i = 0; i < GetCount(); ++i)
		if (maxVal < Membercall(getdata)(i))
			maxVal = Membercall(getdata)(i);
	return maxVal;
}

double DataSource::Avg(Getdatafun getdata) 
{
	double ret = 0;
	for (int i = 0; i < GetCount(); ++i)
		ret += Membercall(getdata)(i);
	return ret/GetCount();
}

END_UPP_NAMESPACE