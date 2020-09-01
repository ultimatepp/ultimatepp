#ifndef _STEM4U_Integral_h_
#define _STEM4U_Integral_h_

namespace Upp {
	
enum IntegralType {TRAPEZOIDAL, SIMPSON_1_3, SIMPSON_3_8, HERMITE_3, HERMITE_5}; 

template <class Range, class T>
T Integral(const Range &y, const Range &x, IntegralType type = TRAPEZOIDAL) {
	ASSERT(x.size() == y.size());
	ASSERT(x.size() > 1);
	
	T ret = 0;
	size_t n = x.size();
	if (type == TRAPEZOIDAL) {
		for (int i = 1; i < n; ++i)
			ret += Avg(y(i), y(i-1))*(x(i) - x(i-1));
	} else if (type == SIMPSON_1_3) {
		if (n < 3)
			return Null;
		int i;
		for (i = 2; i < n; i += 2)
			ret += (x[i] - x[i-2])/6.*(y[i-2] + 4*y[i-1] + y[i]);
		if (i == n)
			ret += Avg(y(n-1), y(n-2))*(x(n-1) - x(n-2));
	} else if (type == SIMPSON_3_8) {
		if (n < 4)
			return Null;
		int i;
		for (i = 3; i < n; i += 3)
			ret += (x[i] - x[i-3])/8.*(y[i-3] + 3*y[i-2] + 3*y[i-1] + y[i]);
		if (i == n)
			ret += (x[n-1] - x[n-3])/6.*(y[n-3] + 4*y[n-2] + y[n-1]);
		else if (i == n+1)
			ret += Avg(y(n-1), y(n-2))*(x(n-1) - x(n-2));
	} else
		NEVER();
	return ret;
}


template <class Range, class T>
inline T Calc1_3(const Range &y, T dx, size_t n) {
	T ret = y[0] + y[n-1];
	for (int i = 1; i < n-1; i++)
		ret += 2*y[i];
	for (int i = 1; i < n-1; i += 2)
		ret += 2*y[i];
	ret *= dx/3.;
	return ret;
}

inline double Calc1_3(Eigen::VectorXd &y, double dx, size_t n) {
	return dx/3*(y(0) + 2*(Eigen::Map<Eigen::VectorXd, 0, Eigen::InnerStride<2>>(y.data()+1, n/2).sum() + 
									 y.block(1, 0, n-2, 1).sum()) + y(n-1));
}

template <class Range, class T>
T Integral(Range &y, T dx, IntegralType type = TRAPEZOIDAL) {
	ASSERT(y.size() > 1);
	Eigen::Index n = y.size();
	
	if (type == TRAPEZOIDAL) 
		return (y.segment(1, n-2).sum() + (y(0) + y(n-1))/2)*dx;
	else if (type == SIMPSON_1_3) {
		if (n < 3)
			return Null;
		T ret0 = 0;
		if ((n-1)%2) {
			ret0 = Avg(y(n-1), y(n-2))*dx;
			--n;
		}
		return ret0 + Calc1_3(y, dx, n);
	} else if (type == SIMPSON_3_8) {
		if (n < 4)
			return Null;
		T ret0 = 0;
		int rem = (n-1)%3;
		if (rem == 2) {
			ret0 = (y[n-3] + 4*y[n-2] + y[n-1])/3.*dx;
			n -= 2;
		} else if (rem == 1) {
			ret0 = Avg(y[n-2], y[n-1])*dx;
			n--;
		}
		T ret = y[0] + y[n-1];
		for (int i = 1; i < n-1; ++i)
			ret += 2*y[i];
		for (int i = 1; i < n-1; ++i) {
			ret += y[i++];
			ret += y[i++];
		}
		return ret0 + ret*dx*3./8;
	} else if (type == HERMITE_3) {
		if (n < 3)
			return Null;
		return (y.segment(1, n-2).sum() + (y(0) + y(n-1))/2)*dx
			 + dx/24.*(3*y[0] - 4*y[1] + y[2] + y[n-3] - 4*y[n-2] + 3*y[n-1]);
	} else if (type == HERMITE_5) {
		if (n < 5)
			return Null;
		return (y.segment(1, n-2).sum() + (y(0) + y(n-1))/2)*dx
			 - dx/144.*(25*(y[0] - y[n-1]) - 48*(y[1] - y[n-2]) + 36*(y[2] - y[n-3])
			 		  - 16*(y[3] - y[n-4]) +  3*(y[4] - y[n-5]));
	} else
		NEVER();
	return Null;
}
	
}
	
#endif
