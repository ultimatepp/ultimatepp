#include <Core/Core.h>
#include <Functions4U/Functions4U.h>
#include <plugin/Eigen/Eigen.h>
#include "Integral.h"

namespace Upp {

using namespace Eigen;


double Integral(VectorXd &y, const VectorXd &x) {
	ASSERT(x.size() == y.size());
	
	double ret = 0;
	Eigen::Index n = x.size();
	for (int i = 1; i < n; ++i)
		ret += Avg(y(i), y(i-1))*(x(i) - x(i-1));
	return ret;
}

double Integral(VectorXd &y, double dx, IntegralType type) {
	Eigen::Index n = y.size();
	
	if (n <= 1)
		return 0;
	else if (n == 2)
		return Avg(y(0), y(1))*dx;
	else if (type == TRAPEZOIDAL) {
		double ret2 = (y.segment(1, n-2).sum() + y(0)/2 + y(n-1)/2)*dx;
		return ret2;
	} else {
		double ret = 0;
		if ((n-1)%2) {
			ret = Avg(y(n-1), y(n-2))*dx;
			--n;
		}
		ret = ret + dx/3*(y(0) + 2*(Map<VectorXd, 0, InnerStride<2>>(y.data()+1, n/2).sum() + 
									 y.block(1, 0, n-2, 1).sum()) + y(n-1));
		return ret;
	}
}

}
