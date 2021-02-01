#include "ScatterDraw.h"

namespace Upp {
using namespace Eigen;

bool IsNum(double n) {
	return IsFin(n) && !IsNull(n);
}

bool IsNum(int n) {
	return !IsNull(n);
}


double DataSource::Min(Getdatafun getdata, int64& id) {
	double minVal = std::numeric_limits<double>::max();
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!!IsNum(d) && minVal > d) {
			minVal = d;	
			id = i;
		}
	}
	if (minVal == std::numeric_limits<double>::max())
		return Null;
	return minVal;		
}
 
double DataSource::Max(Getdatafun getdata, int64& id) {
	double maxVal = std::numeric_limits<double>::lowest();
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d) && maxVal < d) {
			maxVal = d;
			id = i;
		}
	}
	if (maxVal == std::numeric_limits<double>::lowest())
		return Null;
	return maxVal;
}

void DataSource::MaxList(Getdatafun getdataY, Getdatafun getdataX, Vector<int64> &id, double width) {
	id.Clear();
	for (int64 i = 1; i < GetCount() - 1; ++i) {
		double d = Membercall(getdataY)(i);
		if (!IsNum(d))
			continue;
		int64 ii;
		for (ii = i-1; ii >= 0; --ii) {
			if (IsNum(Membercall(getdataY)(ii)))
				break;
		}
		if (ii < 0)
			continue;
		double d_1 = Membercall(getdataY)(ii);
		for (ii = i+1; ii < GetCount(); ++ii) {
			if (IsNum(Membercall(getdataY)(ii)))
				break;
		}
		if (ii >= GetCount())
			continue;
		double d1 = Membercall(getdataY)(ii);
		if (d >= d_1 && d > d1) {
			if (id.GetCount() == 0 || 
				(Membercall(getdataX)(i) - Membercall(getdataX)(id.GetCount() - 1) >= width))
				id << i;
		}
	}
}

Pointf DataSource::MaxSubDataImp(Getdatafun getdataY, Getdatafun getdataX, int64 maxId, int64 width) {
	Vector<Pointf> p;
	
	int iw;
	int64 idMin, idMax;
	iw = 0;
	for (idMin = maxId - 1; idMin >= 0 && iw < width; idMin--) {
		if (!IsNum(Membercall(getdataY)(idMin)) || !IsNum(Membercall(getdataX)(idMin)))
			continue;
		iw++;
	}
	if (idMin < 0)
		idMin = 0;
	iw = 0;
	for (idMax = maxId + 1; idMax < GetCount() && iw < width; idMax++) {
		if (!IsNum(Membercall(getdataY)(idMax)) || !IsNum(Membercall(getdataX)(idMax)))
			continue;
		iw++;
	}
	if (idMax >= GetCount())
		idMax = GetCount() - 1;
	for (int64 i = idMin; i <= idMax; ++i) {
		if (!IsNum(Membercall(getdataY)(i)) || !IsNum(Membercall(getdataX)(i)))
			continue;
		p << Pointf(Membercall(getdataX)(i), Membercall(getdataY)(i));
	}
	VectorPointf pf(p);
	PolynomialEquation2 polyFit;
	if (ExplicitEquation::NoError != polyFit.Fit(pf))
		return Null;
	const Vector<double> &coeff = polyFit.GetCoeff();
	double b = coeff[1];
	double a = coeff[2];
	if (!IsNum(a) || fabs(a) < 1E-10)
		return Null;
	return Pointf(-b/2/a, polyFit.f(-b/2/a));
}

double DataSource::Avg(Getdatafun getdata) {
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d)) {
			ret += d;
			count++;
		}
	}
	if (count == 0) 
		return Null;
	return ret/count;
}

int64 DataSource::Closest(Getdatafun getdata, double dat) {
	double minD = DBL_MAX;
	int64 minDat;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d)) {
			if (minD > abs(d - dat)) {
				minD = abs(d - dat);
				minDat = i;
			}
		}
	}
	if (minD == DBL_MAX)
		return Null;
	return minDat;
}

int64 DataSource::Closest(Getdatafun getdataX, Getdatafun getdataY, double x, double y) {
	double minD = DBL_MAX;
	int64 minDat;
	for (int64 i = 0; i < GetCount(); ++i) {
		double dx = Membercall(getdataX)(i);
		double dy = Membercall(getdataY)(i);
		if (IsNum(dx) && IsNum(dy)) {
			double d = sqr(dx - x) + sqr(dy - y);
			if (minD > d) {
				minD = d;
				minDat = i;
			}
		}
	}
	if (minD == DBL_MAX)
		return Null;
	return minDat;	
}

