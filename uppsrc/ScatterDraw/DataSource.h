#ifndef _ScatterDraw_DataSource_h_
#define _ScatterDraw_DataSource_h_

NAMESPACE_UPP

class DataSource {
public:
	typedef double (DataSource::*Getdatafun)(ptrdiff_t id);

	DataSource() : isParam(false) {}
	virtual ~DataSource() {}	
	virtual double z(ptrdiff_t id)	{return Null;}
	virtual double y(ptrdiff_t id)	{return Null;}
	virtual double x(ptrdiff_t id)	{return Null;}
	virtual ptrdiff_t GetCount()	{return Null;}
	virtual ptrdiff_t GetCount(int series) {return Null;}
	bool IsParam()			{return isParam;}

	virtual double MinX() 	{return Min(&DataSource::x);}	
	virtual double MinY() 	{return Min(&DataSource::y);}	
	virtual double MinZ() 	{return Min(&DataSource::z);}	

	virtual double MaxX() 	{return Max(&DataSource::x);}				
	virtual double MaxY() 	{return Max(&DataSource::y);}	
	virtual double MaxZ() 	{return Max(&DataSource::z);}	

	virtual double AvgX() 	{return Avg(&DataSource::x);}	
	virtual double AvgY() 	{return Avg(&DataSource::y);}					
	virtual double AvgZ() 	{return Avg(&DataSource::z);}	
	
protected:
	bool isParam;

	virtual double Min(Getdatafun getdata);
	virtual double Max(Getdatafun getdata);
	virtual double Avg(Getdatafun getdata);
};

class CArray : public DataSource {
private:
	double *xData, *yData, *zData;
	ptrdiff_t numData;
	double x0, deltaX;
	
public:
	CArray(double *yData, int numData, double x0, double deltaX) : yData(yData), numData(numData), x0(x0), deltaX(deltaX) { xData = NULL; }
	CArray(double *xData, double *yData, int numData) : xData(xData), yData(yData), numData(numData) { zData = NULL; x0 = deltaX = 0; }
	CArray(double *xData, double *yData, double *zData, int numData) : xData(xData), yData(yData), zData(zData), numData(numData) { x0 = deltaX = 0; }
	virtual inline double z(ptrdiff_t id)	{ASSERT(zData);	return zData[id];}
	virtual inline double y(ptrdiff_t id) 	{return yData[id];}
	virtual inline double x(ptrdiff_t id) 	{return xData ? xData[id] : id*deltaX + x0;}
	virtual inline ptrdiff_t GetCount()		{return numData;}
};

template <class Y>
class VectorY : public DataSource {
private:
	Vector<Y> *yData;

public:
	VectorY(Vector<Y> &yData) : yData(&yData) {}
	virtual inline double y(ptrdiff_t id)	{return (*yData)[id];}
	virtual inline double x(ptrdiff_t id) 	{return id;}
	virtual inline ptrdiff_t GetCount()		{return yData->GetCount();}
};	

template <class Y>
class ArrayY : public DataSource {
private:
	Upp::Array<Y> *yData;

public:
	ArrayY(Upp::Array<Y> &yData) : yData(&yData) {}
	virtual inline double y(ptrdiff_t id)	{return (*yData)[id];}
	virtual inline double x(ptrdiff_t id) 	{return id;}
	virtual inline ptrdiff_t GetCount()		{return yData->GetCount();}
};

class VectorDouble : public DataSource {
private:
	const Vector<double> *xData, *yData;

public:
	VectorDouble(const Vector<double> &xData, Vector<double> &yData) : xData(&xData), yData(&yData) {}
	virtual inline double y(ptrdiff_t id)	{return (*yData)[int(id)];}
	virtual inline double x(ptrdiff_t id) 	{return (*xData)[int(id)];}
	virtual inline ptrdiff_t GetCount()		{return xData->GetCount();}
};

