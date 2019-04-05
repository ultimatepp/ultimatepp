#ifndef _ScatterDraw_DataSource_h_
#define _ScatterDraw_DataSource_h_

namespace Upp {


class DataSource {
public:
	typedef double (DataSource::*Getdatafun)(int64 id);

	DataSource() : isParam(false), isExplicit(false), key(111111) {}
	virtual ~DataSource() 					{key = 0;}	
	virtual double y(int64 id)				{/*NEVER();*/	return Null;}
	virtual double x(int64 id)				{/*NEVER();*/	return Null;}
	virtual double znx(int n, int64 id)		{/*NEVER();*/	return Null;}
	virtual double zny(int n, int64 id)		{/*NEVER();*/	return Null;}
	virtual double znFixed(int n, int64 id)	{/*NEVER();*/	return Null;}
	virtual double y(double t)				{/*NEVER();*/	return Null;}
	virtual double x(double t)				{/*NEVER();*/	return Null;}
	virtual double f(double x)				{/*NEVER();*/	return Null;}
	virtual double f(Vector<double> zn)		{/*NEVER();*/	return Null;}
	virtual int64 GetCount()				{/*NEVER();*/	return Null;}
	bool IsEmpty()						{return GetCount() == 0;}
	virtual int GetznxCount(int64 id)	{return 0;}
	virtual int GetznyCount(int64 id)	{return 0;}
	virtual int GetznFixedCount()		{return 0;}
	bool IsParam()						{return isParam;}
	bool IsExplicit()					{return isExplicit;}
	bool IsDeleted()					{return key != 111111;}

	virtual double MinY(int64& id) 		{return Min(&DataSource::y, id);}
	virtual double MinY() 				{int64 dummy;	return Min(&DataSource::y, dummy);}
	virtual double MinX(int64& id) 		{return Min(&DataSource::x, id);}	
	virtual double MinX() 				{int64 dummy;	return Min(&DataSource::x, dummy);}

	virtual double MaxY(int64& id) 		{return Max(&DataSource::y, id);}
	virtual double MaxY() 				{int64 dummy;	return Max(&DataSource::y, dummy);}
	virtual double MaxX(int64& id) 		{return Max(&DataSource::x, id);}	
	virtual double MaxX() 				{int64 dummy;	return Max(&DataSource::x, dummy);}	
	
	virtual double IsSortedY() 			{return IsSorted(&DataSource::y);}		
	virtual double IsSortedX() 			{return IsSorted(&DataSource::x);}	
	
	virtual double AvgY() 				{return Avg(&DataSource::y);}		
	virtual double AvgX() 				{return Avg(&DataSource::x);}	
	virtual double RMSY() 				{return RMS(&DataSource::y);}			
	virtual double StdDevY(double avg = Null) 	{return StdDev(&DataSource::y, avg);}	
	virtual double VarianceY(double avg = Null) {return Variance(&DataSource::y, avg);}	
	virtual Vector<int64> UpperEnvelopeY(double width) 	{return UpperEnvelope(&DataSource::y, &DataSource::x, width);}	
	virtual Vector<int64> LowerEnvelopeY(double width) 	{return LowerEnvelope(&DataSource::y, &DataSource::x, width);}	
	virtual Vector<Pointf> CumulativeY() 				{return Cumulative(&DataSource::y, &DataSource::x);}
	virtual Vector<Pointf> CumulativeAverageY() 		{return CumulativeAverage(&DataSource::y, &DataSource::x);}
	virtual Vector<Pointf> MovingAverageY(double width) {return MovingAverage(&DataSource::y, &DataSource::x, width);}
	virtual Vector<Pointf> SectorAverageY(double width) {return SectorAverage(&DataSource::y, &DataSource::x, width);}	
	virtual void MaxListY(Vector<int64> &id, double width){MaxList(&DataSource::y, &DataSource::x, id, width);}
	virtual Pointf MaxSubDataImpY(int64 maxId, int width)	{return MaxSubDataImp(&DataSource::y, &DataSource::x, maxId, width);}
	virtual void ZeroCrossingY(bool ascending, bool descending, Vector<double> &zeros, Vector<int64> &ids) {
		return ZeroCrossing(&DataSource::y, &DataSource::x, ascending, descending, zeros, ids);}
	virtual double IntegralY()			{return Integral(&DataSource::y, &DataSource::x);}
	virtual double IntegralY(double from, double to, double n)	{return Integral(from, to, n);}

