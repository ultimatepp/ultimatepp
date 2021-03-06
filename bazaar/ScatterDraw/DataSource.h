#ifndef _ScatterDraw_DataSource_h_
#define _ScatterDraw_DataSource_h_

#include <plugin/Eigen/Eigen.h>
#include <numeric>

namespace Upp {

#define Membercall(fun)	(this->*fun)

bool IsNum(double n);
bool IsNum(int n);

class DataSource : public Pte<DataSource>  {
public:
	typedef double (DataSource::*Getdatafun)(int64 id);

	DataSource() : isParam(false), isExplicit(false) {}
	virtual ~DataSource() noexcept				{magic = 4321234;}	
	virtual double y(int64 ) = 0;
	virtual double x(int64 ) = 0;
	virtual double znx(int , int64 ) const		{NEVER();	return Null;}
	virtual double zny(int , int64 ) const		{NEVER();	return Null;}
	virtual double znFixed(int , int64 )		{NEVER();	return Null;}
	virtual double y(double ) 					{NEVER();	return Null;}
	virtual double x(double ) 					{NEVER();	return Null;}
	virtual double f(double ) 					{NEVER();	return Null;}
	virtual double f(Vector<double> ) 			{NEVER();	return Null;}
	virtual int64 GetCount() const = 0;
	bool IsEmpty() const						{return GetCount() == 0;}
	virtual int GetznxCount(int64 ) const		{return 0;}
	virtual int GetznyCount(int64 ) const		{return 0;}
	virtual int GetznFixedCount() const			{return 0;}
	bool IsParam() const						{return isParam;}
	bool IsExplicit() const						{return isExplicit;}

	void SetDestructor(Function <void(void)> _OnDestructor) {OnDestructor = _OnDestructor;}

	template <class Range>
	void Copy(Getdatafun getdata, Range &out) {
		Resize(out, GetCount());
		int n = 0;		
		for (int64 i = 0; i < GetCount(); ++i) { 
			double d = Membercall(getdata)(i);
			if (IsNum(d)) 
				out[n++] = d;
		}
		ResizeConservative(out, n);
	}
	template <class Range>
	void Copy(Getdatafun getdata0, Getdatafun getdata1, Range &out0, Range &out1) {
		Resize(out0, GetCount());
		Resize(out1, GetCount());
		int n = 0;		
		for (int64 i = 0; i < GetCount(); ++i) { 
			double d0 = Membercall(getdata0)(i);
			double d1 = Membercall(getdata1)(i);
			if (IsNum(d0) && IsNum(d1)) {
				out0[n] = d0;
				out1[n] = d1;
				n++;
			}
		}
		ResizeConservative(out0, n);
		ResizeConservative(out1, n);
	}
	template <class Range>
	void Copy(Getdatafun getdata0, Getdatafun getdata1, Getdatafun getdata2, Range &out0, Range &out1, Range &out2) {
		Resize(out0, GetCount());
		Resize(out1, GetCount());
		Resize(out2, GetCount());
		int n = 0;		
		for (int64 i = 0; i < GetCount(); ++i) { 
			double d0 = Membercall(getdata0)(i);
			double d1 = Membercall(getdata1)(i);
			double d2 = Membercall(getdata2)(i);
			if (IsNum(d0) && IsNum(d1) && IsNum(d2)) {
				out0[n] = d0;
				out1[n] = d1;
				out2[n] = d2;
				n++;
			}
		}
		ResizeConservative(out0, n);
		ResizeConservative(out1, n);
		ResizeConservative(out2, n);
	}
		
	double MinY(int64& id)  		{return Min(&DataSource::y, id);}
	double MinY()  					{int64 dummy;	return Min(&DataSource::y, dummy);}
	double MinX(int64& id)  		{return Min(&DataSource::x, id);}	
	double MinX() 					{int64 dummy;	return Min(&DataSource::x, dummy);}

	double MaxY(int64& id)  		{return Max(&DataSource::y, id);}
	double MaxY()  					{int64 dummy;	return Max(&DataSource::y, dummy);}
	double MaxX(int64& id)  		{return Max(&DataSource::x, id);}	
	double MaxX() 					{int64 dummy;	return Max(&DataSource::x, dummy);}	
	
	double IsSortedY()  			{return IsSorted(&DataSource::y);}		
	double IsSortedX()  			{return IsSorted(&DataSource::x);}	
	
	int64 Closest(double x,double y){return Closest(&DataSource::x, &DataSource::y, x, y);}	
	int64 ClosestY(double d)  		{return Closest(&DataSource::y, d);}		
	int64 ClosestX(double d)  		{return Closest(&DataSource::x, d);}
	double AvgY()  					{return Avg(&DataSource::y);}		
	double AvgX()  					{return Avg(&DataSource::x);}	
	double RMSY()  					{return RMS(&DataSource::y);}			
	double StdDevY(double avg = Null)  				{return StdDev(&DataSource::y, avg);}
	double StdDevX(double avg = Null)  				{return StdDev(&DataSource::x, avg);}
	double VarianceY(double avg = Null)  			{return Variance(&DataSource::y, avg);}	
	Vector<int64> UpperEnvelopeY(double width)  	{return UpperEnvelope(&DataSource::y, &DataSource::x, width);}	
	Vector<int64> LowerEnvelopeY(double width)  	{return LowerEnvelope(&DataSource::y, &DataSource::x, width);}	
	Vector<Pointf> CumulativeY() 					{return Cumulative(&DataSource::y, &DataSource::x);}
	Vector<Pointf> CumulativeAverageY()  			{return CumulativeAverage(&DataSource::y, &DataSource::x);}
	Vector<Pointf> MovingAverageY(double width)  	{return MovingAverage(&DataSource::y, &DataSource::x, width);}
	Vector<Pointf> SectorAverageY(double width)  	{return SectorAverage(&DataSource::y, &DataSource::x, width);}	
	void MaxListY(Vector<int64> &id, double width) 	{MaxList(&DataSource::y, &DataSource::x, id, width);}
	Pointf MaxSubDataImpY(int64 maxId, int width) 	{return MaxSubDataImp(&DataSource::y, &DataSource::x, maxId, width);}
	void ZeroCrossingY(bool ascending, bool descending, Vector<double> &zeros, Vector<int64> &ids)  {
		return ZeroCrossing(&DataSource::y, &DataSource::x, ascending, descending, zeros, ids);}
	double IntegralY() 			{return Integral(&DataSource::y, &DataSource::x);}
	double IntegralY(double from, double to, double n) 	{return Integral(from, to, n);}

	enum FFT_WINDOW {NO_WINDOW = 0, HAMMING, COS};
	enum FFT_TYPE   {T_FFT = 0, T_PHASE, T_PSD};