double DataSource::RMS(Getdatafun getdata) {
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d)) {
			ret += d*d;
			count++;
		}
	}
	if (count == 0) 
		return Null;
	return sqrt(ret/count);
}

double DataSource::IsSorted(Getdatafun getdata) {
	int64 num = GetCount();
	if (num == 0)
		return false;
	if (num == 1)
		return 1;
	for (int i = 1; i < num; ++i) {
		if (Membercall(getdata)(i) < Membercall(getdata)(i - 1))
			return false;
	}
	return true;
}

double DataSource::Variance(Getdatafun getdata, double avg) {
	if (!IsNum(avg))
		avg = Avg(getdata);
	if (!IsNum(avg))
		return Null;
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d)) {
			d -= avg;
			ret += d*d;
			count++;
		}
	}
	if (count <= 1)
		return Null;
	return ret/(count - 1);
}

Vector<int64> DataSource::UpperEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width) {
	VectorXd x, y;
	Copy(getdataX, getdataY, x, y);
	return UpperPeaks(x, y, width);
}

Vector<int64> DataSource::LowerEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width) {
	VectorXd x, y;
	Copy(getdataX, getdataY, x, y);
	return LowerPeaks(x, y, width);
}


Vector<Pointf> DataSource::Cumulative(Getdatafun getdataY, Getdatafun getdataX) {
	Vector<Pointf> ret;
	
	double acum = 0;
	for (int i = 0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		
		if (!IsNum(x) || !IsNum(y))
			continue;
		acum += y;
		ret << Pointf(x, acum);
	}
	return ret;
}

Vector<Pointf> DataSource::CumulativeAverage(Getdatafun getdataY, Getdatafun getdataX) {
	Vector<Pointf> ret;
	
	double acum = 0;
	int num = 0;
	for (int i = 0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		
		if (!IsNum(x) || !IsNum(y))
			continue;
		acum += y;
		num++;
		ret << Pointf(x, acum/num);
	}
	return ret;
}

Vector<Pointf> DataSource::MovingAverage(Getdatafun getdataY, Getdatafun getdataX, double width) {
	Vector<Pointf> ret;
	Vector<double> x, y;
	Copy(getdataX, getdataY, x, y);
	
	Upp::MovingAverage(x, y, width, y);
	for (int i = 0; i < x.size(); ++i) 
		ret << Pointf(x[i], y[i]);
	return ret;	
}

Vector<Pointf> DataSource::SectorAverage(Getdatafun getdataY, Getdatafun getdataX, double width) {
	Vector<Pointf> ret;
	
	for (int i = 0; i < GetCount();) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (!IsNum(x) || !IsNum(y))
			continue;
		
		int numAvg = 1;
		double sum = y;
		double sumX = x;
		int j;
		for (j = i+1; j < GetCount(); ++j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (!IsNum(xnext))
				continue;
			if ((xnext - x) > width) {
				--j;
				break;
			}
			if (!IsNum(ynext))
				continue;
			sumX += xnext;
			sum += ynext;
			numAvg++;
		}
		ret << Pointf(sumX/numAvg, sum/numAvg);
		if (j == GetCount())
			break;
		i = j+1;
	}
	return ret;	
}

void DataSource::ZeroCrossing(Getdatafun getdataY, Getdatafun getdataX, bool ascending, bool descending, 
							  Vector<double> &zeros, Vector<int64> &ids) {
	zeros.Clear();
	ids.Clear();
	
	double y_prev = 0, x_prev = 0;
	int i0;
	for (i0 = 0; i0 < GetCount(); ++i0) {
		y_prev = Membercall(getdataY)(i0);
		x_prev = Membercall(getdataX)(i0);
		if (IsNum(x_prev) && IsNum(y_prev))
			break;
	}
	for (int i = i0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (!IsNum(x) || !IsNum(y))
			continue;
		
		if (((y >= 0 && y_prev < 0) && ascending) || ((y <= 0 && y_prev > 0) && descending)) {
			ids << i;
			zeros << (x_prev - (x - x_prev)*y_prev/(y - y_prev));
		}
		x_prev = x;
		y_prev = y;
	}
}

double DataSource::StdDev(Getdatafun getdata, double avg) {
	double var = Variance(getdata, avg);
	return !IsNum(var) ? Null : sqrt(var);
}

double DataSource::SinEstim_Amplitude(double avg) {
	return sqrt(2.*VarianceY(avg));
}

