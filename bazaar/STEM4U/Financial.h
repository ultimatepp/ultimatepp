#ifndef _STEM4U_Financial_h_
#define _STEM4U_Financial_h_

#include <Core/Core.h>


namespace Upp {
	
double NetPresentValue(double discountRate, const Vector<double> &cf);	
Vector<double> InternalRateOfReturn(const Vector<double> &cf, double lowRate = -0.9, double highRate = 1, int maxIteration = 1000, double precisionReq = .00000001);
double PMT(double rate, double nper, double pv);
	
}


#endif