	Upp::Vector<Pointf> FFTY(double tSample, bool frequency = false, int type = FFT_TYPE::T_FFT, 
					int window = FFT_WINDOW::HAMMING, int numSub = 1, double overlapping = 0)  {
		return FFT(&DataSource::y, tSample, frequency, type, window, numSub, overlapping);}
	static int GetFFTWindowCount() 			  {return 3;}
	static const char *GetFFTWindowStr(int i) {
		const char *str[] = {"no window", "hamming", "cos"};
		if (i < 0 || i >= GetFFTWindowCount())
			return 0;
		return str[i];
	}
	void GetSpectralMomentsY(double from, double to, double n, bool frequency, 
									double &m_1, double &m0, double &m1, double &m2)  
		{GetSpectralMoments(from, to, n, frequency, m_1, m0, m1, m2);}
	void GetSpectralMomentsY(bool frequency, double &m_1, double &m0, double &m1, double &m2) 
		{GetSpectralMoments(&DataSource::y, &DataSource::x, frequency, m_1, m0, m1, m2);}
		
	Vector<double> SortDataY() 				{return SortData(&DataSource::y);}
	Vector<double> PercentileY(double rate) {return Percentile(&DataSource::y, rate);}
	double PercentileValY(double rate) 		{return PercentileVal(&DataSource::y, rate);}
	
	Vector<Pointf> DerivativeY(int orderDer, int orderAcc)	  {return Derivative(&DataSource::y, &DataSource::x, orderDer, orderAcc);}
	Vector<Pointf> SavitzkyGolayY(int deg, int size, int der) {return SavitzkyGolay(&DataSource::y, &DataSource::x, deg, size, der);}
	Vector<Pointf> FilterFFTY(double fromT, double toT) 	  {return FilterFFT(&DataSource::y, &DataSource::x, fromT, toT);}
			
	double Min(Getdatafun getdata, int64& id);
	double Max(Getdatafun getdata, int64& id);
	double Avg(Getdatafun getdata);
	int64 Closest(Getdatafun getdata, double d);
	int64 Closest(Getdatafun getdataX, Getdatafun getdataY, double x, double y);
	double IsSorted(Getdatafun getdata);
	double RMS(Getdatafun getdata);
	double StdDev(Getdatafun getdata, double avg = Null);
	double Variance(Getdatafun getdata, double avg = Null);
	Vector<int64> UpperEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width);
	Vector<int64> LowerEnvelope(Getdatafun getdataY, Getdatafun getdataX, double width);
	Vector<Pointf> Cumulative(Getdatafun getdataY, Getdatafun getdataX);
	Vector<Pointf> CumulativeAverage(Getdatafun getdataY, Getdatafun getdataX);
	Vector<Pointf> MovingAverage(Getdatafun getdataY, Getdatafun getdataX, double width);
	Vector<Pointf> SectorAverage(Getdatafun getdataY, Getdatafun getdataX, double width);
	void MaxList(Getdatafun getdataY, Getdatafun getdataX, Vector<int64> &id, double width);
	Pointf MaxSubDataImp(Getdatafun getdataY, Getdatafun getdataX, int64 maxId, int64 width);
	void ZeroCrossing(Getdatafun getdataY, Getdatafun getdataX, bool ascending, bool descending,
							  Vector<double> &zeros, Vector<int64> &ids);
	double Integral(Getdatafun getdataY, Getdatafun getdataX);						  
	double Integral(double from, double to, double n);
	
	double SinEstim_Amplitude(double avg = Null);
	bool SinEstim_FreqPhase(double &frequency, double &phase, double avg = Null);
	Vector<Pointf> FFT(Getdatafun getdata, double tSample, bool frequency = false, 
					   int type = FFT_TYPE::T_FFT, int window = FFT_WINDOW::HAMMING, 
					   int numSub = 1, double overlapping = 0);
	void GetSpectralMoments(double from, double to, double n, bool frequency, 
						double &m_1, double &m0, double &m1, double &m2);
	void GetSpectralMoments(Getdatafun getdataY, Getdatafun getdataX, bool frequency, 
						double &m_1, double &m0, double &m1, double &m2);
	bool SameX(DataSource &data);
	Vector<double> SortData(Getdatafun getdata);
	Vector<double> Percentile(Getdatafun getdata, double rate);
	double PercentileVal(Getdatafun getdata, double rate);
	Vector<Pointf> Derivative(Getdatafun getdataY, Getdatafun getdataX, int orderDer, int orderAcc);
	Vector<Pointf> SavitzkyGolay(Getdatafun getdataY, Getdatafun getdataX, int deg, int size, int der);
	Vector<Pointf> FilterFFT(Getdatafun getdataY, Getdatafun getdataX, double fromT, double toT);
	
	bool IsMagic() 				{return magic == 1234321;}		// Temporal use, just for testing
	
protected:
	bool isParam, isExplicit;
	
private:
	Function <void(void)> OnDestructor;
	Vector<int64> Envelope(Getdatafun getdataY, Getdatafun getdataX, double width, Function <bool(double a, double b)> Fun);
	int magic = 1234321;
};

class DataXRange : public DataSource {
private:
	DataSource *data;
	double xHigh, xLow;
	int count;

public:
	DataXRange() : data(0), count(1000) {}
	DataXRange(DataSource &_data, double _xLow, double _xHigh) {Init(_data, _xLow, _xHigh);}
	void Init(DataSource &_data, double _xLow, double _xHigh) {
		data = &_data;
		isExplicit = _data.IsExplicit();
		isParam = _data.IsParam();
		xLow = _xLow;
		xHigh = _xHigh;
		count = 1000;
	}
	void SetCount(int _count)	{count = _count;}
	void SetXLow(double _xLow) 	{xLow = _xLow;}
	void SetXHigh(double _xHigh){xHigh = _xHigh;}
	bool Check(int64 id) const {
		double x = data->x(id);
		if (IsNum(xHigh) && xHigh < x)
			return false;
		if (IsNum(xLow) && xLow > x)
			return false;
		return true;
	}
	virtual inline double y(int64 id) {
		if (isExplicit)
			return f(xLow + id*(xHigh - xLow)/double(count - 1));
		else
			return Check(id) ? data->y(id) : Null;
	}
	virtual inline double x(int64 id) {
		if (isExplicit)
			return xLow + id*(xHigh - xLow)/double(count - 1);
		else
			return Check(id) ? data->x(id) : Null;
	}
	virtual inline double x(double t) {
		double x = data->x(t);
		if (IsNum(xHigh) && xHigh < x)
			return Null;
		if (IsNum(xLow) && xLow > x)
			return Null;
		return x;
	}
	virtual double f(double x) {
		if (IsNum(xHigh) && xHigh < x)
			return Null;
		if (IsNum(xLow) && xLow > x)
			return Null;
		return data->f(x);
	}
	virtual double MinX() 				{return xLow;}
	virtual double MaxX() 				{return xHigh;}
	virtual inline int64 GetCount() const {
		if (isExplicit)
			return count;
		return data->GetCount();
	}
};

