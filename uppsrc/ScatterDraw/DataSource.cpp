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
	if (minVal == -DOUBLE_NULL)
		return Null;
	return minVal;		
}
 
double DataSource::Max(Getdatafun getdata) {
	double maxVal = DOUBLE_NULL;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && maxVal < d)
			maxVal = d;
	}
	if (maxVal == DOUBLE_NULL)
		return Null;
	return maxVal;
}

double DataSource::Avg(Getdatafun getdata) {
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d)) {
			ret += d;
			count++;
		}
	}
	if (count == 0) 
		return Null;
	return ret/count;
}

double DataSource::Variance(Getdatafun getdata, double avg) {
	if (IsNull(avg))
		avg = Avg(getdata);
	if (IsNull(avg))
		return Null;
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d)) {
			d -= avg;
			ret += d*d;
			count++;
		}
	}
	if (count <= 0)
		return Null;
	return ret/(count - 1);
}

double DataSource::StdDev(Getdatafun getdata, double avg) {
	return sqrt(Variance(getdata, avg));
}

double DataSource::SinEstim_Amplitude(double avg) {
	return sqrt(2.*VarianceY(avg));
}

bool DataSource::SinEstim_FreqPhase(double &frequency, double &phase, double avg) {
	if (GetCount() < 4)
		return false;
	if (IsNull(avg))
		avg = AvgY();
	bool firstIsToPositive = (y(int64(0)) - avg) < 0;
	bool isPossitive = !firstIsToPositive;
	double T = 0;
	int numT = 0;
	double lastZero = Null;
	double firstZero;
	for (int64 id = 1; id < GetCount(); ++id) {
		if (((y(id) - avg) > 0) != isPossitive) {
			isPossitive = !isPossitive;
			double zero = x(id-1) - (y(id-1) - avg)*(x(id) - x(id-1))/(y(id) - y(id-1));
			if (IsNull(lastZero)) 
				firstZero = zero;
			else {
				T += zero - lastZero;
				numT++;
			}
			lastZero = zero;
		}
	}
	T = 2*T/numT;
	frequency = 2*M_PI/T;			
	phase = -frequency*firstZero;
	if (!firstIsToPositive)
		phase += M_PI;
	return true;
}
	
	
double CArray::znFixed(int n, int64 id) {
	if (n == 0)
		return zData[id];
	NEVER();
	return Null;
}

END_UPP_NAMESPACE