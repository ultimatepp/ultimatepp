#ifndef _STEM4U_Integral_h_
#define _STEM4U_Integral_h_

namespace Upp {
	
enum IntegralType {TRAPEZOIDAL, SIMPSON_1_3}; 

double Integral(const Eigen::VectorXd &y, const Eigen::VectorXd &x);
double Integral(Eigen::VectorXd &y, double dx, IntegralType type = TRAPEZOIDAL);
	
}
	
#endif
