#ifndef _STEM4U_CrossCorrelation_h_
#define _STEM4U_CrossCorrelation_h_


namespace Upp {

template <class Range>
void XCorr(const Range &_x, const Range &_y, Range &_R, Range &lags, char scale = 'n', size_t maxlag = 0) {
	size_t N = std::max(_x.size(), _y.size());

	if (maxlag == 0)
		maxlag = N-1;
	
	ASSERT(maxlag <= N-1);

	size_t P = _x.size();
	size_t M = size_t(pow(2, NextPow2(int(N + maxlag))));
	
	Eigen::VectorXd x, y, R;
	
	Copy(_x, x);
	
	if (_y.size() == 0) {	
		Eigen::VectorXcd pre;
		Eigen::FFT<double> fft;
		
		PostPad(x, M, 0.);
		fft.fwd(pre, x);
		
		Eigen::VectorXcd post = pre.conjugate();
		
		for (int i = 0; i < pre.size(); ++i)
			pre[i] *= post[i];
		
		Eigen::VectorXd cor;
		fft.inv(cor, pre);
		
		Resize(R, 2*maxlag+1);
		R << cor.tail(maxlag), cor.head(maxlag+1);		
	} else {			
		Eigen::VectorXcd pre, post;
		Eigen::FFT<double> fft;
		
		PrePad(x, x.size() + maxlag, 0.);
		PostPad(x, M, 0.);
		fft.fwd(pre, x);
		
		Copy(_y, y);
		
		PostPad(y, M, 0.);
		fft.fwd(post, y);
		
		post = post.conjugate();
		
		for (int i = 0; i < pre.size(); ++i)
			pre[i] *= post[i];
		
		Eigen::VectorXd cor;
		fft.inv(cor, pre);
		R = cor.segment(0, 2*maxlag + 1);
	}
	
	double dN = double(N);
	double dmaxlag = double(maxlag);
	
  	if (scale == 'b')
    	R /= dN;
  	else if (scale == 'u') {
  		Vector<double> left, right;
  		LinSpaced(left, int(maxlag), dN-dmaxlag, dN-1);
  		left << dN;
  		LinSpaced(right, int(maxlag), dN-1, dN-dmaxlag);
  		left.Append(right);
    	R = R.cwiseQuotient(Eigen::Map<Eigen::VectorXd>(left, left.size()));
  	} else if (scale == 'c') {
	    if (y.size() == 0)
	      	R /= R[maxlag];
	    else
	      	R /= ::sqrt(x.squaredNorm()*y.squaredNorm());
 	} else if (scale == 'n')
 		;	
 	else 
 		NEVER_("Unknown scale");
	
	Copy(R, _R);
	
    LinSpaced(lags, 2*int(maxlag)+1, -dmaxlag, dmaxlag);
}

template <class Range>
void XCorr(const Range &x, Range &R, Range &lags, char scale = 'n', size_t maxlag = 0) {
	Range y;
	XCorr(x, y, R, lags, scale, maxlag);	 
}

}

#endif