	enum FFT_WINDOW {NO_WINDOW = 0, HAMMING, COS};
	enum FFT_TYPE   {T_FFT = 0, T_PHASE, T_PSD};

	Upp::Vector<Pointf> FFTY(double tSample, bool frequency = false, int type = FFT_TYPE::T_FFT, 
					int window = FFT_WINDOW::HAMMING, int numSub = 1, double overlapping = 0) {
		return FFT(&DataSource::y, tSample, frequency, type, window, numSub, overlapping);}
	static int GetFFTWindowCount()	{return 3;}
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
	
	double Min(Getdatafun getdata, int64& id);
	double Max(Getdatafun getdata, int64& id);
	double Avg(Getdatafun getdata);
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
		
protected:
	bool isParam, isExplicit;
	
private:
	int key;
	
	Vector<int64> Envelope(Getdatafun getdataY, Getdatafun getdataX, double width, bool (*fun)(double a, double b));
};

class DataXRange : public DataSource {
private:
	DataSource *data;
	double xHigh, xLow;
	int count;

public:
	DataXRange() : data(0), count(1000) {}
	DataXRange(DataSource &data, double xLow, double xHigh) {Init(data, xLow, xHigh);}
	void Init(DataSource &data, double xLow, double xHigh) {
		this->data = &data;
		isExplicit = data.IsExplicit();
		isParam = data.IsParam();
		this->xLow = xLow;
		this->xHigh = xHigh;
		count = 1000;
	}
	void SetCount(int count)	{this->count = count;}
	void SetXLow(double xLow) 	{this->xLow = xLow;}
	void SetXHigh(double xHigh) {this->xHigh = xHigh;}
	bool Check(int64 id) {
		double x = data->x(id);
		if (!IsNull(xHigh) && xHigh < x)
			return false;
		if (!IsNull(xLow) && xLow > x)
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
		if (!IsNull(xHigh) && xHigh < x)
			return Null;
		if (!IsNull(xLow) && xLow > x)
			return Null;
		return x;
	}
	virtual double f(double x) {
		if (!IsNull(xHigh) && xHigh < x)
			return Null;
		if (!IsNull(xLow) && xLow > x)
			return Null;
		return data->f(x);
	}
	virtual double MinX() 				{return xLow;}
	virtual double MaxX() 				{return xHigh;}
	virtual inline int64 GetCount()	{
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
	DataReverse(DataSource &data) {Init(data);}
	void Init(DataSource *data) {Init(*data);}
	void Init(DataSource &data) {
		ASSERT(!data.IsExplicit() && !data.IsParam());
		this->data = &data;
	}
	virtual inline double y(int64 id) {return data->y(GetCount() - id - 1);}
	virtual inline double x(int64 id) {return data->x(GetCount() - id - 1);}
	virtual int64 GetCount()		  {return data->GetCount();}
};

class DataReverseX : public DataSource {
private:
	DataSource *data;

public:
	DataReverseX() : data(0) {}
	DataReverseX(DataSource &data) {Init(data);}
	void Init(DataSource &data) {
		ASSERT(!data.IsExplicit() && !data.IsParam());
		this->data = &data;
	}
	virtual inline double y(int64 id) {return data->y(id);}
	virtual inline double x(int64 id) {return data->x(GetCount() - id - 1);}
	virtual int64 GetCount()		  {return data->GetCount();}
};

class DataAppend : public DataSource {
protected:
	DataSource *data1, *data2;

public:
	DataAppend() : data1(0), data2(0) {}
	DataAppend(DataSource &data1, DataSource &data2) {Init(data1, data2);}
	void Init(DataSource &data1, DataSource &data2) {
		ASSERT(!data1.IsExplicit() && !data1.IsParam() && !data2.IsExplicit() && !data2.IsParam());
		this->data1 = &data1;
		this->data2 = &data2;
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
	virtual int64 GetCount()			{return data1->GetCount() + data2->GetCount();}
};

class DataRange : public DataAppend {
public:
	DataRange() : DataAppend() {}
	DataRange(DataSource &data1, DataSource &data2) {Init(data1, data2);}
	void Init(DataSource &data1, DataSource &data2) {
		ASSERT(!data1.IsExplicit() && !data1.IsParam() && !data2.IsExplicit() && !data2.IsParam());
		this->data1 = &data1;
		rev.Init(data2);
		this->data2 = &rev;
	}
private:
	DataReverse rev;	
};
	
class DataStackedY {
public:
	DataStackedY() : is100(false) {}
	void Set100(bool is100)	  	 {this->is100 = is100;}
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
		EachDataStackedY() : data(0), index(-1), parent(0) {}
		void Init(DataSource &data, int index, DataStackedY *parent) {
			ASSERT(!data.IsExplicit() && !data.IsParam());
			this->data = &data;
			this->index = index;
			this->parent = parent;
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
		virtual int64 GetCount() {
			return data->GetCount();
		}
	private:
		DataSource *data;
		int index;
		DataStackedY *parent;
	};
	
