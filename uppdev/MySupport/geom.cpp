#include "support.h"

double LinearInterpolation(double x, Point_<double> p1, Point_<double> p2, bool yx) {
	double y, k;
	
	if(!yx) {
		k = ((p2.y - p1.y) / (p2.x - p1.x));
		if(k >= 0) y = x - (p1.x * k) + p1.y;
		else y = (p1.x * k) - x + p1.y;
	}
	else {
		k = ((p2.y - p1.y) / (p2.x - p1.x));
		if(k < 0) y = p2.x - ((p2.x - x) * k);
		else y = p1.x + ((x - p1.y) / k); // 20.07.2005  provato - OK
	}
#ifdef _WITH_DEBUG
RLOG("\nLinearInterpolation()");
RLOG("k: " + FormatDouble(k));
RLOG("x: " + FormatDouble(x));
RLOG("p1: " + FormatDouble(p1.x) + "," + FormatDouble(p1.y));
RLOG("p2: " + FormatDouble(p2.x) + "," + FormatDouble(p2.y));
RLOG("y: " + FormatDouble(y));
#endif	
	return y;
}	
