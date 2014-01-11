#include "ScatterDraw.h"

NAMESPACE_UPP

#define Membercall(fun)	(this->*fun)
		
double DataSource::Min(Getdatafun getdata) {
	double minVal = -DOUBLE_NULL;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && minVal > d)
			minVal = d;
	}
	return minVal;		
}
 
double DataSource::Max(Getdatafun getdata) {
	double maxVal = DOUBLE_NULL;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && maxVal < d)
			maxVal = d;
	}
	return maxVal;
}

double DataSource::Avg(Getdatafun getdata) {
	double ret = 0;
	for (int64 i = 0; i < GetCount(); ++i)
		ret += Membercall(getdata)(i);
	return ret/GetCount();
}

double CArray::xn(int n, int64 id) {
	switch (n) {
	case 0:		ASSERT(yData);	
				return yData[id];
	case 1:		ASSERT(x1Data);	
				return x1Data[id];
	case 2:		ASSERT(x2Data);	
				return x2Data[id];
	}
	NEVER();
	return Null;
}

END_UPP_NAMESPACE