	EachDataStackedY &Get(int id) {return eachData[id];}

protected:
	Array<EachDataStackedY> eachData;
	bool is100;
};
	
class CArray : public DataSource {
private:
	double *yData, *xData, *zData;
	int64 numData;
	double x0, deltaX;
	
public:
	CArray(double *yData, int numData, double x0, double deltaX) : yData(yData), numData(numData), x0(x0), deltaX(deltaX) {xData = NULL;}
	CArray(double *yData, double *xData, int numData) : yData(yData), xData(xData), numData(numData) {zData = NULL; x0 = deltaX = 0;}
	CArray(double *yData, double *xData, double *zData, int numData) : yData(yData), xData(xData), zData(zData), numData(numData) {x0 = deltaX = 0;}
	virtual inline double y(int64 id) 	{return yData[ptrdiff_t(id)];}
	virtual inline double x(int64 id) 	{return xData ? xData[ptrdiff_t(id)] : id*deltaX + x0;}
	virtual double znFixed(int n, int64 id); 
	virtual int GetznFixedCount()		{return 1;}
	virtual inline int64 GetCount()		{return numData;}
};

template <class Y>
class VectorY : public DataSource {
private:
	Vector<Y> *yData;
	double x0, deltaX;

public:
	VectorY() : yData(0), x0(0), deltaX(0) {}
	VectorY(Vector<Y> &yData, double x0, double deltaX) {Init(yData, x0, deltaX);}
	void Init(Vector<Y> &yData, double x0, double deltaX) {
		this->yData = &yData;
		this->x0 = x0;
		this->deltaX = deltaX;
	}	
	virtual inline double y(int64 id)	{return (*yData)[int(id)];}
	virtual inline double x(int64 id) 	{return id*deltaX + x0;}
	virtual inline int64 GetCount()		{return yData->GetCount();}
	virtual double MinX() 				{return x0;}	
	virtual double MaxX() 				{return x0 + (yData->GetCount() - 1)*deltaX;}	
	virtual double AvgX() 				{return x0 + ((yData->GetCount() - 1)*deltaX)/2.;}
};	

template <class Y>
class ArrayY : public DataSource {
private:
	Upp::Array<Y> *yData;
	double x0, deltaX;

public:
	ArrayY() : yData(0), x0(0), deltaX(0) {}
	ArrayY(Upp::Array<Y> &yData, double x0, double deltaX) {Init(yData, x0, deltaX);}
	void Init(Upp::Array<Y> &yData, double x0, double deltaX) {
		this->yData = &yData;
		this->x0 = x0;
		this->deltaX = deltaX;
	}	
	virtual inline double y(int64 id)	{return (*yData)[ptrdiff_t(id)];}
	virtual inline double x(int64 id) 	{return id*deltaX + x0;}
	virtual inline int64 GetCount()		{return yData->GetCount();}
	virtual double MinX() 				{return x0;}	
	virtual double MaxX() 				{return x0 + yData->GetCount()*deltaX;}	
	virtual double AvgX() 				{return (x0 + yData->GetCount()*deltaX)/2.;}
};

template <class Y>
class VectorVectorY : public DataSource {
private:
	Vector<Vector<Y> > *data;
	bool useRows;
	int idx, idy;
	Vector<int> idsx, idsy, idsFixed;
	int beginData;
	int64 numData;
	
public:
	VectorVectorY() : data(0), useRows(true), beginData(0), numData(Null), idx(0), idy(1) {}
	VectorVectorY(Vector<Vector<Y> > &data, int idx, int idy, 
				  Vector<int> &idsx, Vector<int> &idsy, Vector<int> &idsFixed, 
				  bool useRows = true, int beginData = 0, int numData = Null) {
		Init(data, idx, idy, idsx, idsy, idsFixed, useRows, beginData, numData);
	}
	void Init(Vector<Vector<Y> > &data, int idx, int idy, Vector<int> &idsx, Vector<int> &idsy, Vector<int> &idsFixed, 
			  bool useRows = true, int beginData = 0, int numData = Null) {
		this->data = &data;
		this->useRows = useRows;
		
		this->idx = idx;
		this->idy = idy;
		this->idsx = clone(idsx);
		this->idsy = clone(idsy);
		this->idsFixed = clone(idsFixed);
		this->beginData = beginData;
		this->numData = numData;
		if (IsNull(numData)) {
			if (!useRows) {
				if (data.IsEmpty())
					this->numData = 0;
				else	
					this->numData = data[0].GetCount() - beginData;
			} else
				this->numData = data.GetCount() - beginData;
		}
	}
	void Init(Vector<Vector<Y> > &data, int idx, int idy, bool useRows = true, int beginData = 0, int numData = Null) {
		static Vector<int> idsVoid;
		Init(data, idx, idy, idsVoid, idsVoid, idsVoid, useRows, beginData, numData);
	}
	virtual inline double y(int64 id) {
		if (!IsNull(idy) && idy >= 0) {
			if (useRows) 
				return (*data)[beginData + int(id)][idy];
			else
				return (*data)[idy][beginData + int(id)];
		} else {
			if (GetznyCount(id) == 0)
				return Null;
			double ret = 0;
			for (int i = 0; i < GetznyCount(id); ++i) 
				ret += zny(i, id);
			return ret/GetznyCount(id);
		}
	}
	virtual inline double x(int64 id) {return useRows ? (*data)[beginData + int(id)][idx] : (*data)[idx][beginData + int(id)];}
	//virtual inline double xn(int n, int64 id) 	{return useRows ? (*data)[beginData + int(id)][ids[n]] : (*data)[ids[n]][beginData + int(id)];}
	virtual inline int64 GetCount()		{return numData;};
	virtual double znx(int n, int64 id)	{return useRows ? (*data)[beginData + int(id)][idsx[n]] : (*data)[idsx[n]][beginData + int(id)];}
	virtual double zny(int n, int64 id)	{
		if (!IsNull(idy) && idy < 0) 
			return useRows ? (*data)[beginData + int(id)][n - idy] : (*data)[n - idy][beginData + int(id)];	
		return useRows ? (*data)[beginData + int(id)][idsy[n]] : (*data)[idsy[n]][beginData + int(id)];
	}
	virtual double znFixed(int n, int64 id)	{return useRows ? (*data)[beginData + int(id)][idsFixed[n]] : (*data)[idsFixed[n]][beginData + int(id)];}
	int GetznxCount()						{return idsx.GetCount();}
	virtual int GetznyCount(int64 id) {
		if (!IsNull(idy) && idy < 0) 
			return (useRows ? (*data)[beginData + int(id)].GetCount() : (*data).GetCount()) + idy;
		return idsy.GetCount();
	}
	virtual int GetznFixedCount()		{return idsFixed.GetCount();}
};

class VectorDouble : public DataSource {
private:
	const Vector<double> *xData, *yData;

public:
	VectorDouble(const Vector<double> &yData, Vector<double> &xData) : xData(&xData), yData(&yData) {}
	virtual inline double y(int64 id)	{return (*yData)[int(id)];}
	virtual inline double x(int64 id) 	{return (*xData)[int(id)];}
	virtual inline int64 GetCount()		{return min(xData->GetCount(), yData->GetCount());}
};

class ArrayDouble : public DataSource {
private:
	const Upp::Array<double> *xData, *yData;

public:
	ArrayDouble(const Upp::Array<double> &yData, Upp::Array<double> &xData) : xData(&xData), yData(&yData) {}
	virtual inline double y(int64 id)	{return (*yData)[int(id)];}
	virtual inline double x(int64 id) 	{return (*xData)[int(id)];}
	virtual inline int64 GetCount()		{return min(xData->GetCount(), yData->GetCount());}
};

class VectorPointf : public DataSource {
private:
	const Vector<Pointf> *data;

public:
	VectorPointf() : data(0) {}
	VectorPointf(const Vector<Pointf> &data){Init(&data);}
	VectorPointf(Vector<Pointf> *data) 		{Init(data);}
	void Init(const Vector<Pointf> *_data) 	{data = _data;}
	void Init(const Vector<Pointf> &_data) 	{data = &_data;}
	virtual inline double y(int64 id)		{return (*data)[int(id)].y;}
	virtual inline double x(int64 id) 		{return (*data)[int(id)].x;}
	virtual inline int64 GetCount()			{return data->GetCount();}
};	

class ArrayPointf : public DataSource {
private:
	Upp::Array<Pointf> *data;

public:
	ArrayPointf(Upp::Array<Pointf> &data) : data(&data) {}
	virtual inline double y(int64 id)	{return (*data)[int(id)].y;}
	virtual inline double x(int64 id) 	{return (*data)[int(id)].x;}
	virtual inline int64 GetCount()		{return data->GetCount();}
};	

template <class X, class Y>
class VectorMapXY : public DataSource {
private:
	VectorMap<X, Y> *data;

public:
	VectorMapXY(VectorMap<X, Y> &data) : data(&data) {}
	virtual inline double y(int64 id)	{return (*data)[int(id)];}
	virtual inline double x(int64 id) 	{return (*data).GetKey(int(id));}
	virtual inline int64 GetCount()		{return data->GetCount();}
};	

template <class X, class Y>
class ArrayMapXY : public DataSource {
private:
	ArrayMap<X, Y> *data;

public:
	ArrayMapXY(ArrayMap<X, Y> &data) : data(&data) {}
	virtual inline double y(int64 id)	{return (*data)[int(id)];}
	virtual inline double x(int64 id) 	{return (*data).GetKey(int(id));}
	virtual inline int64 GetCount()		{return data->GetCount();}
};		

class FuncSource : public DataSource {
protected:
	Function <double(double)> function;

public:
	FuncSource() {isExplicit = true;}
	FuncSource(Function <double(double)> function) : function(function) {isExplicit = true;}
	virtual inline double f(double x)	{return function(x);}
};

class FuncSourceV : public DataSource {
private:
	Event<double&, double> function;

public:
	FuncSourceV(Event<double&, double> function) : function(function) {isExplicit = true;}
	virtual inline double f(double x)	{double y; function(y, x); return y;}
};

class FuncSourcePara : public DataSource {
private:
	Function <Pointf(double)> function;
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;
	
public:
	FuncSourcePara(Function <Pointf(double)> function, int np, double from, double to) : 
							function(function), numPoints(np), minT(from), maxT(to) {
		isParam = true; 
		lastT = Null;
	}
	virtual inline double y(double t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	virtual inline double x(double t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual inline int64 GetCount()	{return numPoints;}
};	

typedef Event<double&, double> PlotExplicFunc; 
typedef Function<void(Pointf&, double)> PlotParamFunc;


class PlotExplicFuncSource : public DataSource {
private:
	PlotExplicFunc function;

public:
	PlotExplicFuncSource(PlotExplicFunc &function) : function(function) {isExplicit = true;}
	virtual inline double f(double t)	{double y; function(y, t); return y;}
};	

class PlotParamFuncSource : public DataSource {
private:
	PlotParamFunc function;
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;

public:
	PlotParamFuncSource(PlotParamFunc function, int np, double from, double to) : 
						function(function), numPoints(np), minT(from), maxT(to) {
		isParam = true; 
		lastT = Null;
	}
	inline double y(double t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.y;
	}
	inline double x(double t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = t;
		}
		return lastPointf.x;
	}
	virtual inline int64 GetCount()	{return numPoints;}
};	
	
struct PointfLess {
	bool operator () (const Pointf& a, const Pointf& b) const { return a.x < b.x; }
};

class DataSourceSurf {
public:
	DataSourceSurf() : isExplicit(false), key(1212121) {}
	virtual ~DataSourceSurf() 			{key = 0;}	
	virtual double z(double x, double y)= 0;
	