bool DataSource::SinEstim_FreqPhase(double &frequency, double &phase, double avg) {
	if (GetCount() < 4)
		return false;
	if (!IsNum(avg))
		avg = AvgY();
	int64 firstId;
	for (firstId = 0; firstId < GetCount(); ++firstId) 
		if (!!IsNum(x(firstId)) && !!IsNum(y(firstId)))
			break;
	bool firstIsToPositive = (y(firstId) - avg) < 0;
	bool isPossitive = !firstIsToPositive;
	double T = 0;
	int numT = 0;
	double lastZero = Null;
	double firstZero = Null;
	firstId++;
	for (int64 id = firstId; id < GetCount(); ++id) {
		if (!IsNum(x(id)) || !IsNum(y(id)))
			continue;
		if (((y(id) - avg) > 0) != isPossitive) {
			isPossitive = !isPossitive;
			double zero = x(id-1) - (y(id-1) - avg)*(x(id) - x(id-1))/(y(id) - y(id-1));
			if (!IsNum(lastZero)) 
				firstZero = zero;
			else {
				T += zero - lastZero;
				numT++;
			}
			lastZero = zero;
		}
	}
	if (T == 0 || numT == 0)
		return false;
	T = 2*T/numT;
	frequency = 2*M_PI/T;			
	phase = -frequency*firstZero;
	if (!firstIsToPositive)
		phase += M_PI;
	phase = phase - 2*M_PI*int(phase/(2*M_PI));
	if (phase > M_PI)
		phase = phase - 2*M_PI;
	if (phase < -M_PI)
		phase = 2*M_PI + phase;
	return true;
}

	
double CArray::znFixed(int n, int64 id) {
	if (n == 0)
		return zData[id];
	NEVER();
	return Null;
}

Vector<Pointf> FFTSimple(VectorXd &data, double tSample, bool frequency, int type, 
		int window, int numOver) {
	int numData = int(data.size());
    
    double numDataFact = 0;
    switch (window) {
    case DataSource::HAMMING:	
				numDataFact = HammingWindow<VectorXd>(data);
			    break;
	case DataSource::COS:		
				numDataFact = CosWindow<VectorXd>(data, numOver);
			    break;
	default:	numDataFact = numData;
    }
    Vector<Pointf> res;
    VectorXcd freqbuf;
    try {
	    FFT<double> fft;
	    fft.SetFlag(fft.HalfSpectrum);
	    fft.fwd(freqbuf, data);
    } catch(...) {
        return res;
    }
    double threshold = 0;
    if (type == DataSource::T_PHASE) {
        for (int i = 0; i < int(freqbuf.size()); ++i) {    
            if (threshold < std::abs(freqbuf[i]))
                threshold = std::abs(freqbuf[i]);
        }
    }
    threshold /= 10000.;
    if (frequency) {
    	for (int i = 0; i < int(freqbuf.size()); ++i) {    
    		double xdata = i/(tSample*numData);
		
			switch (type) {
			case DataSource::T_PHASE:	
					if (std::abs(freqbuf[i]) > threshold)
						res << Pointf(xdata, std::arg(freqbuf[i]));
					else
						res << Pointf(xdata, 0);
					break;
			case DataSource::T_FFT:		// Amplitude spectrum
					res << Pointf(xdata, 2*std::abs(freqbuf[i])/numDataFact);		break;
			case DataSource::T_PSD:		// Variance density spectrum
					res << Pointf(xdata, 2*sqr(std::abs(freqbuf[i]))/(numDataFact/tSample)); // 1/2*FFT^2
			}
    	}
    } else {
        for (int i = int(freqbuf.size()) - 1; i > 0; --i) {    
    		double xdata = (tSample*numData)/i;
		
			switch (type) {
			case DataSource::T_PHASE:	
					if (std::abs(freqbuf[i]) > threshold) 
						res << Pointf(xdata, std::arg(freqbuf[i]));
					else
						res << Pointf(xdata, 0);
					break;
			case DataSource::T_FFT:		
					res << Pointf(xdata, 2*std::abs(freqbuf[i])/numDataFact);		break;
			case DataSource::T_PSD:		
					res << Pointf(xdata, 2*sqr(std::abs(freqbuf[i]))/(numDataFact/tSample));
			}
    	}
    }
    return res;
}

