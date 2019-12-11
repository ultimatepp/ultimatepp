#ifndef _Mathlib_Histogram_h_
#define _Mathlib_Histogram_h_

namespace Upp {

class MultiDimMatrixIndex {
public:
	MultiDimMatrixIndex()				{};
	MultiDimMatrixIndex(int numAxis)	{axisDim.SetCount(numAxis);};
	
	void SetNumAxis(int numAxis)		{axisDim.SetCount(numAxis);};
	void SetAxisDim(int axis, int dim) {
		ASSERT(axis >= 0 && axis < axisDim.GetCount() && dim > 0);
		axisDim[axis] = dim;
	}
	void SetAxis1D(int dimX) {
		ASSERT(dimX > 0);
		axisDim.SetCount(1);
		axisDim[0] = dimX;
	}
	void SetAxis2D(int dimX, int dimY) {
		ASSERT(dimX > 0 && dimY > 0);
		axisDim.SetCount(2);
		axisDim[0] = dimX;
		axisDim[1] = dimY;
	}
	const Upp::Vector<int> &GetAxisDim()	{return axisDim;};
	int GetIndex(Vector<int> &index) {
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
	int GetIndex(T t, Args... args) {
		Vector<int> index;
		
		index << t;
	    AddIndex(index, args...);
	
	    return GetIndex(index);
	}
	inline int GetIndex(int x, int y) {
		ASSERT(IsValid(x, y));
		return x + axisDim[0]*y;
	}	
	inline bool IsValid(int x, int y) {
		return x >= 0 && x < axisDim[0] && y >= 0 && y < axisDim[1];
	}
	inline int GetNumAxis() {return axisDim.GetCount();}
	
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
	void AddIndex(Vector<int> &index, T t) {
		index << t;
	}	
	template<typename T, typename... Args>
	void AddIndex(Vector<int> &index, T t, Args... args) {
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
	
	Histogram &Create(DataSource &data, double min, double max, int numVals, bool isY);
	Histogram &Create(Upp::Array<HistogramDataAxis> &dataAxis, bool isY);
	Histogram &Create2D(Vector<Vector<double> > &_ranges, Vector<double> &data, double total);

	Histogram &Normalize(double val = 1);
	Histogram &Accumulative(bool _accum = true)	{accumulative = _accum;	return *this;}
	
	double Compare(const Histogram &hist);
	
	inline double &GetRange(int dim, int i)	{return ranges[dim][i];};
	inline int64 GetCount(int dim) const	{return ranges[dim].GetCount();}
	inline double GetValue(int index)		{return y(index);}
	inline double GetValue(int c, int r)	{return y(valuesIdx.GetIndex(c, r));}
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
