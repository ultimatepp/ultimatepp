#ifndef _STEM4U_Butterworth_h_
#define _STEM4U_Butterworth_h_

namespace Upp {

// Butterworth filter based on https://github.com/nxsEdson/Butterworth-Filter

template <class Range>	
void TrinomialMultiply(int order, const Range &b, const Range &c, Range &ret) {
	Resize(ret, 4*order, 0.);

	ret[2] = c[0];
	ret[3] = c[1];
	ret[0] = b[0];
	ret[1] = b[1];

	for (int i = 1; i < order; ++i) {
		ret[2*(2*i + 1)] += c[2*i] * ret[2*(2*i - 1)] - c[2*i + 1] * ret[2*(2*i - 1) + 1];
		ret[2*(2*i + 1) + 1] += c[2*i] * ret[2*(2*i - 1) + 1] + c[2*i + 1] * ret[2*(2*i - 1)];

		for (int j = 2*i; j > 1; --j) {
			ret[2*j] 	 += b[2*i] * ret[2*(j-1)]   - b[2*i+1] * ret[2*(j-1)+1] +
						    c[2*i] * ret[2*(j-2)]   - c[2*i+1] * ret[2*(j-2)+1];
			ret[2*j + 1] += b[2*i] * ret[2*(j-1)+1] + b[2*i+1] * ret[2*(j-1)] +
							c[2*i] * ret[2*(j-2)+1] + c[2*i+1] * ret[2*(j-2)];
		}
		ret[2] += b[2*i] * ret[0] - b[2*i + 1] * ret[1] + c[2*i];
		ret[3] += b[2*i] * ret[1] + b[2*i + 1] * ret[0] + c[2*i+1];
		ret[0] += b[2*i];
		ret[1] += b[2*i+1];
	}
}

template <class Range>	
void BinomialMultiply(int order, const Range &p, Range &ret) {
	Resize(ret, 2*order, 0.);

    for (int i = 0; i < order; ++i) {
		for(int j = i; j > 0; --j) {
		    ret[2*j]   += p[2*i] * ret[2*(j-1)]   - p[2*i+1] * ret[2*(j-1)+1];
		    ret[2*j+1] += p[2*i] * ret[2*(j-1)+1] + p[2*i+1] * ret[2*(j-1)];
		}
		ret[0] += p[2*i];
		ret[1] += p[2*i+1];
    }
}

template <class Range>
void CoefficientsLowPass(int order, Range &coeffs) {
	Resize(coeffs, order + 1);

	coeffs[0] = 1;
	coeffs[1] = order;
	int m = order/2;
	for (int i = 2; i <= m; ++i) {
		coeffs[i] 		= (order-i+1)*coeffs[i-1]/i;
		coeffs[order-i] = coeffs[i];
	}
	coeffs[order-1] = order;
	coeffs[order]   = 1;
}

template <class Range>
void CoefficientsHighPass(int order, Range &coeffs) {
	CoefficientsLowPass(order, coeffs);

	for (int i = 0; i <= order; ++i)
		if (i%2) 
			coeffs[i] = -coeffs[i];
}

template <class Range>
void DCoefficientsLowHigh(int order, typename Range::value_type cutoff, Range &cden, double st, double ct) {
	using Scalar = typename Range::value_type;
	
	Range rcoeffs(2*order);  
	
	for (int i = 0; i < order; ++i) {
		Scalar poleAngle = M_PI * (2*i + 1) / (2.*order);
		Scalar sinPoleAngle = sin(poleAngle);
		Scalar cosPoleAngle = cos(poleAngle);
		Scalar a = 1 + st*sinPoleAngle;
		rcoeffs[2*i] = -ct / a;
		rcoeffs[2*i + 1] = -st*cosPoleAngle / a;
	}

	BinomialMultiply(order, rcoeffs, cden);

	cden[1] = cden[0];
	cden[0] = 1;
	for (int i = 3; i <= order; ++i)
		cden[i] = cden[2*i - 2];

	ResizeConservative(cden, order + 1);
}

template <class Range>
void Butter(int order, typename Range::value_type lowcutoff, typename Range::value_type upcutoff, Range &cnum, Range &cden) {
	using Scalar = typename Range::value_type;
	
	Scalar cp = cos(M_PI*(upcutoff+lowcutoff)/2.);
	Scalar theta = M_PI*(upcutoff-lowcutoff)/2.;
	Scalar st = sin(theta);
	Scalar ct = cos(theta);
	Scalar s2t = 2*st*ct;       
	Scalar c2t = 2*ct*ct-1;  

	Range rcoeffs(2*order);  
	Range tcoeffs(2*order); 
	
	for (int i = 0; i < order; ++i) {
		Scalar poleAngle = M_PI * (2.*i+1)/(2.*order);
		Scalar sinPoleAngle = sin(poleAngle);
		Scalar cosPoleAngle = cos(poleAngle);
		Scalar a = 1 + s2t*sinPoleAngle;
		rcoeffs[2*i] = c2t / a;
		rcoeffs[2*i + 1] = s2t*cosPoleAngle/a;
		tcoeffs[2*i]   = -2*cp*(ct + st*sinPoleAngle)/a;
		tcoeffs[2*i+1] = -2*cp*st*cosPoleAngle/a;
	}

	TrinomialMultiply(order, tcoeffs, rcoeffs, cden);

	cden[1] = cden[0];
	cden[0] = 1;
	for (int i = 3; i <= 2*order; ++i)
		cden[i] = cden[2*i-2];

	ResizeConservative(cden, 2*order+1);

	Resize(cnum, 2*order + 1);

	Range numbers(2*order + 1);
	for (int n = 0; n < 2*order + 1; n++)
		numbers[n] = (Scalar)n;

	CoefficientsHighPass(order, tcoeffs);

	for (int i = 0; i < order; ++i) {
		cnum[2*i] = tcoeffs[i];
		cnum[2*i + 1] = 0;
	}
	cnum[2*order] = tcoeffs[order];
	
	Scalar cplow = 4*tan(M_PI * lowcutoff / 2.);
	Scalar cpup  = max(0., 4*tan(M_PI * upcutoff  / 2.));

	Scalar wn = 2 * atan2(sqrt(cplow * cpup), 4);
	const auto result = std::complex<Scalar>(-1, 0);

	Array<std::complex<Scalar>> normalizedKernel(2 * order + 1);
	for (int i = 0; i < 2*order + 1; i++)
		normalizedKernel[i] = std::exp(-sqrt(result)*wn*numbers[i]);

	Scalar b = 0;
	Scalar den = 0;
	for (int d = 0; d < 2*order + 1; d++) {
		b   += real(normalizedKernel[d] * cnum[d]);
		den += real(normalizedKernel[d] * cden[d]);
	}
	for (int c = 0; c < 2*order + 1; c++)
		cnum[c] *= den/b;
}

template <class Range>
void ButterBandStop(int order, typename Range::value_type lowcutoff, typename Range::value_type upcutoff, Range &cnum, Range &cden) {
	using Scalar = typename Range::value_type;
	
	Scalar cp = cos(M_PI*(upcutoff+lowcutoff)/2.);
	Scalar theta = M_PI*(upcutoff-lowcutoff)/2.;
	Scalar st = sin(theta);
	Scalar ct = cos(theta);
	Scalar s2t = 2*st*ct;       
	Scalar c2t = 2*ct*ct-1;  

	Range RCoeffs(2*order);  
	Range TCoeffs(2*order); 
	
	for (int i = 0; i < order; ++i) {
		Scalar poleAngle = M_PI * (2.*i + 1)/(2.*order);
		Scalar sinPoleAngle = sin(poleAngle);
		Scalar cosPoleAngle = cos(poleAngle);
		Scalar a = 1 + s2t*sinPoleAngle;
		RCoeffs[2*i] = c2t/a;
		RCoeffs[2*i + 1] = -s2t*cosPoleAngle/a;
		TCoeffs[2*i] = -2*cp*(ct + st*sinPoleAngle)/a;
		TCoeffs[2*i+1] = 2*cp*st*cosPoleAngle/a;
	}

	TrinomialMultiply(order, TCoeffs, RCoeffs, cden);

	cden[1] = cden[0];
	cden[0] = 1;
	for (int i = 3; i <= 2*order; ++i)
		cden[i] = cden[2*i-2];

	ResizeConservative(cden, 2*order+1);

	Resize(cnum, 2*order+1, 0.);

    Scalar alpha = -2*cp/ct;
    cnum[0] = 1;
    cnum[2] = 1;
    cnum[1] = alpha;
  
    for(int i = 1; i < order; ++i ) {
		cnum[2*i+2] += cnum[2*i];
		for(int j = 2*i; j > 1; --j )
		    cnum[j+1] += alpha * cnum[j] + cnum[j-1];
	
		cnum[2] += alpha * cnum[1] + 1.0;
		cnum[1] += alpha;
    }
	
    Scalar tt = tan(theta);
    Scalar sfr = 1;
    Scalar sfi = 0;

    for(int k = 0; k < order; ++k ) {
		Scalar parg = M_PI * (double)(2*k+1)/(double)(2*order);
		Scalar sparg = tt + sin(parg);
		Scalar cparg = cos(parg);
		Scalar a = (sfr + sfi)*(sparg - cparg);
		Scalar b = sfr * sparg;
		Scalar c = -sfi * cparg;
		sfr = b - c;
		sfi = a - b - c;
    }
	cnum = cnum/sfr;
}

template <class Range>
void ButterLowPass(int order, typename Range::value_type cutoff, Range &cnum, Range &cden) {
	using Scalar = typename Range::value_type;
	
	Scalar theta = M_PI*cutoff;
	Scalar st = sin(theta);
	Scalar ct = cos(theta);
	
	DCoefficientsLowHigh(order, cutoff, cden, st, ct);

	Resize(cnum, 2*order + 1);

	CoefficientsLowPass(order, cnum);

    Scalar parg0 = M_PI/(2*order);

    Scalar sf = 1;
    for(int k = 0; k < order/2; ++k )
        sf *= 1 + st*sin((2*k+1)*parg0);

    st = sin(theta/2);

    if(order%2) 
    	sf *= st + cos(theta/2);
    sf = pow(st, order)/sf;

    cnum = cnum*sf;
    
    ResizeConservative(cnum, order + 1);
}

template <class Range>
void ButterHighPass(int order, typename Range::value_type cutoff, Range &cnum, Range &cden) {
	using Scalar = typename Range::value_type;
	
	Scalar theta = M_PI*cutoff;
	Scalar st = sin(theta);
	Scalar ct = cos(theta);
	
	DCoefficientsLowHigh(order, cutoff, cden, st, ct);

	Resize(cnum, 2*order + 1);

	CoefficientsHighPass(order, cnum);

    double parg0 = M_PI/(2*order);

    double sf = 1;
    for(int k = 0; k < order/2; ++k )
        sf *= 1 + st*sin((2*k+1)*parg0);

    st = cos(theta/2);

    if(order%2) 
    	sf *= st + sin(theta/2);
    sf = pow(st, order)/sf;

    cnum = cnum*sf;
    
	ResizeConservative(cnum, order + 1);
}

template <class Range>
void Filter(const Range &x, const Range &cnum, const Range &cden, Range &filtered) {
	size_t len_x = x.size();
	size_t len_b = cnum.size();
	size_t len_a = cden.size();

	Range zi;
	Resize(zi, len_b, 0.);

	Resize(filtered, len_x);

	if (len_a == 1) {
		for (int m = 0; m < len_x; m++) {
			filtered[m] = cnum[0] * x[m] + zi[0];
			for (int i = 1; i<len_b; i++)
				zi[i - 1] = cnum[i] * x[m] + zi[i];
		}
	} else {
		for (int m = 0; m < len_x; m++) {
			filtered[m] = cnum[0] * x[m] + zi[0];
			for (int i = 1; i<len_b; i++)
				zi[i - 1] = cnum[i] * x[m] + zi[i] - cden[i] * filtered[m];
		}
	}
}

// Zero phase filter implementation based on Hugh Nolan code posted here: https://stackoverflow.com/questions/17675053/matlabs-filtfilt-algorithm/27270420#27270420

template <class Range>
void filter_(const Range &b, const Range &a, const Range &X, Range &Y, Range &Zi) {
	using Scalar = typename Range::value_type;
	
    if (a.size() == 0)
        throw Exc("The feedback filter coefficients are empty.");
    if (std::all_of(a.data(), a.data()+a.size(), [](Scalar coef){return coef == 0;}))
        throw Exc("At least one of the feedback filter coefficients has to be non-zero.");
    if (a[0] == 0)
        throw Exc("First feedback coefficient has to be non-zero.");

	std::vector<Scalar> A(a.size()), B(b.size());
	std::copy(Begin(a), End(a), A.begin());
	std::copy(Begin(b), End(b), B.begin());

    auto a0 = A[0];
    if (a0 != 1) {   // Normalize feedback coefficients if a[0] != 1;
        std::transform(A.begin(), A.end(), A.begin(), [a0](Scalar v) {return v/a0;});
        std::transform(B.begin(), B.end(), B.begin(), [a0](Scalar v) {return v/a0;});
    }

    size_t input_size = X.size();
    size_t filter_order = std::max(A.size(), B.size());
    ResizeConservative(B, filter_order, 0.);
    ResizeConservative(A, filter_order, 0.);  
    ResizeConservative(Zi, filter_order, 0.);
    Resize(Y, input_size);

    for (size_t i = 0; i < input_size; ++i) {
        size_t order = filter_order - 1;
        while (order) {
            if (i >= order)
                Zi[order - 1] = B[order]*X[i - order] - A[order]*Y[i - order] + Zi[order];
            --order;
        }
        Y[i] = B[0]*X[i] + Zi[0];
    }
    ResizeConservative(Zi, filter_order - 1);
}

template <class Range>
void filter_(const Range &B, const Range &A, const Range &X, Range &Y) {
	Range zi;
   	filter_(B, A, X, Y, zi);
}

template <class Range>
void add_index_range(Range &indices, size_t beg, size_t end, int inc = 1) {
	size_t len0 = indices.size();
    ResizeConservative(indices, len0 + 1 + (end - beg)/inc);
    for (size_t i = beg; i <= end; i += inc)
       indices[len0++] = int(i);
}

template <class Range>
void add_index_const(Range &indices, size_t value, size_t numel) {
    ResizeConservative(indices, indices.size() + numel, typename Range::value_type(value));
}

template <class Range>
void append_vector(std::vector<double> &vec, const Range &tail) {
    vec.insert(vec.end(), tail.data(), tail.data() + tail.size());
}

template <class Range1, class Range2>
void subvector_reverse(const Range1 &vec, size_t idx_end, size_t idx_start, Range2 &result) {
	Resize(result, idx_end + 1 - idx_start);
    std::copy(&vec[idx_start], &vec[idx_end+1], Begin(result));
    Reverse(result);
}

template <class Range>
void Filtfilt(const Range &X, const Range &b, const Range &a, Range &Y) {
    using namespace Eigen;
	using Scalar = typename Range::value_type;
	
    size_t len = X.size();     
    size_t na = a.size();    
    size_t nb = b.size();
    
    std::vector<Scalar> A(Begin(a), End(a)), 
    					B(Begin(b), End(b));
    
    size_t nfilt = (nb > na) ? nb : na;
    size_t nfact = 3 * (nfilt - 1); // length of edge transients

    if (len <= nfact)
        throw Exc("Input data too short! Data must have length more than 3 times filter order.");

    // set up filter's initial conditions to remove DC offset problems at the
    // beginning and end of the sequence
    B.resize(nfilt, 0);
    A.resize(nfilt, 0);

    std::vector<int> rows, cols;
    //rows = [1:nfilt-1           2:nfilt-1             1:nfilt-2];
    add_index_range(rows, 0, nfilt - 2);
    if (nfilt > 2) {
        add_index_range(rows, 1, nfilt - 2);
        add_index_range(rows, 0, nfilt - 3);
    }
    //cols = [ones(1,nfilt-1)         2:nfilt-1          2:nfilt-1];
    add_index_const(cols, 0, nfilt - 1);
    if (nfilt > 2) {       
        add_index_range(cols, 1, nfilt - 2);
        add_index_range(cols, 1, nfilt - 2);
    }
    // data = [1+a(2)         a(3:nfilt)        ones(1,nfilt-2)    -ones(1,nfilt-2)];

    auto klen = rows.size();
    std::vector<Scalar> data;
    data.resize(klen);
    data[0] = 1 + A[1];  int j = 1;
    if (nfilt > 2) {
        for (int i = 2; i < nfilt; i++)
            data[j++] = A[i];
        for (int i = 0; i < nfilt - 2; i++)
            data[j++] = 1.0;
        for (int i = 0; i < nfilt - 2; i++)
            data[j++] = -1.0;
    }

    std::vector<Scalar> leftpad;
    subvector_reverse(X, nfact, 1, leftpad);
    Scalar _2x0 = 2 * X[0];
    std::transform(leftpad.begin(), leftpad.end(), leftpad.begin(), [_2x0](Scalar val) {return _2x0 - val; });

    std::vector<Scalar> rightpad;
    subvector_reverse(X, len - 2, len - nfact - 1, rightpad);
    Scalar _2xl = 2 * X[len-1];
    std::transform(rightpad.begin(), rightpad.end(), rightpad.begin(), [_2xl](Scalar val) {return _2xl - val; });

    Scalar y0;
    std::vector<Scalar> signal1, signal2, zi;

    signal1.reserve(leftpad.size() + X.size() + rightpad.size());
    append_vector(signal1, leftpad);
    append_vector(signal1, X);
    append_vector(signal1, rightpad);

    // Calculate initial conditions
    MatrixXd sp = MatrixXd::Zero(std::max_element(rows.begin(), rows.end())[0] + 1, 
    							 std::max_element(cols.begin(), cols.end())[0] + 1);
    for (size_t k = 0; k < klen; ++k)
        sp(rows[k], cols[k]) = data[k];

    auto bb = VectorXd::Map(B.data(), B.size());
    auto aa = VectorXd::Map(A.data(), A.size());
    MatrixXd zzi = (sp.inverse() * (bb.segment(1, nfilt - 1) - (bb(0) * aa.segment(1, nfilt - 1))));
    zi.resize(zzi.size());

    // Do the forward and backward filtering
    y0 = signal1[0];
    std::transform(zzi.data(), zzi.data() + zzi.size(), zi.begin(), [y0](Scalar val){ return val*y0; });
    filter_(B, A, signal1, signal2, zi);
    std::reverse(signal2.begin(), signal2.end());   
    y0 = signal2[0];
    std::transform(zzi.data(), zzi.data() + zzi.size(), zi.begin(), [y0](Scalar val){ return val*y0; });
    filter_(B, A, signal2, signal1, zi);
    subvector_reverse(signal1, signal1.size() - nfact - 1, nfact, Y);
}


}

#endif