class ArrayDouble : public DataSource {
private:
	const Upp::Array<double> *xData, *yData;

public:
	ArrayDouble(const Upp::Array<double> &xData, Upp::Array<double> &yData) : xData(&xData), yData(&yData) {}
	virtual inline double y(ptrdiff_t id)	{return (*yData)[int(id)];}
	virtual inline double x(ptrdiff_t id) 	{return (*xData)[int(id)];}
	virtual inline ptrdiff_t GetCount()		{return xData->GetCount();}
};

class VectorPointf : public DataSource {
private:
	const Vector<Pointf> *data;

public:
	VectorPointf(const Vector<Pointf> &data) : data(&data) 	{}
	VectorPointf(Vector<Pointf> *data) : data(data) 		{}
	virtual inline double y(ptrdiff_t id)	{return (*data)[int(id)].y;}
	virtual inline double x(ptrdiff_t id) 	{return (*data)[int(id)].x;}
	virtual inline ptrdiff_t GetCount()		{return data->GetCount();}
};	

class ArrayPointf : public DataSource {
private:
	Upp::Array<Pointf> *data;

public:
	ArrayPointf(Upp::Array<Pointf> &data) : data(&data) {}
	virtual inline double y(ptrdiff_t id)	{return (*data)[int(id)].y;}
	virtual inline double x(ptrdiff_t id) 	{return (*data)[int(id)].x;}
	virtual inline ptrdiff_t GetCount()		{return data->GetCount();}
};	

template <class X, class Y>
class VectorMapXY : public DataSource {
private:
	VectorMap<X, Y> *data;

public:
	VectorMapXY(VectorMap<X, Y> &data) : data(&data) {}
	virtual inline double y(ptrdiff_t id)	{return (*data)[int(id)];}
	virtual inline double x(ptrdiff_t id) 	{return (*data).GetKey(int(id));}
	virtual inline ptrdiff_t GetCount()		{return data->GetCount();}
};	

template <class X, class Y>
class ArrayMapXY : public DataSource {
private:
	ArrayMap<X, Y> *data;

public:
	ArrayMapXY(ArrayMap<X, Y> &data) : data(&data) {}
	virtual inline double y(ptrdiff_t id)	{return (*data)[id];}
	virtual inline double x(ptrdiff_t id) 	{return (*data).GetKey(id);}
	virtual inline ptrdiff_t GetCount()		{return data->GetCount();}
};		
 
class FuncSource : public DataSource {
private:
	double (*function)(double);

public:
	FuncSource(double (*function)(double)) : function(function) {}
	virtual inline double y(ptrdiff_t t)	{return function(double(t));}
	virtual inline double x(ptrdiff_t t) 	{return double(t);}
	virtual inline ptrdiff_t GetCount()		{return Null;}
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
		lastT = Null;
		isParam = true;
	}
	virtual inline double y(ptrdiff_t t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = double(t);
		}
		return lastPointf.y;
	}
	virtual inline double x(ptrdiff_t t) {
		if (IsNull(lastT) || t != lastT) {
			lastPointf = function(minT + t*(maxT-minT)/numPoints);
			lastT = double(t);
		}
		return lastPointf.x;
	}
	virtual inline ptrdiff_t GetCount()	{return numPoints;}
};	

typedef Callback2<double&, double> PlotFunc; 
typedef Callback2<Pointf&, double> PlotParamFunc;

class PlotFuncSource : public DataSource {
private:
	PlotFunc function;

public:
	PlotFuncSource() 										{}
	PlotFuncSource(PlotFunc &function) : function(function) {}
	virtual inline double y(ptrdiff_t t)	{double y; function(y, double(t)); return y;}
	virtual inline double x(ptrdiff_t t) 	{return double(t);}
	virtual inline ptrdiff_t GetCount()		{return -1;}
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
		lastT = Null;
		isParam = true;
	}
	inline double y(ptrdiff_t t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = double(t);
		}
		return lastPointf.y;
	}
	
	inline double x(ptrdiff_t t) {
		if (IsNull(lastT) || t != lastT) {
			function(lastPointf, minT + t*(maxT-minT)/numPoints);
			lastT = double(t);
		}
		return lastPointf.x;
	}
	virtual inline ptrdiff_t GetCount()	{return numPoints;}
};	

END_UPP_NAMESPACE

#endif
