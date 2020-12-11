#ifndef _STEM4U_utility_h_
#define _STEM4U_utility_h_

namespace Upp {

double R2(const Eigen::VectorXd &serie, const Eigen::VectorXd &serie0, double mean = Null);


template <class Range>
void CleanOutliers(const Range &x, const Range &y, const Range &filtery, Range &retx, Range &rety, 
				   const typename Range::value_type& ratio, 
				   const typename Range::value_type& zero = 0) {
	ASSERT(x.size() == y.size() && x.size() == filtery.size());
 
 	Resize(retx, x.size());
 	Resize(rety, x.size());
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
}

}

#endif
