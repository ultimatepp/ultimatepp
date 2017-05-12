#include "ScatterDraw.h"

#include <plugin/Eigen/Eigen.h>

namespace Upp {

using namespace Eigen;

#define Membercall(fun)	(this->*fun)

double DataSource::Min(Getdatafun getdata, int64& id) {
	double minVal = -DOUBLE_NULL;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && minVal > d) {
			minVal = d;
			id = i;
		}
	}
	if (minVal == -DOUBLE_NULL)
		return Null;
	return minVal;		
}
 
double DataSource::Max(Getdatafun getdata, int64& id) {
	double maxVal = DOUBLE_NULL;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d) && maxVal < d) {
			maxVal = d;
			id = i;
		}
	}
	if (maxVal == DOUBLE_NULL)
		return Null;
	return maxVal;
}

void DataSource::MaxList(Getdatafun getdataY, Getdatafun getdataX, Vector<int64> &id, double width) {
	id.Clear();
	for (int64 i = 1; i < GetCount() - 1; ++i) {
		double d = Membercall(getdataY)(i);
		if (IsNull(d))
			continue;
		int64 ii;
		for (ii = i-1; ii >= 0; --ii) {
			if (!IsNull(Membercall(getdataY)(ii)))
				break;
		}
		if (ii < 0)
			continue;
		double d_1 = Membercall(getdataY)(ii);
		for (ii = i+1; ii < GetCount(); ++ii) {
			if (!IsNull(Membercall(getdataY)(ii)))
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

Pointf DataSource::MaxSubDataImp(Getdatafun getdataY, Getdatafun getdataX, int64 maxId, int64 width)
{
	Vector<Pointf> p;
	
	int iw;
	int64 idMin, idMax;
	iw = 0;
	for (idMin = maxId - 1; idMin >= 0 && iw < width; idMin--) {
		if (IsNull(Membercall(getdataY)(idMin)) || IsNull(Membercall(getdataX)(idMin)))
			continue;
		iw++;
	}
	if (idMin < 0)
		idMin = 0;
	iw = 0;
	for (idMax = maxId + 1; idMax < GetCount() && iw < width; idMax++) {
		if (IsNull(Membercall(getdataY)(idMax)) || IsNull(Membercall(getdataX)(idMax)))
			continue;
		iw++;
	}
	if (idMax >= GetCount())
		idMax = GetCount() - 1;
	for (int64 i = idMin; i <= idMax; ++i) {
		if (IsNull(Membercall(getdataY)(i)) || IsNull(Membercall(getdataX)(i)))
			continue;
		p << Pointf(Membercall(getdataX)(i), Membercall(getdataY)(i));
	}
	VectorPointf pf(p);
	PolynomialEquation2 polyFit;
	polyFit.Fit(pf);
	const Vector<double> &coeff = polyFit.GetCoeff();
	double b = coeff[1];
	double a = coeff[2];
	return Pointf(-b/2/a, polyFit.f(-b/2/a));
}

double DataSource::Avg(Getdatafun getdata) {
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d)) {
			ret += d;
			count++;
		}
	}
	if (count == 0) 
		return Null;
	return ret/count;
}

double DataSource::RMS(Getdatafun getdata) {
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d)) {
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
	if (IsNull(avg))
		avg = Avg(getdata);
	if (IsNull(avg))
		return Null;
	double ret = 0;
	int count = 0;
	for (int64 i = 0; i < GetCount(); ++i) {
		double d = Membercall(getdata)(i);
		if (!IsNull(d)) {
			d -= avg;
			ret += d*d;
			count++;
		}
	}
	if (count <= 0)
		return Null;
	return ret/(count - 1);
}

Vector<int64> DataSource::Envelope(Getdatafun getdataY, Getdatafun getdataX, double width, bool (*fun)(double a, double b)) {
	Vector<int64> ret;
	double width_2 = width/2.;
		
	for (int i = 0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (IsNull(x) || IsNull(y))
			continue;
		int numComparisons = 0;
		for (int j = i-1; j >= 0; --j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (IsNull(xnext) || IsNull(ynext))
				continue;
			if ((x - xnext) > width_2)
				break;
			if (!fun(y, ynext)) {
				numComparisons = Null;
				break;
			}
			numComparisons++;
		}
		if (IsNull(numComparisons))
			continue;
		for (int j = i+1; j < GetCount(); ++j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (IsNull(xnext) || IsNull(ynext))
				continue;
			if ((xnext - x) > width_2)
				break;
			if (!fun(y, ynext)) {
				numComparisons = Null;
				break;
			}
			numComparisons++;
		}
		if (IsNull(numComparisons))
			continue;
		if (numComparisons > 2) {
			if (!ret.IsEmpty()) {
				int64 prev_i = ret[ret.GetCount() - 1];
				if (Membercall(getdataX)(prev_i) != x)
					ret << i;
			} else 
				ret << i;
		}
	}
	return ret;
}

bool GreaterEqualThan(double a, double b) {return a >= b;}
bool LowerEqualThan(double a, double b) {return a <= b;}

