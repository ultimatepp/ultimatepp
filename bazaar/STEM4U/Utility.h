#ifndef _STEM4U_utility_h_
#define _STEM4U_utility_h_

namespace Upp {

double R2(const Eigen::VectorXd &serie, const Eigen::VectorXd &serie0, double mean = Null);


template <class Range>
void CleanOutliers(const Range &x, const Range &y, const Range &filtery, Range &rretx, Range &rrety, 
				   const typename Range::value_type& ratio, const typename Range::value_type& zero = 0) {
	ASSERT(x.size() == y.size() && x.size() == filtery.size());
 
 	Range retx(x.size()), rety(x.size());
 
 	int id = 0;
	for (int i = 0; i < x.size(); ++i) {
		if (EqualRatio(y[i], filtery[i], ratio, zero)) {
			retx[id] = x[i];
			rety[id] = y[i];
			id++;
		}
	}
	ResizeConservative(retx, id); 
	ResizeConservative(rety, id);
	
	rretx = pick(retx);
	rrety = pick(rety);
}

template <class Range>
void CleanCondition(const Range &x, const Range &y, Range &rretx, Range &rrety, 
		Function <bool(int id)> Cond) {
	ASSERT(x.size() == y.size());
 
 	Range retx(x.size()), rety(x.size());
 	
 	int id = 0;
	for (int i = 0; i < x.size(); ++i) {
		if (Cond(i)) {
			retx[id] = x[i];
			rety[id] = y[i];
			id++;
		}
	}
	ResizeConservative(retx, id); 
	ResizeConservative(rety, id);

	rretx = pick(retx);
	rrety = pick(rety);
}

template <class Range>
typename Range::value_type GetSampleRate(const Range &x, int numDecimals) {
	using Scalar = typename Range::value_type;
	
	int n = int(x.size());
	if (n < 2)
		return Null;
	
	Vector<Scalar> delta;
	Vector<int> num;
	
	for (int i = 1; i < n; ++i) {
		Scalar d = x[i]-x[i-1];
		int id = FindRoundDecimals(delta, d, numDecimals);	
		if (id >= 0)
			num[id]++;
		else {
			delta << d;
			num << 1;		
		}
	}
	int nummx = num[0], idmx = 0;
	for (int i = 1; i < delta.size(); ++i) {
		if (num[i] > nummx) {
			nummx = num[i];
			idmx = i;
		}
	}
	return delta[idmx];
}

}

#endif