Vector<Pointf> DataSource::FFT(Getdatafun getdata, double tSample, bool frequency, int type, 
		int window, int numSub, double overlapping) {
	int numData = int(GetCount());

    VectorXd data(numData);
    int num = 0;
    for (int i = 0; i < numData; ++i) {
        double d = Membercall(getdata)(i);
        if (!!IsNum(d)) {
			data[i] = d;
			num++;
        }
    }
    numData = num;
    
    Vector<Pointf> res;	
    if (num < 3)
        return res;
    data.resize(numData);
	double numOver;
	    
	if (numSub == 1) {
		numOver = numData*overlapping;
		return FFTSimple(data, tSample, frequency, type, window, int(numOver));
	} else {	// solve v t=2*(v-f*v/2) + (n-2)*(v-f*v) ==> v=t/(f + n -f*n)
		double numDataPart = numData/(overlapping + numSub - overlapping*numSub); 
		int inumDataPart = int(numDataPart);
		numOver = numDataPart*overlapping;
		VectorXd dataPart(inumDataPart);
		double izero = 0;
		int izerod = 0;
		Vector<Pointf> fft;
		for (int iPart = 0; iPart < numSub; ++iPart) {
			if (iPart > 0) {
				izero += int(numDataPart - numOver);
				izerod = int(izero);
			}
			for (int i = 0; i < inumDataPart; ++i)
				dataPart[i] = data[izerod + i];
			Vector<Pointf> fftPart;
			fftPart = FFTSimple(dataPart, tSample, frequency, type, window, int(numOver));
			if (iPart == 0)
				fft = clone(fftPart);		// pick()
			else {
				for (int i = 0; i < fftPart.GetCount(); ++i) {
					fft[i].y += fftPart[i].y;
					ASSERT(fft[i].x == fftPart[i].x);
				}
			}
		}
		for (int i = 0; i < fft.GetCount(); ++i) 
			fft[i].y /= numSub;
		return fft;
	}
}

double DataSource::Integral(double from, double to, double n) {
	double h = (to - from)/n;
	double h_2 = h/2;
	   
   	double sum1 = 0, sum2 = 0;
   	for(int i = 0; i < n; i++) {
      	sum1 += f(from + h*i + h_2);
      	sum2 += f(from + h*i);
   	}
   	return h/6*(f(from) + f(to) + 4*sum1 + 2*sum2);
}

double DataSource::Integral(Getdatafun getdataY, Getdatafun getdataX) {
	double prevx = Membercall(getdataX)(0);	
	double prevy = Membercall(getdataY)(0);
	double sum = 0;
	for (int i = 1; i < GetCount(); ++i) {
		double x = Membercall(getdataX)(i);	
		double y = Membercall(getdataY)(i);	
		sum += (x - prevx)*(y + prevy);
		prevx = x;
		prevy = y;
	}
	return sum/2;	
}

void DataSource::GetSpectralMoments(double from, double to, double n, bool frequency, 
									double &m_1, double &m0, double &m1, double &m2) {
	if (!frequency) {
		from = 1/to;
		to = 1/from;
	}
	double h = (to - from)/n;
	double h2 = h/2;
	   
   	double sum1_m_1 = 0, sum2_m_1 = 0;
   	double sum1_m0 = 0, sum2_m0 = 0;
   	double sum1_m1 = 0, sum2_m1 = 0;
   	double sum1_m2 = 0, sum2_m2 = 0;
   	double f1, f2, x1, x2;
   	for(int i = 0; i < n; i++) {
   		if (frequency) {
	   		x1 = from + h*i + h2;
	   		x2 = from + h*i;
	   		f1 = f(x1);
	   		f2 = f(x2);
   		} else {
   			x1 = 1/(from + h*i + h2);
	   		x2 = 1/(from + h*i);
	   		f1 = f(1/x1);
	   		f2 = f(1/x2);
   		}
	   	sum1_m_1 += f1/x1;
      	sum2_m_1 += f2/x2;
      	sum1_m0 += f1;
      	sum2_m0 += f2;
      	sum1_m1 += f1*x1;
      	sum2_m1 += f2*x2;
      	sum1_m2 += f1*x1*x1;
      	sum2_m2 += f2*x2*x2;
   	}
   	double f_from, f_to;
	if (frequency) {
   		f_from = f(from);
   		f_to = f(to);   	
	} else {
   		f_from = f(1/from);
   		f_to = f(1/to);   	
	}
   	m_1 = h/6*(f_from/from 	 	+ f_to/to    + 4*sum1_m_1 + 2*sum2_m_1);
   	m0  = h/6*(f_from 			+ f(to) 	 + 4*sum1_m0  + 2*sum2_m0);
   	m1  = h/6*(f_from*from 	 	+ f_to*to    + 4*sum1_m1  + 2*sum2_m1);
   	m2  = h/6*(f_from*from*from + f_to*to*to + 4*sum1_m2  + 2*sum2_m2);
}
	