	virtual bool IsEmpty()				= 0;
	bool IsDeleted()					{return key != 1212121;}
	bool IsExplicit()					{return isExplicit;}
	
	virtual double MinX()				= 0;
	virtual double MaxX()				= 0;
	virtual double MinY()				= 0;
	virtual double MaxY()				= 0;
	virtual double MinZ()				= 0;
	virtual double MaxZ()				= 0;

	Vector<Pointf> GetIsoline(double thres, const Rectf &area, double deltaX, double deltaY);
	Vector<Pointf> GetIsolines(const Vector<double> &vals, const Rectf &area, double deltaX, double deltaY);

protected:
	bool isExplicit;
	
private:
	int key;
};

template <class T>
inline T LinearInterpolate(T x, T x0, T x1, T v0, T v1) {
	ASSERT(x1 != x0);
  	return ((x1 - x)/(x1 - x0))*v0 + ((x - x0)/(x1 - x0))*v1;
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
	T r0 = LinearInterpolate(y, y0, y1, x00, x01);
	T r1 = LinearInterpolate(y, y0, y1, x10, x11);
	
	return LinearInterpolate(z, z0, z1, r0, r1);
}

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
	void Inter(Interpolate inter)	{this->inter = inter;}
	
	double z_area(Getdatafun getdataX, Getdatafun getdataY, Getdatafun getdata, 
					double x, double y);
	double z_point(Getdatafun getdataX, Getdatafun getdataY, Getdatafun getdata, 
					double x, double y);