class DataTranslateScale : public DataSource {
private:
	DataSource *data;
	double shiftX, shiftY;
	double multX, multY;
	int count;

public:
	DataTranslateScale() : data(0), count(1000) {}
	DataTranslateScale(DataSource &_data, double _shiftX, double _multX, double _shiftY, double _multY) {
		Init(_data, _shiftX, _multX, _shiftY, _multY);
	}
	void Init(DataSource &_data, double _shiftX, double _multX, double _shiftY, double _multY) {
		data = &_data;
		isExplicit = _data.IsExplicit();
		isParam = _data.IsParam();
		shiftX = _shiftX;
		shiftY = _shiftY;
		multX = _multX;
		multY = _multY;
		count = 1000;
	}
	void SetCount(int _count)		{count = _count;}
	void SetShiftX(double _shiftX) 	{shiftX = _shiftX;}
	void SetShiftY(double _shiftY)	{shiftY = _shiftY;}
	void SetMultX(double _multX) 	{multX = _multX;}
	void SetMultY(double _multY)	{multY = _multY;}
	/*virtual inline double y(int64 id) {
		if (isExplicit)
			return f(id)*multY + shiftY;
		else
			return data->y(id)*multY + shiftY;
	}*/
	virtual inline double x(double t) {
		return data->x(t)*multX + shiftX;
	}
	virtual double f(double x) {
		return data->f(x)*multY + shiftY;
	}
	virtual inline int64 GetCount() const {
		if (isExplicit)
			return count;
		return data->GetCount();
	}
};

class DataReverse : public DataSource {
private:
	DataSource *data;

public:
	DataReverse() : data(0) {}
	DataReverse(DataSource &_data) {Init(_data);}
	void Init(DataSource *_data)   {Init(*_data);}
	void Init(DataSource &_data) {
		ASSERT(!_data.IsExplicit() && !_data.IsParam());
		data = &_data;
	}
	virtual inline double y(int64 id) {return data->y(GetCount() - id - 1);}
	virtual inline double x(int64 id) {return data->x(GetCount() - id - 1);}
	virtual int64 GetCount() const	  {return data->GetCount();}
};

class DataReverseX : public DataSource {
private:
	DataSource *data;

public:
	DataReverseX() : data(0) {}
	DataReverseX(DataSource &_data) {Init(_data);}
	void Init(DataSource &_data) {
		ASSERT(!_data.IsExplicit() && !_data.IsParam());
		data = &_data;
	}
	virtual inline double y(int64 id) {return data->y(id);}
	virtual inline double x(int64 id) {return data->x(GetCount() - id - 1);}
	virtual int64 GetCount() const	  {return data->GetCount();}
};

class DataAppend : public DataSource {
protected:
	DataSource *data1, *data2;

public:
	DataAppend() : data1(0), data2(0) {}
	DataAppend(DataSource &_data1, DataSource &_data2) {Init(_data1, _data2);}
	void Init(DataSource &_data1, DataSource &_data2) {
		ASSERT(!_data1.IsExplicit() && !_data1.IsParam() && !_data2.IsExplicit() && !_data2.IsParam());
		data1 = &_data1;
		data2 = &_data2;
	}
	virtual inline double y(int64 id) {
		int64 count1 = data1->GetCount();
		if (id < count1)
			return data1->y(id);	
		return data2->y(id - count1);
	}
	virtual inline double x(int64 id) {
		int64 count1 = data1->GetCount();
		if (id < count1)
			return data1->x(id);	
		return data2->x(id - count1);
	}
	virtual int64 GetCount() const		{return data1->GetCount() + data2->GetCount();}
};

class DataRange : public DataAppend {
public:
	DataRange() : DataAppend() {}
	DataRange(DataSource &_data1, DataSource &_data2) {Init(_data1, _data2);}
	void Init(DataSource &_data1, DataSource &_data2) {
		ASSERT(!_data1.IsExplicit() && !_data1.IsParam() && !_data2.IsExplicit() && !_data2.IsParam());
		data1 = &_data1;
		rev.Init(_data2);
		data2 = &rev;
	}
private:
	DataReverse rev;	
};
	
class DataStackedY {
public:
	DataStackedY() : is100(false) {}
	void Set100(bool _is100)	  {is100 = _is100;}
	DataStackedY &Add(DataSource &data) {
		EachDataStackedY &each = eachData.Add();
		each.Init(data, eachData.GetCount() -1, this);
		return *this;
	}
	double Get100Y(int index, int64 id) {
		double acc = 0;
		for (int i = 0; i < eachData.GetCount(); ++i)
			acc += eachData[i].RealY(id);
		if (acc == 0)
			return 0;
		else
			return 100*eachData[index].RealY(id)/acc;
	}
	double GetY(int index, int64 id) {
		double res = 0;
		for (int i = 0; i <= index; ++i) {
			if (is100) 
				res += Get100Y(i, id);
			else
				res += eachData[i].RealY(id);
		}
		return res;
	}
	
	class EachDataStackedY : public DataSource {
	public:
		EachDataStackedY() {}
		void Init(DataSource &_data, int _index, DataStackedY *_parent) {
			ASSERT(!_data.IsExplicit() && !_data.IsParam());
			this->data = &_data;
			this->index = _index;
			this->parent = _parent;
		}
		virtual inline double y(int64 id) {
			return parent->GetY(index, id);	
		}
		double RealY(int64 id) {
			return data->y(id);	
		}
		virtual inline double x(int64 id) {
			return data->x(id);	
		}
		virtual int64 GetCount() const {
			return data->GetCount();
		}
	private:
		DataSource *data = 0;
		int index = -1;
		DataStackedY *parent = 0;
	};
	
	EachDataStackedY &Get(int id) {return eachData[id];}

protected:
	Array<EachDataStackedY> eachData;
	bool is100;
};
	
class CArray : public DataSource {
private:
	const double *yData, *xData, *zData;
	int64 numData;
	double x0, deltaX;
	
public:
	CArray(const double *_yData, int _numData, double _x0, double _deltaX) : yData(_yData), numData(_numData), x0(_x0), deltaX(_deltaX) {xData = NULL;}
	CArray(const double *_xData, const double *_yData, int _numData) : xData(_xData), yData(_yData), numData(_numData) {zData = nullptr; x0 = deltaX = 0;}
	CArray(const double *_xData, const double *_yData, const double *_zData, int _numData) : xData(_xData), yData(_yData), zData(_zData), numData(_numData) {x0 = deltaX = 0;}
	virtual inline double y(int64 id)  	{return yData[ptrdiff_t(id)];}
	virtual inline double x(int64 id)  	{return xData ? xData[ptrdiff_t(id)] : id*deltaX + x0;}
	virtual double znFixed(int n, int64 id); 
	virtual int GetznFixedCount() const			{return 1;}
	virtual inline int64 GetCount() const		{return numData;}
};

class EigenVector : public DataSource {
private:
	const Eigen::VectorXd *xData, *yData, *zData;
	double x0, deltaX;
	
public:
	EigenVector(const Eigen::VectorXd &_yData, double _x0, double _deltaX) : yData(&_yData), x0(_x0), deltaX(_deltaX) {xData = nullptr;}
	EigenVector(const Eigen::VectorXd &_xData, const Eigen::VectorXd &_yData) : xData(&_xData), yData(&_yData) {
		ASSERT(_xData.size() == _yData.size());
		zData = nullptr; 
		x0 = deltaX = 0;
	}
	EigenVector(const Eigen::VectorXd &_xData, const Eigen::VectorXd &_yData, const Eigen::VectorXd &_zData) : xData(&_xData), yData(&_yData), zData(&_zData) {
		ASSERT(_xData.size() == _yData.size() && _xData.size() == _zData.size());
		x0 = deltaX = 0;
	}
	virtual inline double x(int64 id)  	{return xData ? (*xData)(Eigen::Index(id)) : id*deltaX + x0;}
	virtual inline double y(int64 id)  	{return (*yData)(Eigen::Index(id));}
	virtual double znFixed(int n, int64 id) {
		if (n == 0)
			return (*zData)(Eigen::Index(id));
		NEVER();
		return Null;
	}
	virtual int GetznFixedCount() const			{return 1;}
	virtual inline int64 GetCount() const		{return yData->size();}
};