void DataSource::GetSpectralMoments(Getdatafun getdataY, Getdatafun getdataX, bool frequency, 
						double &m_1, double &m0, double &m1, double &m2) {
	double prevx = Membercall(getdataX)(0);
	double Si_1 = Membercall(getdataY)(0);
	m_1 = m0 = m1 = m2 = 0;
	for (int i = 1; i < GetCount(); ++i) {
		double x = Membercall(getdataX)(i);
		double Si = Membercall(getdataY)(i);
		
		double deltaX;
		double fi, fi_1;
		if (frequency) {
			fi = x;
			fi_1 = prevx;
			deltaX = fi - fi_1;
		} else {
			fi = 1/x;
			fi_1 = 1/prevx;
			deltaX = fi_1 - fi;
		}
		if (fi != 0 && fi_1 != 0) {
			m_1 += (Si/fi + Si_1/fi_1)*deltaX;
			m0  += (Si + Si_1)*deltaX;
			m1  += (Si*fi + Si_1*fi_1)*deltaX;
			m2  += (Si*fi*fi + Si_1*fi_1*fi_1)*deltaX;
		}
		prevx = x;
		Si_1 = Si;
	}
	m_1 /= 2;
	m0  /= 2;
	m1  /= 2;
	m2  /= 2;
}
			
bool DataSource::SameX(DataSource &data) {
	int64 num = GetCount();
	if (num == 0)
		return false;
	if (data.GetCount() != num)
		return false;
	for (int64 i = 0; i < num; ++i) {
		if (data.x(i) != x(i))
			return false;
	}
	return true;
}

Vector<double> DataSource::SortData(Getdatafun getdata) {
	Vector<double> ret;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (IsNum(d)) {
			ret << d;
			count++;
		}
	}
	if (count == 0) 
		return ret;
	
	Sort(ret);
	
	return ret;	
}

Vector<double> DataSource::Percentile(Getdatafun getdata, double rate) {
	ASSERT(rate >= 0 && rate <= 1);
	Vector<double> data = SortData(getdata);
	int num = int(data.GetCount()*rate) + 1;
	if (num < data.GetCount())
		data.Remove(num, data.GetCount()-num);
	return data;
}

double DataSource::PercentileVal(Getdatafun getdata, double rate) {
	ASSERT(rate >= 0 && rate <= 1);
	
	Vector<double> data = SortData(getdata);
	int num = int(data.GetCount()*rate);
	return LinearInterpolate<double>(data.GetCount()*rate, num, num+1, data[num-1], data[num]);
}

Vector<Pointf> DataSource::Derivative(Getdatafun getdataY, Getdatafun getdataX, int orderDer, int orderAcc) {
	ASSERT(orderDer >= 1 && orderDer <= 2);
	ASSERT(orderAcc == 2 || orderAcc == 4 || orderAcc == 6 || orderAcc == 8);
	
	int numData = int(GetCount());

    VectorXd xv(numData), yv(numData);
    for (int i = 0; i < numData; ++i) {
        yv[i] = Membercall(getdataY)(i);
        xv[i] = Membercall(getdataX)(i);
    }

	CleanNANDupXSort(xv, yv, xv, yv);   
    Resample(xv, yv, xv, yv);
	double from = xv[0];
	
	// From https://en.wikipedia.org/wiki/Finite_difference_coefficient
	double kernels1[4][9] = {{-1/2., 0, 1/2.},
		{1/12., -2/3., 0, 2/3., -1/12.},
		{-1/60., 3/20., -3/4., 0,  3/4., -3/20., 1/60.},
		{1/280., -4/105., 1/5., -4/5., 0., 4/5., -1/5., 4/105., -1/280.}};
	double kernels2[4][9] = {{1, -2, 1},
		{-1/12., 4/3., -5/2., 4/3., -1/12.},
		{1/90., -3/20., 3/2., -49/18., 3/2., -3/20., 1/90.},
		{-1/560., 8/315., -1/5., 8/5., -205/72., 8/5., -1/5., 8/315., -1/560.}};					
	
	int idkernel = orderAcc/2-1;
	
	Vector<Pointf> res;	
	
	double factor;
	VectorXd kernel;
	double h = xv[1]-xv[0];
	if (orderDer == 1) {
		factor = 1/h;
		kernel = Map<MatrixXd>(kernels1[idkernel], orderAcc+1, 1);
	} else if (orderDer == 2) { 	
		factor = 1/h/h;
		kernel = Map<MatrixXd>(kernels2[idkernel], orderAcc+1, 1);
	} else
		return res;
	
	VectorXd resE = Convolution(yv, kernel, factor);
	
	int nnumData = int(yv.size())-orderAcc;
	res.SetCount(nnumData);
	int frame = orderAcc/2 + 1;
	for (int i = 0; i < nnumData; ++i) {
		res[i].y = resE(i);
		res[i].x = from + (i+frame)*h;
	}
	return res;
}

