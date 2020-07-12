#include <Core/Core.h>
#include <Functions4U/Functions4U.h>

#include "Financial.h"

namespace Upp {
	
double NetPresentValue(double discountRate, const Vector<double> &cf) {	
	double npv = 0;

	for (int i = 0; i < cf.GetCount(); i++) {
		double denom = pow(1 + discountRate, i+1);
		npv += cf[i]/denom;
  	}
	return npv;
}

Vector<double> InternalRateOfReturn(const Vector<double> &cf, double lowRate, double highRate, int maxIteration, double precisionReq) { 
	double deltaBase = 0.01;
	Vector<double> ret;
	double prevx = lowRate - deltaBase;
	double prev = NetPresentValue(prevx, cf);
	for (double valx = lowRate; valx < highRate; valx += deltaBase) {
		double val = NetPresentValue(valx, cf);
		if (Sign(val) != Sign(prev)) {
			double fromx = prevx;
			double from = prev;
			double tox = valx;
			double to = val;
			double oldGuessx = DOUBLE_NULL;
			for (int iter = 0; iter < maxIteration; ++iter) {
				double guessx = fromx - from*(tox - fromx)/(to - from);
				double guess = NetPresentValue(guessx, cf);
				if (fabs(oldGuessx - guessx) < precisionReq) {
					ret	<< guessx;
					break;
				}
				oldGuessx = guessx;
				if (Sign(from) == Sign(guess)) {
					from = guess;
					fromx = guessx;
				} else {
					to = guess;
					tox = guessx;
				}
			}
		}
		prev = val;
		prevx = valx;
	}
	return ret;
}

double PMT(double rate, double nper, double pv) {
	return -pv * rate * pow(1 + rate, nper) / (pow(1 + rate, nper) - 1);
}
	
}