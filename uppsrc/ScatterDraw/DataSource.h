#ifndef _ScatterDraw_DataSource_h_
#define _ScatterDraw_DataSource_h_

NAMESPACE_UPP

class DataSource {
public:
	typedef double (DataSource::*Getdatafun)(int64 id);

	DataSource() : isParam(false), isExplicit(false), key(111111) {}
	virtual ~DataSource() 				{key = 0;}	
	virtual double y(int64 id)			{return xn(0, id);}
	virtual double x(int64 id)			{return xn(1, id);}
	virtual double xn(int n, int64 id)	{NEVER();	return Null;}
	virtual double y(double t)			{return xn(0, t);}
	virtual double x(double t)			{return xn(1, t);}
	virtual double xn(int n, double t)	{NEVER();	return Null;}
	virtual double f(double x)			{NEVER();	return Null;}
	virtual double f(Vector<double> xn)	{NEVER();	return Null;}
	virtual int64 GetCount()			{NEVER();	return Null;}
	bool IsParam()						{return isParam;}
	bool IsExplicit()					{return isExplicit;}
	bool IsDeleted()					{return key != 111111;}

	virtual double MinY() 		{return Min(&DataSource::y);}	
	virtual double MinX() 		{return Min(&DataSource::x);}	

	virtual double MaxY() 		{return Max(&DataSource::y);}	
	virtual double MaxX() 		{return Max(&DataSource::x);}	
	
	virtual double AvgY() 		{return Avg(&DataSource::y);}		
	virtual double StdDevY(double avg = Null) 	{return StdDev(&DataSource::y, avg);}	
	virtual double VarianceY(double avg = Null) {return Variance(&DataSource::y, avg);}	

	double Min(Getdatafun getdata);
	double Max(Getdatafun getdata);
	double Avg(Getdatafun getdata);
	double StdDev(Getdatafun getdata, double avg = Null);
	double Variance(Getdatafun getdata, double avg = Null);
	double SinEstim_Amplitude(double avg = Null);
	bool SinEstim_FreqPhase(double &frequency, double &phase, double avg = Null);
		
protected:
	bool isParam, isExplicit;

private:
	int key;
};

class CArray : public DataSource {
private:
	double *yData, *x1Data, *x2Data;
	int64 numData;
	double x0, deltaX;
	
public:
	CArray(double *yData, int numData, double x0, double deltaX) : yData(yData), numData(numData), x0(x0), deltaX(deltaX) {x1Data = NULL;}
	CArray(double *yData, double *xData, int numData) : yData(yData), x1Data(xData), numData(numData) {x2Data = NULL; x0 = deltaX = 0;}
	CArray(double *yData, double *x1Data, double *x2Data, int numData) : yData(yData), x1Data(x1Data), x2Data(x2Data), numData(numData) {x0 = deltaX = 0;}
	virtual inline double y(int64 id) 	{return yData[ptrdiff_t(id)];}
	virtual inline double x(int64 id) 	{return x1Data ? x1Data[ptrdiff_t(id)] : id*deltaX + x0;}
	virtual double xn(int n, int64 id);
	virtual inline int64 GetCount()		{return numData;}
};

template <class Y>
class VectorY : public DataSource {
private:
	Vector<Y> *yData;
	double x0, deltaX;

public:
	VectorY(Vector<Y> &yData, double x0, double deltaX) : yData(&yData), x0(x0), deltaX(deltaX) {}
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
	ArrayY(Upp::Array<Y> &yData, double x0, double deltaX) : yData(&yData), x0(x0), deltaX(deltaX) {}
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
	bool useCols;
	Vector<int> ids;
	int beginData;
	int64 numData;
	
public:
	VectorVectorY() : data(0), useCols(true), beginData(0), numData(Null) {ids << 0 << 1;}
	VectorVectorY(Vector<Vector<Y> > &data, Vector<int> &ids, bool useCols = true, int beginData = 0, int numData = Null) : 
		data(&data), useCols(useCols), beginData(beginData), numData(numData) {
		Init(data, ids, useCols, beginData, numData);
	}
	void Init(Vector<Vector<Y> > &_data, Vector<int> &_ids, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		data = &_data;
		useCols = _useCols;
		
		ids.SetCount(_ids.GetCount());
		for (int i = 0; i < ids.GetCount(); ++i)
			ids[i] = _ids[i];
		beginData = _beginData;
		numData = _numData;
		if (IsNull(_numData)) {
			if (!useCols) {
				if (data->IsEmpty())
					numData = 0;
				else	
					numData = (*data)[0].GetCount() - beginData;
			} else
				numData = data->GetCount() - beginData;
		}
	}
	void Init(Vector<Vector<Y> > &_data, int idY, int idX, bool _useCols = true, int _beginData = 0, int _numData = Null) {
		Vector<int> ids;
		ids << idY << idX;
		Init(_data, ids, _useCols, _beginData, _numData);
	}
	virtual inline double y(int64 id)	{return useCols ? (*data)[beginData + int(id)][ids[0]] : (*data)[ids[0]][beginData + int(id)];}
	virtual inline double x(int64 id) {
		if (IsNull(ids[1]))
			return double(id);
		else
			return useCols ? (*data)[beginData + int(id)][ids[1]] : (*data)[ids[1]][beginData + int(id)];
	}
	virtual inline double xn(int n, int64 id)	{return useCols ? (*data)[beginData + int(id)][ids[n]] : (*data)[ids[n]][beginData + int(id)];}
	virtual inline int64 GetCount()	{return numData;};
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
	VectorPointf(const Vector<Pointf> &data) : data(&data) 	{}
	VectorPointf(Vector<Pointf> *data) : data(data) 		{}
	virtual inline double y(int64 id)	{return (*data)[int(id)].y;}
	virtual inline double x(int64 id) 	{return (*data)[int(id)].x;}
	virtual inline int64 GetCount()		{return data->GetCount();}
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
private:
	double (*function)(double);

public:
	FuncSource(double (*function)(double)) : function(function) {isExplicit = true;}
	virtual inline double f(double x)	{return function(x);}
};

class FuncSourceV : public DataSource {
private:
	void (*function)(double&, double);

public:
	FuncSourceV(void (*function)(double&, double)) : function(function) {isExplicit = true;}
	virtual inline double f(double x)	{double y; function(y, x); return y;}
};

class FuncSourcePara : public DataSource {
private:
	Pointf (*function)(double);
	Pointf lastPointf;
	double lastT;
	int numPoints;
	double minT, maxT;
	
public:
	FuncSourcePara(Pointf (*function)(double), int np, double from, double to) : 
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

typedef Callback2<double&, double> PlotExplicFunc; 
typedef Callback2<Pointf&, double> PlotParamFunc;

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

END_UPP_NAMESPACE

#endif