bool SavitzkyGolay_CheckParams(int nleft, int nright, int deg, int der) {
	return nleft >= 0 && nright >= 0 && der <= deg && nleft + nright >= deg;
}

VectorXd SavitzkyGolay_Coeff(int nleft, int nright, int deg, int der) {
    ASSERT(SavitzkyGolay_CheckParams(nleft, nright, deg, der));
	
	int cols = deg + 1;
    MatrixXd A(cols, cols);
    
    for(int ipj = 0; ipj <= (deg << 1); ipj++) {
        double sum = ipj ? 0 : 1;
        for(int k = 1; k <= nright; k++) 
        	sum += pow(k, ipj);
        for(int k = 1; k <= nleft; k++) 
        	sum += pow(-k, ipj);
        int mm = min(ipj, 2 * deg - ipj);
        for(int imj = -mm; imj <= mm; imj += 2) 
        	A((ipj+imj)/2, (ipj-imj)/2) = sum;
    }
    
    MatrixXd At = A.transpose();
	VectorXd B(VectorXd::Zero(cols));
    B[der] = 1;
    VectorXd y = (At * A).inverse() * (At * B);
    
    VectorXd coeff(nleft + nright + 1);
    double factor = der > 0 ? pow(2, der-1) : 1;
    coeff.setZero();
    int ic = 0;
    for(int k = -nleft; k <= nright; k++) {
        double sum = y[0];
        double fac = 1;
        for(int mm = 1; mm <= deg; mm++) 
        	sum += y[mm]*(fac *= k);
        coeff[ic++] = sum*factor;
    }
    return coeff;
}

bool SavitzkyGolay_Check(const VectorXd &coeff) {
	double unity = coeff.sum();
	return abs(1-unity) < 0.0000001 || abs(unity) < 0.0000001;  
}

Vector<Pointf> DataSource::SavitzkyGolay(Getdatafun getdataY, Getdatafun getdataX, int deg, int size, int der) {
	Vector<Pointf> res;
	
	int numData = int(GetCount());

    VectorXd xv(numData), yv(numData);
    for (int i = 0; i < numData; ++i) {
        yv[i] = Membercall(getdataY)(i);
        xv[i] = Membercall(getdataX)(i);
    }

	CleanNANDupXSort(xv, yv, xv, yv);
    Resample(xv, yv, xv, yv);
	double from = xv[0];
    
    VectorXd coeff = SavitzkyGolay_Coeff(size/2, size/2, deg, der);
	if (!SavitzkyGolay_Check(coeff))
		return res;
	
	double h = xv[1]-xv[0];
	VectorXd resE = Convolution(yv, coeff, 1/pow(h, der));

	int nnumData = int(yv.size())-size;
	res.SetCount(nnumData);
	int frame = size/2;
	for (int i = 0; i < nnumData; ++i) {
		res[i].y = resE(i);
		res[i].x = from + (i+frame)*h;
	}
	return res;
}

double LinearInterpolate(double x, const VectorXd &vecx, const VectorXd &vecy) {
	ASSERT(vecx.size() > 1);
	ASSERT(vecx.size() == vecy.size());
	return LinearInterpolate(x, vecx.data(), vecy.data(), vecx.size());
}

void Resample(const VectorXd &x, const VectorXd &y, VectorXd &rrx, VectorXd &rry, double srate) {
	VectorXd rx, ry;
		
	double delta = x[x.size()-1] - x[0];
	if (!IsNum(srate)) 
		srate = delta/(x.size()-1);
	int len = int(delta/srate) + 1;
	rx.resize(len);
	ry.resize(len);
	for (int i = 0; i < len; ++i) { 
		rx[i] = x[0] + i*srate;
		ry[i] = LinearInterpolate(rx[i], x, y);
	}
	rrx = pick(rx);
	rry = pick(ry);
}

void Resample(const VectorXd &x, const VectorXd &y, const VectorXd &z, 
			  VectorXd &rrx, VectorXd &rry, VectorXd &rrz, double srate) {
	VectorXd rx, ry, rz;
		
	double delta = x[x.size()-1] - x[0];
	if (!IsNum(srate)) 
		srate = delta/(x.size()-1);
	int len = int(delta/srate) + 1;
	rx.resize(len);
	ry.resize(len);
	rz.resize(len);
	for (int i = 0; i < len; ++i) { 
		rx[i] = x[0] + i*srate;
		ry[i] = LinearInterpolate(rx[i], x, y);
		rz[i] = LinearInterpolate(rx[i], x, z);
	}
	rrx = pick(rx);
	rry = pick(ry);
	rrz = pick(rz);
}
		