template <class Y>
class VectorY : public DataSource {
private:
	const Vector<Y> *yData;
	double x0, deltaX;

public:
	VectorY() : yData(0), x0(0), deltaX(0) {}
	VectorY(const Vector<Y> &_yData, double _x0 = 0, double _deltaX = 1) {Init(_yData, _x0, _deltaX);}
	void Init(const Vector<Y> &_yData, double _x0 = 0, double _deltaX = 1) {
		this->yData = &_yData;
		this->x0 = _x0;
		this->deltaX = _deltaX;
	}	
	virtual inline double y(int64 id) 		{return (*yData)[int(id)];}
	virtual inline double x(int64 id) 		{return id*deltaX + x0;}
	virtual inline int64 GetCount() const	{return yData->GetCount();}
	virtual double MinX() 					{return x0;}	
	virtual double MaxX() 					{return x0 + (yData->GetCount() - 1)*deltaX;}	
	virtual double AvgX() 					{return x0 + ((yData->GetCount() - 1)*deltaX)/2.;}
};	

template <class Y>
class ArrayY : public DataSource {
private:
	const Upp::Array<Y> *yData = 0;
	double x0 = 0, deltaX = 0;

public:
	ArrayY() {}
	ArrayY(const Upp::Array<Y> &_yData, double _x0 = 0, double _deltaX = 1) {Init(_yData, _x0, _deltaX);}
	void Init(const Upp::Array<Y> &_yData, double _x0 = 0, double _deltaX = 1) {
		this->yData = &_yData;
		this->x0 = _x0;
		this->deltaX = _deltaX;
	}	
	virtual inline double y(int64 id) 		{return (*yData)[ptrdiff_t(id)];}
	virtual inline double x(int64 id) 		{return id*deltaX + x0;}
	virtual inline int64 GetCount() const	{return yData->GetCount();}
	virtual double MinX() 					{return x0;}	
	virtual double MaxX() 					{return x0 + yData->GetCount()*deltaX;}	
	virtual double AvgX() 					{return (x0 + yData->GetCount()*deltaX)/2.;}
};

template <class Y>
class VectorVectorY : public DataSource {
private:
	const Vector<Vector<Y> > *data = 0;
	bool useRows = true;
	int idx = 0, idy = 1;
	Vector<int> idsx, idsy, idsFixed;
	int beginData = 0;
	int numData = Null;
	
public:
	VectorVectorY() {}
	VectorVectorY(const Vector<Vector<Y> > &_data, int _idx, int _idy, 
				  const Vector<int> &_idsx, const Vector<int> &_idsy, const Vector<int> &_idsFixed, 
				  bool _useRows = true, int _beginData = 0, int _numData = Null) {
		Init(_data, _idx, _idy, _idsx, _idsy, _idsFixed, _useRows, _beginData, _numData);
	}
	void Init(const Vector<Vector<Y> > &_data, int _idx, int _idy, const Vector<int> &_idsx, const Vector<int> &_idsy, 
			  const Vector<int> &_idsFixed, bool _useRows = true, int _beginData = 0, int _numData = Null) {
		this->data = &_data;
		this->useRows = _useRows;
		
		this->idx = _idx;
		this->idy = _idy;
		this->idsx = clone(_idsx);
		this->idsy = clone(_idsy);
		this->idsFixed = clone(_idsFixed);
		this->beginData = _beginData;
		this->numData = _numData;
		if (IsNull(_numData)) {
			if (!_useRows) {
				if (_data.IsEmpty())
					this->numData = 0;
				else	
					this->numData = _data[0].GetCount() - _beginData;
			} else
				this->numData = _data.GetCount() - _beginData;
		}
	}
	void Init(const Vector<Vector<Y> > &_data, int _idx, int _idy, bool _useRows = true, int _beginData = 0, int _numData = Null) {
		static Vector<int> idsVoid;
		Init(_data, _idx, _idy, idsVoid, idsVoid, idsVoid, _useRows, _beginData, _numData);
	}
	virtual inline double y(int64 id)  {
		if (!IsNull(idy) && idy >= 0) {
			if (useRows) {
				if ((*data)[beginData + int(id)].GetCount() <= idy)
					return Null;
				return (*data)[beginData + int(id)][idy];
			} else {
				if ((*data)[idy].GetCount() <= beginData + int(id))
					return Null;
				return (*data)[idy][beginData + int(id)];
			}
		} else {
			if (GetznyCount(id) == 0)
				return Null;
			double ret = 0;
			for (int i = 0; i < GetznyCount(id); ++i) 
				ret += zny(i, id);
			return ret/GetznyCount(id);
		}
	}
	virtual inline double x(int64 id)  {return useRows ? (*data)[beginData + int(id)][idx] : (*data)[idx][beginData + int(id)];}
	virtual inline int64 GetCount() const	{return numData;};
	virtual double znx(int n, int64 id)	const {
		return useRows ? (*data)[beginData + int(id)][idsx[n]] : (*data)[idsx[n]][beginData + int(id)];}
	virtual double zny(int n, int64 id)	const {
		if (!IsNull(idy) && idy < 0) 
			return useRows ? (*data)[beginData + int(id)][n - idy] : (*data)[n - idy][beginData + int(id)];	
		return useRows ? (*data)[beginData + int(id)][idsy[n]] : (*data)[idsy[n]][beginData + int(id)];
	}
	virtual double znFixed(int n, int64 id)	{return useRows ? (*data)[beginData + int(id)][idsFixed[n]] : (*data)[idsFixed[n]][beginData + int(id)];}
	virtual int GetznxCount(int64) const	{
		return idsx.GetCount();}
	virtual int GetznyCount(int64 id) const {
		if (!IsNull(idy) && idy < 0) 
			return (useRows ? (*data)[beginData + int(id)].GetCount() : (*data).GetCount()) + idy;
		return idsy.GetCount();
	}
	virtual int GetznFixedCount() const		{return idsFixed.GetCount();}
};

class VectorXY : public DataSource {
private:
	const Vector<double> *xData, *yData;

public:
	VectorXY(const Vector<double> &_xData, const Vector<double> &_yData) : xData(&_xData), yData(&_yData) {}
	virtual inline double x(int64 id) 		{return (*xData)[int(id)];}
	virtual inline double y(int64 id) 		{return (*yData)[int(id)];}
	virtual inline int64 GetCount()	 const	{return min(xData->GetCount(), yData->GetCount());}
};

class ArrayXY : public DataSource {
private:
	const Upp::Array<double> *xData, *yData;

public:
	ArrayXY(const Upp::Array<double> &_xData, const Upp::Array<double> &_yData) : xData(&_xData), yData(&_yData) {}
	virtual inline double x(int64 id) 		{return (*xData)[int(id)];}
	virtual inline double y(int64 id) 		{return (*yData)[int(id)];}
	virtual inline int64 GetCount() const	{return min(xData->GetCount(), yData->GetCount());}
};

