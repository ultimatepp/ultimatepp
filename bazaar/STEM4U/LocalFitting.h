#ifndef _STEM4U_LocalFitting_h_
#define _STEM4U_LocalFitting_h_

namespace Upp {
using namespace Eigen;

template <class Range1, class Range2>
void LocalFitting(const Range1 &x, const Range1 &y, const Range2 &resx, Range2 &resy, 
	int deg, typename Range1::value_type windowsize) {
	using Scalar = typename Range1::value_type;
	ASSERT(x.size() == y.size());
	ASSERT(deg >=1 && deg <= 2);
	Resize(resy, resx.size());

	const Scalar minweight = 0.05;
	Scalar w_2 = windowsize/2;
	VectorXd coeff, weight;
	for (int i = 0; i < resx.size(); ++i) {
		int ibegin, iend;
		const auto &rx = resx[i];
		for (ibegin = 0; ibegin < x.size(); ++ibegin) 
			if (x[ibegin] >= rx - w_2) {
				if (x[ibegin] >= rx)
					ibegin--;
				ibegin = max(0, ibegin);
				break;
			}
		for (iend = ibegin+1; iend < x.size(); ++iend) 
			if (x[iend] >= rx + w_2) 
				break;
		iend = min(iend, int(x.size()-1));
		int num = iend-ibegin+1;
		if (num == 2) 
			resy[i] = LinearInterpolate(rx, x[ibegin], x[iend], y[ibegin], y[iend]);
		else if (num == 3 && deg == 2)
		 	resy[i] = QuadraticInterpolate(rx, x[ibegin], x[ibegin+1], x[iend], y[ibegin], y[ibegin+1], y[iend]);
		else {
			coeff.setConstant(deg+1, 1);
			coeff[0] = 0;
			for (int ii = ibegin; ii <= iend; ii++)
				coeff[0] += y[ii];
			coeff[0] /= num;
			weight.resize(num);
			for (int ii = ibegin; ii <= iend; ii++)
				weight[ii-ibegin] = max(minweight, pow3(1 - pow3(min(1., abs(x[ii-ibegin]-rx)/w_2))));
					
			if (!NonLinearOptimization(coeff, num, [&](const VectorXd &c, VectorXd &residual)->int {
				for (int ii = ibegin; ii <= iend; ii++) {
					if (deg == 2) 
						residual[ii-ibegin] = weight[ii-ibegin]*(c(0) + c(1)*x[ii] + c(2)*sqr(x[ii]) - y[ii]);
					else 
						residual[ii-ibegin] = weight[ii-ibegin]*(c(0) + c(1)*x[ii] - y[ii]);
				}
				return 0;	
			}))
				throw Exc(t_("LOWESS Impossible to get coefficients"));
			if (deg == 2)
				resy[i] = coeff(0) + coeff(1)*rx + coeff(2)*sqr(rx);
			else
				resy[i] = coeff(0) + coeff(1)*rx;
		}
	}
}

template <class Range1, class Range2>
void LocalFitting(const Range1 &x, const Range1 &y, Range2 &resx, Range2 &resy, int deg, typename Range1::value_type windowsize, int num,
		typename Range1::value_type from = Null, typename Range1::value_type to = Null) {
	ASSERT(x.size() == y.size());
	if (IsNull(from))
		from = x[0];
	if (IsNull(to))
		to = x[x.size()-1];
	ASSERT(x[0] <= from && x[x.size()-1] >= to);
	
	resx.setLinSpaced(num, from, to);
	
	return LocalFitting(x, y, resx, resy, deg, windowsize);
}


/*  c++ implementation of Lowess weighted regression based on Peter Glaus 
 *		http://www.cs.man.ac.uk/~glausp/
 *		https://github.com/BitSeq/BitSeq/blob/master/lowess.cpp
 *
 *  Based on fortran code by Cleveland downloaded from:
 *  	http://netlib.org/go/lowess.f
 *  original author:
 *  	wsc@research.bell-labs.com Mon Dec 30 16:55 EST 1985
 *  	W. S. Cleveland
 *  	Bell Laboratories
 *  	Murray Hill NJ 07974
 * 		Cleveland, W.S. (1979) “Robust Locally Weighted Regression and Smoothing Scatterplots”. Journal of the American Statistical Association 74 (368): 829-836.
 */  
 
template <class Range1, class Range2>
bool Lowest(const Range1 &x, const Range1 &y, typename Range1::value_type xs, Range2 &ys, int nleft, int nright, 
			Vector<typename Range1::value_type> &weight, bool userw, Vector<typename Range1::value_type> &residual) {
	using Scalar = typename Range1::value_type;
	int n = x.size();
	Scalar range = x[n-1]-x[0];
	Scalar h = max(xs-x[nleft],x[nright]-xs);
	Scalar h9 = 0.999*h;
	Scalar h1 = 0.001*h;
	Scalar a = 0; 
	int j;
	for (j = nleft; j < n; j++) {		// compute weights (pick up all ties on right)
		weight[j] = 0;
		Scalar r = abs(x[j]-xs);
		if (r <= h9) {		// small enough for non-zero weight
			if (r > h1) 
				weight[j] = pow3(1 - pow3(r/h));
			else 
				weight[j] = 1;
			if (userw) 
				weight[j] *= residual[j];
			a += weight[j];
		} else if (x[j] > xs) 
			break; 			// get out at first zero wt on right
	}
	int nrt = j-1;
	if (a <= 0) 			// rightmost pt (may be greater than nright because of ties)
   		return false;
   	else {					// weighted least squares	
      	for (int j = nleft; j <= nrt; j++)		// normalize weights
         	weight[j] /= a;
      	if (h > 0) {         	// use linear fit
         	Scalar a = 0;
         	for (j = nleft; j <= nrt; j++)
            	a += weight[j]*x[j]; 			// weighted centre of values
			Scalar b = xs-a;
			Scalar c = 0;
			for (j = nleft; j <= nrt; j++)
            	c += weight[j]*sqr(x[j] - a);
         	if (sqrt(c) > 0.001*range) {		// points are spread enough to compute slope
            	b /= c;
            	for (j = nleft; j <= nrt; j++)
               		weight[j] *= (1. + b*(x[j] - a));
         	}
      	}
      	ys = 0;
      	for (int j = nleft; j <= nrt; j++)
         	ys += weight[j]*y[j];
   	}
   	return true;
}

// frac 	Between 0 and 1. The fraction of the data used when estimating each y-value.
// nsteps  	The number of residual-based reweightings to perform.
// delta	Distance within which to use linear-interpolation instead of weighted regression.

template <class Range1, class Range2>
void Lowess(const Range1 &x, const Range1 &y, typename Range1::value_type frac, int nsteps, typename Range1::value_type delta, Range2 &ys) {
	using Scalar = typename Range1::value_type;
	ASSERT(x.size() == y.size());
	int n = x.size();
	if (n == 0 || y.size() != n) 
		return;
	ys.SetCount(n);
	Vector<Scalar> weight(n), residual(n);
	if (n == 1) {
		ys[0] = y[0];
		return;
	}
	int ns = max(min(int(frac*n), n), 2);// ns - at least 2, at most n
	for (int iter = 0; iter < nsteps+1; iter++) {	// robustness iterations
		int nleft = 0;
		int nright = ns-1;
		int last = -1;					// index of last estimated point
		int i = 0;						// index of current point
		do {
			while (nright < n-1) {		// move <nleft,nright> right, while radius decreases
				Scalar d1 = x[i]-x[nleft];
				Scalar d2 = x[nright+1] - x[i];
				if (d1 <= d2)
					break;
				nleft++;
				nright++;
			}
			if (!Lowest(x, y, x[i], ys[i], nleft, nright, residual, iter>0, weight)) // fit value at x[i]
				ys[i] = y[i];
			if (last < i-1) {			// interpolate skipped points
				if (last < 0)
					throw Exc("Lowess: out of range");
				Scalar denom = x[i] - x[last];
				for (int j = last+1; j < i; j++) {
					Scalar alpha = (x[j] - x[last])/denom;
					ys[j] = alpha * ys[i] + (1-alpha)*ys[last];
				}
			}
			last = i;
			Scalar cut = x[last]+delta;
			for (i = last+1; i < n; i++) {
				if (x[i] > cut)
					break;
				if (x[i] == x[last]) {
					ys[i] = ys[last];
					last = i;
				}
			}
			i = max(last+1,i-1);
		} while (last < n-1);
		
		for (int i = 0; i < n; i++)
			residual[i] = y[i] - ys[i];
		if (iter == nsteps)
			break;
		for (int i = 0; i < n; i++)
			weight[i] = abs(residual[i]);
		Sort(weight);
		int m1 = n/2 + 1;
		int m2 = n-m1;
		m1--;
		Scalar cmad = 3*(weight[m1] + weight[m2]);
		Scalar c9 = .999*cmad;
		Scalar c1 = .001*cmad;
		for (int i = 0; i < n; i++) {
			Scalar r = abs(residual[i]);
			if (r <= c1) 
				weight[i] = 1;
			else if (r > c9) 
				weight[i] = 0;
			else 
				weight[i] = sqr(1 - sqr(r/cmad));
		}
	}
}


}
		
		
#endif