void FilterFFT(VectorXd &data, double T, double fromT, double toT) {
	double samplingFrecuency = 1/T;
	
	size_t numData = data.size();
    VectorXcd freqbuf;
    Eigen::FFT<double> fft;
    fft.SetFlag(fft.HalfSpectrum);
    fft.fwd(freqbuf, data);
	
    for (int i = 0; i < freqbuf.size(); ++i) {
        double freq = i*samplingFrecuency/numData;
        double T = 1/freq;
        if ((!IsNum(fromT) || T > fromT) && (!IsNum(toT) || T < toT))
            freqbuf[i] = 0;
    }
	fft.inv(data, freqbuf);
}

Vector<Pointf> DataSource::FilterFFT(Getdatafun getdataY, Getdatafun getdataX, double fromT, double toT) {
	Vector<Pointf> res;
	ASSERT(!!IsNum(fromT) || !!IsNum(toT));
	if (!IsNum(fromT) && !IsNum(toT))
		return res;
	
	int numData = int(GetCount());

    VectorXd xv(numData), yv(numData);
    for (int i = 0; i < numData; ++i) {
        yv[i] = Membercall(getdataY)(i);
        xv[i] = Membercall(getdataX)(i);
    }
    
	CleanNANDupXSort(xv, yv, xv, yv);  
    Resample(xv, yv, xv, yv);
	double from = xv[0];
	double T = xv[1]-xv[0];
    
	Upp::FilterFFT(yv, T, fromT, toT);
	int nnumData = int(yv.size());
	res.SetCount(nnumData);
	for (int i = 0; i < nnumData; ++i) {
		res[i].x = from + i*T;
		res[i].y = yv[i];
	}
	return res;
}

void ExplicitData::Init(Function<double (double x, double y)> _funz, double _minX, double _maxX, double _minY, double _maxY) {
	ASSERT(maxX >= minX && maxY >= minY);
	this->funz = _funz;
	this->minX = _minX;
	this->maxX = _maxX;
	this->minY = _minY;
	this->maxY = _maxY;
	
	minZ = std::numeric_limits<double>::max();
	maxZ = std::numeric_limits<double>::lowest();
	double deltax = (_maxX - _minX)/100.;
	double deltay = (_maxY - _minY)/100.;
	for (double x = _minX; x <= _maxX; x += deltax) {
		for (double y = _minY; y <= _maxY; y += deltay) {
			double z = _funz(x, y);
			if (!!IsNum(z)) {
				minZ = min(minZ, z);
				maxZ = max(maxZ, z);
			}
		}
	}
}

int FindClosest(Pointf &p, Vector<Pointf> &points, double deltaX, double deltaY, double &d) {
	double dxmin = std::numeric_limits<double>::max();
	double dymin = std::numeric_limits<double>::max();
	int imin = -1;
	for (int i = 0; i < points.GetCount(); ++i) {
		double dx = abs(p.x - points[i].x);		
		double dy = abs(p.y - points[i].y);		
		if (dx*dx + dy*dy < dxmin*dxmin + dymin*dymin) {
			dxmin = dx;
			dymin = dy;
			imin = i;
		}
	}
	if ((dxmin > deltaX*1.00000000001) || (dymin > deltaY*1.00000000001)) 
		return Null;
	d = sqrt(dxmin*dxmin + dymin*dymin);
	return imin;
}

Vector<Pointf> DataSourceSurf::GetIsolines(const Vector<double> &vals, const Rectf &area, double deltaX, double deltaY) {	
	Vector<Pointf> isolines;
	for (int i = 0; i < vals.GetCount(); ++i) {
		if (i > 0)
			isolines << Null;
		Vector<Pointf> isoaux = GetIsoline(vals[i], area, deltaX, deltaY);
		isolines.Append(isoaux);
	}
	return isolines;
}