	virtual inline double x(int d)				{/*NEVER();*/	return Null;}
	virtual inline double y(int d)				{/*NEVER();*/	return Null;}
	virtual inline double data(int d)			{/*NEVER();*/	return Null;}
	
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
			Interpolate inter, bool areas) {Init(data, xAxis, yAxis, inter, areas);}
	void Init(Vector<double> &data, Vector<double> &xAxis, Vector<double> &yAxis, 
					Interpolate inter, bool areas) {
		ASSERT(areas ?  (data.GetCount() == (xAxis.GetCount() - 1)*(yAxis.GetCount() - 1)) : true);
		ASSERT(!areas ? (data.GetCount() == xAxis.GetCount()*yAxis.GetCount()) : true);
		this->pdata = &data;
		this->lendata = data.GetCount();
		this->pxAxis = &xAxis;
		this->lenxAxis = xAxis.GetCount();
		this->pyAxis = &yAxis;
		this->lenyAxis = yAxis.GetCount();
		this->inter = inter;
		this->areas = areas;
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
	TableDataCArray() : pdata(0), pxAxis(0), pyAxis(0)/*, lendata(-23), lenxAxis(0), lenyAxis(0)*/ {}
	TableDataCArray(double *data, int lendata, double *xAxis, int lenxAxis, double *yAxis, int lenyAxis, 
					Interpolate inter, bool areas) {Init(data, lendata, xAxis, lenxAxis, yAxis, lenyAxis, inter, areas);}
	void Init(double *data, int lendata, double *xAxis, int lenxAxis, double *yAxis, int lenyAxis, 
					Interpolate inter, bool areas) {
		ASSERT(areas ?  (lendata == (lenxAxis - 1)*(lenyAxis - 1)) : true);
		ASSERT(!areas ? (lendata == lenxAxis*lenyAxis) : true);
		this->pdata = data;
		this->lendata = lendata;
		this->pxAxis = xAxis;
		this->lenxAxis = lenxAxis;
		this->pyAxis = yAxis;
		this->lenyAxis = lenyAxis;
		this->inter = inter;
		this->areas = areas;
	}
	virtual inline double x(int id) 	{return pxAxis[id];}
	virtual inline double y(int id) 	{return pyAxis[id];}
	virtual inline double data(int id) 	{return pdata[id];}
	
private:
	double *pdata;
	//int lendata;
	double *pxAxis;
	//int lenxAxis;
	double *pyAxis;
	//int lenyAxis;
};


class ExplicitData : public DataSourceSurf {
public:
	ExplicitData() {}
	ExplicitData(Function<double (double x, double y)> funz, double minX, double maxX, double minY, double maxY) {
		Init(funz, minX, maxX, minY, maxY);
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
	
}

#endif