class VectorPointf : public DataSource {
private:
	const Vector<Pointf> *data;

public:
	VectorPointf() : data(0) {}
	VectorPointf(const Vector<Pointf> &_data)	{Init(&_data);}
	VectorPointf(Vector<Pointf> *_data) 		{Init(_data);}
	void Init(const Vector<Pointf> *_data) 		{data = _data;}
	void Init(const Vector<Pointf> &_data) 		{data = &_data;}
	virtual inline double y(int64 id) 			{return (*data)[int(id)].y;}
	virtual inline double x(int64 id) 	 		{return (*data)[int(id)].x;}
	virtual inline int64 GetCount() const		{return data->GetCount();}
};	

class ArrayPointf : public DataSource {
private:
	const Upp::Array<Pointf> *data;

public:
	ArrayPointf(const Upp::Array<Pointf> &_data) : data(&_data) {}
	virtual inline double y(int64 id) 		{return (*data)[int(id)].y;}
	virtual inline double x(int64 id) 	 {return (*data)[int(id)].x;}
	virtual inline int64 GetCount() const	{return data->GetCount();}
};	

template <class X, class Y>
class VectorMapXY : public DataSource {
private:
	const VectorMap<X, Y> *data;

public:
	VectorMapXY(const VectorMap<X, Y> &_data) : data(&_data) {}
	virtual inline double y(int64 id) 			{return (*data)[int(id)];}
	virtual inline double x(int64 id) 	 	{return (*data).GetKey(int(id));}
	virtual inline int64 GetCount() const		{return data->GetCount();}
};	

template <class X, class Y>
class ArrayMapXY : public DataSource {
private:
	const ArrayMap<X, Y> *data;

public:
	ArrayMapXY(const ArrayMap<X, Y> &_data) : data(&_data) {}
	virtual inline double y(int64 id) 			{return (*data)[int(id)];}
	virtual inline double x(int64 id) 		 	{return (*data).GetKey(int(id));}
	virtual inline int64 GetCount() const		{return data->GetCount();}
};		

class FuncSource : public DataSource {
protected:
	Function <double(double)> function;

public:
	FuncSource() {isExplicit = true;}
	FuncSource(Function <double(double)> _function) : function(_function) {isExplicit = true;}
	virtual inline double f(double x)		{return function(x);}
	virtual double x(int64 ) 				{NEVER(); return Null;}
	virtual double y(int64 ) 				{NEVER(); return Null;}
	virtual inline int64 GetCount() const	{return 0;}
};

class FuncSourceV : public DataSource {
private:
	Event<double&, double> function;

public:
	FuncSourceV(Event<double&, double> _function) : function(_function) {isExplicit = true;}
	virtual inline double f(double x)		{double y; function(y, x); return y;}
	virtual double x(int64 ) 				{NEVER(); return Null;}
	virtual double y(int64 ) 				{NEVER(); return Null;}
	virtual inline int64 GetCount() const	{return 0;}
};

