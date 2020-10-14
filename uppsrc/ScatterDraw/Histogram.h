#ifndef _Mathlib_Histogram_h_
#define _Mathlib_Histogram_h_

namespace Upp {

class MultiDimMatrixIndex {
public:
	MultiDimMatrixIndex()			   {};
	template<typename... Args>
	MultiDimMatrixIndex(Args... args)  {SetAxis(args...);}
	
	void SetNumAxis(int numAxis)	   {axisDim.SetCount(numAxis);};
	void SetAxisDim(int axis, int dim) {
		ASSERT(axis >= 0 && axis < axisDim.GetCount() && dim > 0);
		axisDim[axis] = dim;
	}
	void SetAxis(int dimX) {
		ASSERT(dimX > 0);
		axisDim << dimX;
	}
	template<typename... Args>
	void SetAxis(int t, Args... args) {
		ASSERT(t > 0);
		axisDim << t;
		SetAxis(args...);
	}
		
	Upp::Vector<int> &GetAxisDim()	{return axisDim;};
	int GetIndex(const Vector<int> &index) const {
		ASSERT_(index[0] >= 0 && index[0] < axisDim[0], Format("index[0]=%d", index[0]));
		int ret = index[0];
		int factor = 1;
		for (int ix = 1; ix <= axisDim.GetCount()-1; ++ix) {
			ASSERT(index[ix] >= 0 && index[ix] < axisDim[ix]);
			factor *= axisDim[ix-1];
			ret += factor*index[ix];
		}
		return ret;
	}
	template<typename T, typename... Args>
	int GetIndex(T t, Args... args) const {
		Vector<int> index;
		
		index << t;
	    AddIndex(index, args...);
	
	    return GetIndex(index);
	}
	template<typename... Args>
	inline int operator()(Args... args) const {return GetIndex(args...);}
	
	inline int GetIndex(int x, int y) const {
		ASSERT(IsValid(x, y));
		return x + axisDim[0]*y;
	}
	inline int operator()(int x, int y) const  {return GetIndex(x, y);}
		
	inline bool IsValid(int x, int y) const  {
		return x >= 0 && x < axisDim[0] && y >= 0 && y < axisDim[1];
	}
	inline int GetNumAxis() const  {return axisDim.GetCount();}
	int GetNumData() const {
		int ret = 1;
		for (auto dim : axisDim)
			ret *= dim;
		return ret;
	}
	void Xmlize(XmlIO xml) {
		xml
			("axisDim", axisDim)	
		;	
	}
	void Jsonize(JsonIO& json) {
		json
			("axisDim", axisDim)	
		;	
	}
private:
	Vector<int> axisDim;
	
	template<typename T>
	static void AddIndex(Vector<int> &index, T t) {
		index << t;
	}	
	template<typename T, typename... Args>
	static void AddIndex(Vector<int> &index, T t, Args... args) {
		index << t;
		AddIndex(index, args...);
	}
};

struct HistogramDataAxis : Moveable<HistogramDataAxis> {
	HistogramDataAxis(DataSource &data, double min, double max, int numVals) : data(data), min(min), max(max), numVals(numVals) {}
	DataSource &data;
	double min, max;
	int numVals;
};

class Histogram : public DataSource, public Moveable<Histogram> {
public:
	Histogram();
	Histogram(const Histogram &hist);
	Histogram(const Nuller&) 	{SetNull();}
	
	void SetNull() 				{Clear();}
	bool IsNullInstance() const	{return ranges.IsEmpty();}
	
	virtual Histogram &operator=(const Histogram &hist);
	void Clear();
	
	Histogram &Create(DataSource &data, double min, double max, int numVals, bool isY = true);
	Histogram &Create(Upp::Array<HistogramDataAxis> &dataAxis, bool isY = true);
	Histogram &Create2D(const Vector<Vector<double> > &_ranges, const Vector<double> &data, double total);

	Histogram &Normalize(double val = 1);
	Histogram &Accumulative(bool _accum = true)	{accumulative = _accum;	return *this;}
	
	double Compare(const Histogram &hist);
	
	inline const double &GetRange(int dim, int i)	{return ranges[dim][i];};
	inline int64 GetCount(int dim) const	{return ranges[dim].GetCount();}
	inline double GetValue(int index)		{return y(index);}
	inline double GetValue(int c, int r)	{return y(valuesIdx(c, r));}
	inline bool IsValid(int c, int r)		{return valuesIdx.IsValid(c, r);}
	
	inline double y(int64 id) {
		ASSERT(values.size());
		if (!accumulative)	
			return values(ptrdiff_t(id));
		else {
			double ret = 0;
			for (ptrdiff_t i = 0; i <= ptrdiff_t(id); ++i)
				ret += values(i);
			return ret;
		}
	}
	inline double x(int64 id) {
		ASSERT(values.size());	
		return ranges[0][int(id)];
	}
	inline virtual int64 GetCount() const {return !values.size() ? 0 : values.size();};
	inline virtual int64 size() const 	  {return GetCount();};
	
	void Xmlize(XmlIO xml) {
		xml
			("totalVals", totalVals)	
			("valuesIdx", valuesIdx)	
			("ranges", ranges)	
		;	
		Upp::Xmlize(xml, values);
	}
	void Jsonize(JsonIO& json) {
		json
			("totalVals", totalVals)	
			("valuesIdx", valuesIdx)	
			("ranges", ranges)	
		;
		Upp::Jsonize(json, values);	
	}
protected:
	double totalVals;
	bool accumulative = false;
	
	MultiDimMatrixIndex valuesIdx;
	Eigen::VectorXd values; 
	Vector<Vector<double> > ranges;
};
	
}

#endif