Vector<int64> DataSource::UpperEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width) {return Envelope(getdataY, getdataX, width, GreaterEqualThan);}
Vector<int64> DataSource::LowerEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width) {return Envelope(getdataY, getdataX, width, LowerEqualThan);}

Vector<Pointf> DataSource::MovingAverage(Getdatafun getdataY, Getdatafun getdataX, double width) {
	Vector<Pointf> ret;
	double width_2 = width/2.;
	
	for (int i = 0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (IsNull(x) || IsNull(y))
			continue;
		int numAvg = 1;
		double sum = y;
		int j;
		for (j = i-1; j >= 0; --j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (IsNull(xnext) || IsNull(ynext))
				continue;
			if ((x - xnext) > width_2)
				break;
			sum += ynext;
			numAvg++;
		}
		if (j < 0)
			continue;
		for (j = i+1; j < GetCount(); ++j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (IsNull(xnext))
				continue;
			if ((xnext - x) > width_2) 
				break;
			if (IsNull(ynext))
				continue;
			sum += ynext;
			numAvg++;
		}
		if (j == GetCount())
			continue;
		ret << Pointf(x, sum/numAvg);
	}
	return ret;	
}

Vector<Pointf> DataSource::SectorAverage(Getdatafun getdataY, Getdatafun getdataX, double width) {
	Vector<Pointf> ret;
	
	for (int i = 0; i < GetCount();) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (IsNull(x) || IsNull(y))
			continue;
		
		int numAvg = 1;
		double sum = y;
		double sumX = x;
		int j;
		for (j = i+1; j < GetCount(); ++j) {
			double ynext = Membercall(getdataY)(j);
			double xnext = Membercall(getdataX)(j);
			if (IsNull(xnext))
				continue;
			if ((xnext - x) > width) {
				--j;
				break;
			}
			if (IsNull(ynext))
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
	
	double y_prev, x_prev;
	int i0;
	for (i0 = 0; i0 < GetCount(); ++i0) {
		y_prev = Membercall(getdataY)(i0);
		x_prev = Membercall(getdataX)(i0);
		if (!IsNull(x_prev) && !IsNull(y_prev))
			break;
	}
	for (int i = i0; i < GetCount(); ++i) {
		double y = Membercall(getdataY)(i);
		double x = Membercall(getdataX)(i);
		if (IsNull(x) || IsNull(y))
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
	return IsNull(var) ? Null : sqrt(var);
}

double DataSource::SinEstim_Amplitude(double avg) {
	return sqrt(2.*VarianceY(avg));
}

bool DataSource::SinEstim_FreqPhase(double &frequency, double &phase, double avg) {
	if (GetCount() < 4)
		return false;
	if (IsNull(avg))
		avg = AvgY();
	int64 firstId;
	for (firstId = 0; firstId < GetCount(); ++firstId) 
		if (!IsNull(x(firstId)) && !IsNull(y(firstId)))
			break;
	bool firstIsToPositive = (y(firstId) - avg) < 0;
	bool isPossitive = !firstIsToPositive;
	double T = 0;
	int numT = 0;
	double lastZero = Null;
	double firstZero;
	firstId++;
	for (int64 id = firstId; id < GetCount(); ++id) {
		if (IsNull(x(id)) || IsNull(y(id)))
			continue;
		if (((y(id) - avg) > 0) != isPossitive) {
			isPossitive = !isPossitive;
			double zero = x(id-1) - (y(id-1) - avg)*(x(id) - x(id-1))/(y(id) - y(id-1));
			if (IsNull(lastZero)) 
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
				for (int i = 0; i < numData; ++i) {
			        double windowFact = 0.54 - 0.46*cos(2*M_PI*i/numData);
			        numDataFact += windowFact;
			    	data[i] *= windowFact;	
			    }
			    break;
	case DataSource::COS:		
				for (int i = 0; i < numOver; ++i) {
			        double windowFact = 0.5*(1 - cos(M_PI*i/numOver));
			        numDataFact += windowFact;
			    	data[i] *= windowFact;	
			    }
			    for (int i = numOver; i < numData - numOver; ++i) {
			        double windowFact = 1;		// 1.004
			        numDataFact += windowFact;
			    	//data[i] *= windowFact;	
			    }
			    for (int i = numData - numOver; i < numData; ++i) {
			  		double windowFact = 0.5*(1 + cos(M_PI*(numData - i - numOver)/numOver));
			        numDataFact += windowFact;
			    	data[i] *= windowFact;	
			    }
			    break;
	default:	numDataFact = numData;
    }
    Vector<Pointf> res;
    VectorXcd freqbuf;
    try {
	    Eigen::FFT<double> fft;
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
        if (!IsNull(d)) {
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
	double h2 = h/2;
	   
   	double sum1 = 0, sum2 = 0;
   	for(int i = 0; i < n; i++) {
      	sum1 += f(from + h*i + h2);
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
	if (data.GetCount() != num)
		return false;
	for (int64 i = 0; i < num; ++i) {
		if (data.x(i) != x(i))
			return false;
	}
	return true;
}

}