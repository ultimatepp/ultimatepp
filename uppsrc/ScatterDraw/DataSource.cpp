#include "ScatterDraw.h"

NAMESPACE_UPP

#define Membercall(fun)	(this->*fun)
		
double DataSource::Min(Getdatafun getdata) 
{
	double minVal = -DOUBLE_NULL;
	for (int i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && minVal > d)
			minVal = d;
	}
	return minVal;		
}

double DataSource::Max(Getdatafun getdata) 
{
	double maxVal = DOUBLE_NULL;
	for (int i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && maxVal < d)
			maxVal = d;
	}
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