Vector<Pointf> DataSourceSurf::GetIsoline(double thres, const Rectf &area, double deltaX, double deltaY) {
	Vector<double> zp;

	int width = (int)(area.GetWidth()/deltaX) + 1;
	int height = -(int)(area.GetHeight()/deltaY) + 1;
	zp.SetCount(width*height);
	int iy = 0;
	for (double y = area.bottom; iy < height; y += deltaY, iy++) {
		int ix = 0;
		for (double x = area.left; ix < width; x += deltaX, ix++) 
			zp[ix + iy*width] = z(x, y);
	}
	
	Vector<Pointf> points;
	for (int iy = 0; iy < height; iy++) {
		for (int ix = 0; ix < width-1; ix++) {
			double z0 = zp[ix + iy*width];
			double z1 = zp[ix+1 + iy*width];
			if (!IsNum(z0) || !IsNum(z1))
				continue;
			if ((z1 > thres && z0 <= thres) || (z0 > thres && z1 <= thres)) {
				double delta = abs(thres - z0)/abs(z1 - z0);
				points << Pointf(area.left + (ix + delta)*deltaX, area.bottom + iy*deltaY);
			}
		}
	}
	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height-1; iy++) {
			double z0 = zp[ix + iy*width];
			double z1 = zp[ix + (iy+1)*width];
			if (!IsNum(z0) || !IsNum(z1))
				continue;
			if ((z1 > thres && z0 <= thres) || (z0 > thres && z1 <= thres)) {
				double delta = abs(thres - z0)/abs(z1 - z0);
				points << Pointf(area.left + ix*deltaX, area.bottom + (iy + delta)*deltaY);
			}
		}
	}
	if (points.IsEmpty())
		return points;
	
	Vector<Pointf> isoline;
	isoline << points[0];
	points.Remove(0);
	while (!points.IsEmpty()) {
		int imin;
		double dt, d0;
		int iminT = FindClosest(isoline.Top(), points, deltaX, deltaY, dt);
		int imin0 = FindClosest(isoline[0], points, deltaX, deltaY, d0);
		if (!IsNum(iminT) && !IsNum(imin0)) {	
			isoline << Null;
			imin = 0;
		} else if (!IsNum(iminT)) {
			Reverse(isoline);
			imin = imin0;
		} else if (!IsNum(imin0))
			imin = iminT;
		else {
			if (dt > d0) {
		 		Reverse(isoline);
				imin = imin0;
			} else
				imin = iminT;
		}
		isoline << points[imin];
		points.Remove(imin);
	}
	return isoline;
}
  
Pointf Intersection(Pointf &a, Pointf &b, Pointf &c, Pointf &d) {
    // Line AB represented as a1x + b1y = c1
    double a1 = b.y - a.y;
    double b1 = a.x - b.x;
    double c1 = a1*(a.x) + b1*(a.y);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = d.y - c.y;
    double b2 = c.x - d.x;
    double c2 = a2*(c.x)+ b2*(c.y);
 
    double det = a1*b2 - a2*b1;
 
    if (det == 0) // Parallel
        return Null;
    else {
        double x = (b2*c1 - b1*c2)/det;
        double y = (a1*c2 - a2*c1)/det;
        return Pointf(x, y);
    }
}
 
Pointf SegIntersection(Pointf &a, Pointf &b, Pointf &c, Pointf &d) {
	Pointf inter = Intersection(a, b, c, d); 	
	if (IsNull(inter))
		return Null;
    if (((a.x <= inter.x && b.x >= inter.x) || (b.x <= inter.x && a.x >= inter.x)) &&
    	((a.y <= inter.y && b.y >= inter.y) || (b.y <= inter.y && a.y >= inter.y)) &&
    	((c.x <= inter.x && d.x >= inter.x) || (d.x <= inter.x && c.x >= inter.x)) &&
    	((c.y <= inter.y && d.y >= inter.y) || (d.y <= inter.y && c.y >= inter.y)))
    	return inter;
    else
		return Null;
}

Vector<Pointf> Intersection(Vector<Pointf> &poly1, Vector<Pointf> &poly2) {
	Vector<Pointf> listInter;
	for (int i1 = 0; i1 < poly1.GetCount() - 1; ++i1) {
		for (int i2 = 0; i2 < poly2.GetCount() - 1; ++i2) {
			Pointf inter = SegIntersection(poly1[i1], poly1[i1+1], poly2[i2], poly2[i2+1]);
			if (!IsNull(inter)) {
				bool found = false;
				for (int i = 0; i < listInter.GetCount(); ++i) {
					if (abs((inter.x - listInter[i].x)/inter.x) < 0.000001 && 
						abs((inter.y - listInter[i].y)/inter.y) < 0.000001) {	
						found = true;
						break;
					}
				}
				if (!found)
					listInter << inter;
			}
		}
	}
	return listInter;
}

void Simplify(Vector<Pointf> &poly, double dx, double dy) {
	for (int i = 1; i < poly.GetCount(); ++i) {
		if (abs(poly[i].x - poly[i-1].x) < dx && abs(poly[i].y - poly[i-1].y) < dy) {
			poly.Remove(i, 1);		
			i--;
		}
	}
}

}