class FuncSourcePara : public DataSource {
private:
	Function <Pointf(double)> function;
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;
	
public:
	FuncSourcePara(Function <Pointf(double)> _function, int np, double from, double to) : 
							function(_function), numPoints(np), minT(from), maxT(to) {
		isParam = true; 
		lastT = Null;
	}
	virtual inline double y(double t) {
		if (!IsNum(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	virtual inline double x(double t) {
		if (!IsNum(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual double x(int64 ) 				{NEVER(); return Null;}
	virtual double y(int64 ) 				{NEVER(); return Null;}
	virtual inline int64 GetCount() const	{return numPoints;}
};	

typedef Event<double&, double> PlotExplicFunc; 
typedef Function<void(Pointf&, double)> PlotParamFunc;


class PlotExplicFuncSource : public DataSource {
private:
	PlotExplicFunc function;

public:
	PlotExplicFuncSource(PlotExplicFunc &_function) : function(_function) {isExplicit = true;}
	virtual inline double f(double t)	{double y; function(y, t); return y;}
	virtual double x(int64 ) 				{NEVER(); return Null;}
	virtual double y(int64 ) 				{NEVER(); return Null;}
	virtual inline int64 GetCount() const	{NEVER(); return Null;}
};	

class PlotParamFuncSource : public DataSource {
private:
	PlotParamFunc function;
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;

public:
	PlotParamFuncSource(PlotParamFunc _function, int np, double from, double to) : 
						function(_function), numPoints(np), minT(from), maxT(to) {
		isParam = true; 
		lastT = Null;
	}
	inline double y(double t) {
		if (!IsNum(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	inline double x(double t) {
		if (!IsNum(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual double x(int64 ) 				{NEVER(); return Null;}
	virtual double y(int64 ) 				{NEVER(); return Null;}
	virtual inline int64 GetCount() const	{return numPoints;}
};	
	
struct PointfLess {
	bool operator () (const Pointf& a, const Pointf& b) const { return a.x < b.x; }
};

class DataSourceSurf {
public:
	DataSourceSurf() : isExplicit(false), key(1212121) {}
	virtual ~DataSourceSurf() noexcept	{key = 1231231;}	
	virtual double z(double x, double y)= 0;
	
	virtual bool IsEmpty() = 0;
	bool IsDeleted()	   {return key != 1212121;}
	bool IsExplicit()	   {return isExplicit;}
 	
	virtual double MinX() = 0;
	virtual double MaxX() = 0;
	virtual double MinY() = 0;
	virtual double MaxY() = 0;
	virtual double MinZ() = 0;
	virtual double MaxZ() = 0;

	Vector<Pointf> GetIsoline(double thres, const Rectf &area, double deltaX, double deltaY);
	Vector<Pointf> GetIsolines(const Vector<double> &vals, const Rectf &area, double deltaX, double deltaY);

protected:
	bool isExplicit;
	
private:
	int key;
};

template <class T>
inline T LinearInterpolate(T x, T x0, T x1, T y0, T y1) {
	T x1_0 = x1 - x0;
	if (abs(x1_0) < T(FLT_EPSILON))
		return (y0 + y1)/T(2);
	
  	return y0 + (x - x0)*(y1 - y0)/x1_0;
}

template <class T>
inline T QuadraticInterpolate(T x, T x0, T x1, T x2, T y0, T y1, T y2) {
	T x0_1 = x0 - x1;
	T x0_2 = x0 - x2;
	T x1_2 = x1 - x2;
	if (abs(x0_1) < T(FLT_EPSILON))
		return LinearInterpolate(x, x0, x2, (y0 + y1)/T(2), y2);
	else if (abs(x0_2) < T(FLT_EPSILON))
		return LinearInterpolate(x, x0, x1, (y0 + y2)/T(2), y1);
	else if (abs(x1_2) < T(FLT_EPSILON))
		return LinearInterpolate(x, x0, x1, y0, (y1 + y2)/T(2));
	
	T x_0 = x - x0;
	T x_1 = x - x1;
	T x_2 = x - x2;
	
  	return ((x_1*x_2)/(x0_1*x0_2))*y0 - ((x_0*x_2)/(x0_1*x1_2))*y1 + ((x_0*x_1)/(x0_2*x1_2))*y2;
}

template <class T>
inline T BilinearInterpolate(T x, T y, T x0, T x1, T y0, T y1, T v00, T v01, T v10, T v11) {
	T r0 = LinearInterpolate(x, x0, x1, v00, v10);
	T r1 = LinearInterpolate(x, x0, x1, v01, v11);
	
	return LinearInterpolate(y, y0, y1, r0, r1);
}

template <class T>
inline T TrilinearInterpolate(T x, T y, T z, T x0, T x1, T y0, T y1, T z0, T z1, T v000, T v001, T v010, T v011, T v100, T v101, T v110, T v111) {
	T x00 = LinearInterpolate(x, x0, x1, v000, v100);
	T x10 = LinearInterpolate(x, x0, x1, v010, v110);
	T x01 = LinearInterpolate(x, x0, x1, v001, v101);
	T x11 = LinearInterpolate(x, x0, x1, v011, v111);
	T r0  = LinearInterpolate(y, y0, y1, x00,  x01);
	T r1  = LinearInterpolate(y, y0, y1, x10,  x11);
	
	return LinearInterpolate(z, z0, z1, r0, r1);
}

template <class T>
T LinearInterpolate(const T x, const Point_<T> *vec, int len) {
	ASSERT(len > 1);
	if (x < vec[0].x)
		return vec[0].y;
	for (int i = 0; i < len-1; ++i) {
		if (vec[i+1].x >= x && vec[i].x <= x) 
			return LinearInterpolate(x, vec[i].x, vec[i+1].x, vec[i].y, vec[i+1].y);
	}
	return vec[len-1].y;
}

template <class Range>
typename Range::value_type LinearInterpolate(const typename Range::value_type x, const Range &vec) {
	ASSERT(vec.GetCount() > 1);
	return LinearInterpolate(x, (const typename Range::value_type *)vec, vec.GetCount());
}

template <class T>
void GetInterpolatePos(const T x, const T *vecx, int len, int &x0, int &x1) {
	if (x < vecx[0]) {
		x0 = x1 = 0;
		return;
	}
	for (int i = 0; i < len-1; ++i) {
		if (vecx[i+1] >= x && vecx[i] <= x) {
			x0 = i;
			x1 = i+1;
			return;
		}
	}
	x0 = x1 = len-1;
}

template <class Range>
void GetInterpolatePos(const typename Range::value_type x, const Range &vecx, int &x0, int &x1) {
	ASSERT(vecx.size() > 1);
	GetInterpolatePos(x, (const typename Range::value_type *)vecx, vecx.size(), x0, x1);
}

template <class T>
T LinearInterpolate(const T x, const T *vecx, const T *vecy, size_t len) {
	ASSERT(len > 1);
	if (x < vecx[0])
		return vecy[0];
	for (int i = 0; i < len-1; ++i) {
		if (vecx[i+1] >= x && vecx[i] <= x) 
			return LinearInterpolate(x, vecx[i], vecx[i+1], vecy[i], vecy[i+1]);
	}
	return vecy[len-1];
}

template <class Range>
typename Range::value_type LinearInterpolate(typename Range::value_type x, const Range &vecx, const Range &vecy) {
	ASSERT(vecx.size() > 1);
	ASSERT(vecx.size() == vecy.size());
	return LinearInterpolate(x, (const typename Range::value_type *)vecx, 
								(const typename Range::value_type *)vecy, vecx.size());
}

double LinearInterpolate(double x, const Eigen::VectorXd &vecx, const Eigen::VectorXd &vecy);

class TableInterpolate {
public:
	enum Interpolate {NO, BILINEAR};
	
protected:
	Interpolate inter;
};

								 		       
class TableData : public DataSourceSurf, public TableInterpolate {
public:
	typedef double (TableData::*Getdatafun)(int d);
	
	TableData() : lendata(0), lenxAxis(0), lenyAxis(0), areas(false) {};

	Interpolate Inter()				{return inter;}
	void Inter(Interpolate _inter)	{this->inter = _inter;}
	
	double z_area(Getdatafun getdataX, Getdatafun getdataY, Getdatafun getdata, 
					double x, double y);
	double z_point(Getdatafun getdataX, Getdatafun getdataY, Getdatafun getdata, 
					double x, double y);

	virtual inline double x(int )		{NEVER();	return Null;}
	virtual inline double y(int )		{NEVER();	return Null;}
	virtual inline double data(int )	{NEVER();	return Null;}
	
	double z(double x, double y) {
		return z(&TableData::x, &TableData::y, &TableData::data, x, y);
	}
	double z(Getdatafun getdataX, Getdatafun getdataY, Getdatafun getdata, double x, double y) {
		if (areas)
			return z_area(getdataX, getdataY, getdata, x, y);
		else
			return z_point(getdataX, getdataY, getdata, x, y);
	}
	
	bool IsEmpty() {
		return lendata == 0 || lenxAxis == 0 || lenyAxis == 0;
	}
	
	double MinX(Getdatafun getdata);
	virtual double MinX() 				{return MinX(&TableData::x);}
	double MaxX(Getdatafun getdata);
	virtual double MaxX() 				{return MaxX(&TableData::x);}
	double MinY(Getdatafun getdata);
	virtual double MinY() 				{return MinY(&TableData::y);}
	double MaxY(Getdatafun getdata);
	virtual double MaxY() 				{return MaxY(&TableData::y);}
	double MinZ(Getdatafun getdata);
	virtual double MinZ() 				{return MinZ(&TableData::data);}
	double MaxZ(Getdatafun getdata);
	virtual double MaxZ() 				{return MaxZ(&TableData::data);}

	int lendata;
	int lenxAxis;
	int lenyAxis;
	
protected:
	bool areas;
};

class TableDataVector : public TableData {
public:
	TableDataVector() : pdata(0), pxAxis(0), pyAxis(0) {}
	TableDataVector(Vector<double> &data, Vector<double> &xAxis, Vector<double> &yAxis, 
			Interpolate _inter, bool _areas) {Init(data, xAxis, yAxis, _inter, _areas);}
	void Init(Vector<double> &data, Vector<double> &xAxis, Vector<double> &yAxis, 
					Interpolate _inter, bool _areas) {
		ASSERT(_areas ?  (data.GetCount() == (xAxis.GetCount() - 1)*(yAxis.GetCount() - 1)) : true);
		ASSERT(!_areas ? (data.GetCount() == xAxis.GetCount()*yAxis.GetCount()) : true);
		this->pdata = &data;
		this->lendata = data.GetCount();
		this->pxAxis = &xAxis;
		this->lenxAxis = xAxis.GetCount();
		this->pyAxis = &yAxis;
		this->lenyAxis = yAxis.GetCount();
		this->inter = _inter;
		this->areas = _areas;
	}
	virtual inline double x(int id) 	{return (*pxAxis)[id];}
	virtual inline double y(int id) 	{return (*pyAxis)[id];}
	virtual inline double data(int id) 	{return (*pdata)[id];}
	
private:
	Vector<double> *pdata;
	Vector<double> *pxAxis;
	Vector<double> *pyAxis;
};

class TableDataCArray : public TableData {
public:
	TableDataCArray() : pdata(0), pxAxis(0), pyAxis(0) {}
	TableDataCArray(double *data, int _lendata, double *xAxis, int _lenxAxis, double *yAxis, int _lenyAxis, 
					Interpolate _inter, bool _areas) {Init(data, _lendata, xAxis, _lenxAxis, yAxis, _lenyAxis, _inter, _areas);}
	void Init(double *data, int _lendata, double *xAxis, int _lenxAxis, double *yAxis, int _lenyAxis, 
					Interpolate _inter, bool _areas) {
		ASSERT(_areas ?  (_lendata == (_lenxAxis - 1)*(_lenyAxis - 1)) : true);
		ASSERT(!_areas ? (_lendata == _lenxAxis*_lenyAxis) : true);
		this->pdata = data;
		this->lendata = _lendata;
		this->pxAxis = xAxis;
		this->lenxAxis = _lenxAxis;
		this->pyAxis = yAxis;
		this->lenyAxis = _lenyAxis;
		this->inter = _inter;
		this->areas = _areas;
	}
	virtual inline double x(int id) 	{return pxAxis[id];}
	virtual inline double y(int id) 	{return pyAxis[id];}
	virtual inline double data(int id) 	{return pdata[id];}
	
private:
	double *pdata;
	double *pxAxis;
	double *pyAxis;
};


class ExplicitData : public DataSourceSurf {
public:
	ExplicitData() {}
	ExplicitData(Function<double (double x, double y)> _funz, double _minX, double _maxX, double _minY, double _maxY) {
		Init(_funz, _minX, _maxX, _minY, _maxY);
	}
	void Init(Function<double (double x, double y)> funz, double minX, double maxX, double minY, double maxY);
		
	double z(double x, double y)	{return funz ? funz(x, y) : Null;}
	
	bool IsEmpty() 	{return funz;}
	
	double MinX()	{return minX;}
	double MaxX()	{return maxX;}
	double MinY()	{return minY;}
	double MaxY()	{return maxY;}
	double MinZ()	{return minZ;}
	double MaxZ()	{return maxZ;}
		
private:
	double minX, maxX, minY, maxY, minZ, maxZ;
	Function<double (double x, double y)> funz;
};

Vector<Pointf> Intersection(Vector<Pointf> &poly1, Vector<Pointf> &poly2);
void Simplify(Vector<Pointf> &poly, double dx, double dy);

bool SavitzkyGolay_CheckParams(int nleft, int nright, int deg, int der);
Eigen::VectorXd SavitzkyGolay_Coeff(int nleft, int nright, int deg, int der);

template<class T>
typename T::PlainObject Convolution(const Eigen::MatrixBase<T>& orig, const Eigen::MatrixBase<T>& kernel, const double factor = 1) {
	const Eigen::Index ksize = kernel.size();
	
	ASSERT_(ksize % 2 != 0, "Only support odd sized kernels");
	ASSERT(orig.size() > ksize);
	
	typename T::PlainObject dest(orig.size() - 2 * (ksize/2));
	
	for (typename T::Index row = 0; row < dest.size(); ++row)
	  	dest(row) = orig.segment(row, ksize).cwiseProduct(kernel).sum();
	if (factor != 1)
		dest *= factor;
	return dest;
}

template<class T>
typename T::PlainObject Convolution2D(const Eigen::MatrixBase<T>& orig, const Eigen::MatrixBase<T>& kernel, const double factor = 1) {
	const Eigen::Index krows = kernel.rows();
	const Eigen::Index kcols = kernel.cols();
	
	ASSERT_(krows % 2 != 0, "Only support odd sized kernels (even rows)");
	ASSERT_(kcols % 2 != 0, "Only support odd sized kernels (even cols)");
	ASSERT(orig.rows() > krows);
	ASSERT(orig.cols() > kcols);
	
	typename T::PlainObject dest(orig.rows() - 2*(krows/2), orig.cols() - 2*(kcols/2));
	
	for (typename T::Index row = 0; row < dest.rows(); ++row)
		for (typename T::Index col = 0; col < dest.cols(); ++col) 
	  		dest(row, col) = orig.block(row, col, krows, kcols).cwiseProduct(kernel).sum();
	if (factor != 1)
		dest *= factor;
	return dest;
}

template <class Range>
typename Range::value_type HammingWindow(Range &data) {
	size_t numData = data.size();
	typename Range::value_type numDataFact = 0;
	for (size_t i = 0; i < numData; ++i) {
        typename Range::value_type windowFact = 0.54 - 0.46*cos(2*M_PI*i/numData);
        numDataFact += windowFact;
    	data[i] *= windowFact;
	}
	return numDataFact;
}


template <class Range1, class Range2>
void MovingAverage(const Range1 &x, const Range1 &y, typename Range1::value_type width, Range2 &rresy) {
	using Scalar = typename Range1::value_type;
	ASSERT(x.size() == y.size());
	Range2 resy(x.size());
	
	if (x.size() == 0)
		return;
	
	Scalar width_2 = width/2.;
	
	for (int i = 0; i < x.size(); ++i) {
		int ifrom, ito;
		Scalar xi = x[i];
		for (ifrom = i-1; ifrom >= 0; --ifrom) 
			if ((xi - x[ifrom]) > width_2)
				break;
		ifrom++;
		for (ito = i+1; ito < x.size(); ++ito) 
			if ((x[ito] - xi) > width_2) 
				break;
		ito--;
		resy[i] = std::accumulate(&y[ifrom], &y[ito], 0.)/(ito - ifrom + 1);
	}
	rresy = pick(resy);
}

template <class Range>
void FindPeaks(const Range &y, Vector<int64> &imx, Vector<int64> &imn) {
	using Scalar = typename Range::value_type;
	
	imx.Clear();
	imn.Clear();

	if (y.size() < 3)
		return;
	
	bool nextIsMax = y[1] > y[0];
	int64 beginpeak = -1;
	
	Scalar thisy = y[1];
	for (int i = 1; i < y.size()-1; ++i) {
		Scalar nexty = y[i+1];
		if (nexty == thisy) {
			if (beginpeak < 0)
				beginpeak = i;
		} else {
			if (nextIsMax) {
				if (nexty < thisy) {
					imx << (beginpeak < 0 ? i : (beginpeak + i)/2);
					nextIsMax = false;
				}
			} else {
				if (nexty > thisy) {
					imn << (beginpeak < 0 ? i : (beginpeak + i)/2);
					nextIsMax = true;
				}
			}
			beginpeak = -1;
		}
		thisy = nexty;
	}
}

template <class Range>
void FindPeaks(const Range &x, const Range &y, typename Range::value_type width, Vector<int64> &rimx, Vector<int64> &rimn) {
	using Scalar = typename Range::value_type;
	
	rimx.Clear();
	rimn.Clear();
	Vector<int64> imx, imn;
	
	FindPeaks(y, imx, imn);
	
	Scalar x0 = x[0];
	int64 idm = -1;
	for (int i = 0; i < imx.size(); ++i) {
		int64 id = imx[i];
		if (x[id] - x0 > width) {
			if (idm >= 0)  
				rimx << idm;
			x0 += width; 
			idm = -1;
		} 
		if (idm < 0 || y[id] > y[idm])
			idm = id;
	}
	x0 = x[0];
	for (int i = 0; i < imn.size(); ++i) {
		int64 id = imn[i];
		if (x[id] - x0 > width) {
			if (idm >= 0)  
				rimn << idm;
			x0 += width; 
			idm = -1;
		} 
		if (idm < 0 || y[id] < y[idm])
			idm = id;
	}
}	

template <class Range>
Vector<int64> UpperPeaks(const Range &x, const Range &y, typename Range::value_type width) {
	Vector<int64> imx, imn;
	FindPeaks(x, y, width, imx, imn);
	return imx;
}

template <class Range>
Vector<int64> LowerPeaks(const Range &x, const Range &y, typename Range::value_type width) {
	Vector<int64> imx, imn;
	FindPeaks(x, y, width, imx, imn);
	return imn;
}

template <class Range>
typename Range::value_type CosWindow(Range &data, int numOver) {
	size_t numData = data.size();
	typename Range::value_type numDataFact = 0;
	for (size_t i = 0; i < numOver; ++i) {
        typename Range::value_type windowFact = 0.5*(1 - cos(M_PI*i/numOver));
        numDataFact += windowFact;
    	data[i] *= windowFact;	
    }
    for (size_t i = numOver; i < numData - numOver; ++i) {
        typename Range::value_type windowFact = 1;		// 1.004
        numDataFact += windowFact;
    }
    for (size_t i = numData - numOver; i < numData; ++i) {
  		typename Range::value_type windowFact = 0.5*(1 + cos(M_PI*(numData - i - numOver)/numOver));
        numDataFact += windowFact;
    	data[i] *= windowFact;	
    }
	return numDataFact;			    
}

template <class Range1, class Range2>
void CleanNAN(const Range1 &x, Range2 &retx) {
	int num = x.size();
	Resize(retx, num);
	int n = 0;
	for (int i = 0; i < num; ++i) {		
		if (IsNum(x[i])) 
			retx[n++] = x[i];
	}
	ResizeConservative(retx, n);
}

template <class Range1, class Range2>
void CleanNAN(const Range1 &x, const Range1 &y, Range2 &retx, Range2 &rety) {
	int num = int(x.size());
	ASSERT(num == y.size());
	Resize(retx, num);
	Resize(rety, num);
	int n = 0;
	for (int i = 0; i < num; ++i) {		
		if (IsNum(x[i]) && IsNum(y[i])) {
			retx[n]   = x[i];
			rety[n] = y[i];
			n++;
		}
	}
	ResizeConservative(retx, n);
	ResizeConservative(rety, n);
}

template <class Range1, class Range2>
void CleanNAN(const Range1 &x, const Range1 &y, const Range1 &z, Range2 &retx, Range2 &rety, Range2 &retz) {
	int num = int(x.size());
	ASSERT(num == y.size() && num == z.size());
	Resize(retx, num);
	Resize(rety, num);
	Resize(retz, num);
	int n = 0;
	for (int i = 0; i < num; ++i) {		
		if (IsNum(x[i]) && IsNum(y[i]) && IsNum(z[i])) {
			retx[n] = x[i];
			rety[n] = y[i];
			retz[n] = z[i];
			n++;
		}
	}
	ResizeConservative(retx, n);
	ResizeConservative(rety, n);
	ResizeConservative(retz, n);
}

template <class Range1, class Range2>
Range1 ApplyIndex(const Range1 &x, Range2 &indices) {
	ASSERT(x.size() == indices.size());
	Range1 ret(x.size());
	for (int i = 0; i < x.size(); ++i)
		ret[i] = x[indices[i]];
	return ret;
}

template <class Range1, class Range2>
void CleanNANDupXSort(const Range1 &x, const Range1 &y, Range2 &rrx, Range2 &rry) {
	Range2 rx, ry;
	CleanNAN(x, y, rx, ry);
	
	int n = int(rx.size());
	Vector<int> indices(n);
	for (int i = 0; i < indices.size(); ++i)
		indices[i] = i;
	StableSort(indices,[&](int a, int b)-> bool {return x[a] < x[b];}); 
	rx = ApplyIndex(rx, indices);
	ry = ApplyIndex(ry, indices);
	
	Vector<int> num(n, 1);
	double epsx = (rx[n-1] - rx[0])/1000./n;
	for (int i = n-1; i >= 1; --i) {
		if (rx[i] - rx[i-1] < epsx) {
			num[i-1] += num[i];
			ry[i-1]  += ry[i];		
			num[i] = 0;
		}
	}
	int id = 0;
	for (int i = 0; i < num.size(); ++i) {
		if (num[i] != 0) { 
			ry[id] = ry[i]/num[i];
			rx[id] = rx[i];
			id++;
		}
	}
	ResizeConservative(rx, id);
	ResizeConservative(ry, id);
	
	rrx = pick(rx);
	rry = pick(ry);
}

template <class Range1, class Range2>
void CleanNANDupXSort(const Range1 &x, const Range1 &y, const Range1 &z, Range2 &rrx, Range2 &rry, Range2 &rrz) {
	Range2 rx, ry, rz;
	CleanNAN(x, y, z, rx, ry, rz);
	
	int n = int(rx.size());
	Vector<int> indices(n);
	for (int i = 0; i < indices.size(); ++i)
		indices[i] = i;
	StableSort(indices,[&](int a, int b)-> bool {return rx[a] < rx[b];}); 
	rx = ApplyIndex(rx, indices);
	ry = ApplyIndex(ry, indices);
	rz = ApplyIndex(rz, indices);
	
	Vector<int> num(n, 1);
	double epsx = (rx[n-1] - rx[0])/1000./n;
	for (int i = n-1; i >= 1; --i) {
		if (rx[i] - rx[i-1] < epsx) {
			num[i-1] += num[i];
			num[i] = 0;
			ry[i-1]  += ry[i];	
			rz[i-1]  += rz[i];	
		}
	}
	int id = 0;
	for (int i = 0; i < num.size(); ++i) {
		if (num[i] != 0) { 
			rx[id] = rx[i];
			ry[id] = ry[i]/num[i];
			rz[id] = rz[i]/num[i];			
			id++;
		}
	}
	ResizeConservative(rx, id);
	ResizeConservative(ry, id);
	ResizeConservative(rz, id);
	
	rrx = pick(rx);
	rry = pick(ry);
	rrz = pick(rz);
}
	
void Resample(const Eigen::VectorXd &sx, const Eigen::VectorXd &sy, 
			  Eigen::VectorXd &rx, Eigen::VectorXd &ry, double srate = Null);
void Resample(const Eigen::VectorXd &sx, const Eigen::VectorXd &sy, const Eigen::VectorXd &sz, 
			  Eigen::VectorXd &rx, Eigen::VectorXd &ry, Eigen::VectorXd &rz, double srate = Null);
Vector<Pointf> FFTSimple(Eigen::VectorXd &data, double tSample, bool frequency, int type, 
						 int window, int numOver);
void FilterFFT(Eigen::VectorXd &data, double T, double fromT, double toT);

}

#endif
