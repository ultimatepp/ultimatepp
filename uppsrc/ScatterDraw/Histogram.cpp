#include <Core/Core.h>
#include <ScatterDraw/DataSource.h>
#include <plugin/Eigen/Eigen.h>
#include <ScatterDraw/Histogram.h>

namespace Upp {

Histogram::Histogram() {
	Clear();
}

Histogram::Histogram(const Histogram &hist) {
	*this = hist;
}

Histogram &Histogram::operator=(const Histogram &hist) {
	totalVals = hist.totalVals;
	
	values = hist.values; 
	ranges <<= hist.ranges;
	
	return *this;
}

void Histogram::Clear() {
	ranges.Clear();	
	values.resize(0);
}

Histogram &Histogram::Create2D(const Vector<Vector<double> > &_ranges, const Vector<double> &data, double total) {
	Clear();
	
	int numAxis = 2;
	int numVals = 1;
	for (int ix = 0; ix < numAxis; ++ix)
		numVals *= _ranges[ix].GetCount();
	valuesIdx.SetNumAxis(numAxis);
	for (int ix = 0; ix < numAxis; ++ix)
		valuesIdx.SetAxisDim(ix, _ranges[ix].GetCount());
	values.resize(numVals);
	values.setZero();
	ranges.SetCount(numAxis);
	for (int ix = 0; ix < numAxis; ++ix) {
		ranges[ix].SetCount(_ranges[ix].GetCount());
		for (int d = 0; d < _ranges[ix].GetCount(); ++d) 
			ranges[ix][d] = _ranges[ix][d];
	}
	totalVals = total;
	for (int i = 0; i < data.GetCount(); ++i) 
		values(ptrdiff_t(i)) = data[i];
	return *this;
}
	
Histogram &Histogram::Create(Upp::Array<HistogramDataAxis> &dataAxis, bool isY) {
	Clear();
	
	int numAxis = dataAxis.GetCount();
	int numVals = 1;
	for (int ix = 0; ix < numAxis; ++ix)
		numVals *= dataAxis[ix].numVals;
	valuesIdx.SetNumAxis(numAxis);
	for (int ix = 0; ix < numAxis; ++ix)
		valuesIdx.SetAxisDim(ix, dataAxis[ix].numVals);
	values.resize(numVals);
	values.setZero();
	ranges.SetCount(numAxis);
	Vector<double> delta;
	delta.SetCount(numAxis);
	for (int ix = 0; ix < numAxis; ++ix) {
		ranges[ix].SetCount(dataAxis[ix].numVals);
		delta[ix] = (dataAxis[ix].max - dataAxis[ix].min + 1)/dataAxis[ix].numVals;
		for (int d = 0; d < dataAxis[ix].numVals; ++d) 
			ranges[ix][d] = (d + 1)*delta[ix];
	}
	
	totalVals = double(dataAxis[0].data.GetCount());
	Vector<int> index;
	index.SetCount(numAxis);
	for (int64 i = 0; i < totalVals; ++i) {
		for (int ix = 0; ix < numAxis; ++ix) {
			double d = isY ? dataAxis[ix].data.y(i) : dataAxis[ix].data.x(i);
			if (!!IsNum(d)) {
				double val = d - dataAxis[ix].min;
				val = val/delta[ix];
				if (val >= dataAxis[ix].numVals)
					val = dataAxis[ix].numVals - 1;
				else if (val < 0)
					val = 0;
				ASSERT(int(val) >= 0 && int(val) < dataAxis[ix].numVals);
				index[ix] = int(val);
			} else
				index[ix] = Null;
		}
		if (!IsNull(index[0]))
			values(valuesIdx.GetIndex(index))++;	
	}
	return *this;	
}

Histogram &Histogram::Create(DataSource &data, double min, double max, int numVals, bool isY) {
	Clear();
	
	values.resize(numVals);
	values.setZero();
	ranges.SetCount(1);
	ranges[0].SetCount(numVals);
	double delta = (max - min)/numVals;
	for (int ii = 0; ii < numVals; ++ii) 
		ranges[0][ii] = min + (ii + 0.5)*delta;
	
	int64 total = data.GetCount();
	totalVals = 0;
	for (int64 i = 0; i < total; ++i) {
		double d = isY ? data.y(i) : data.x(i);
		if (!!IsNum(d)) {
			double val = (d - min)/delta;
			if (val >= numVals)
				val = numVals - 1;
			else if (val < 0)
				val = 0;
			values(int(val))++;	
			totalVals++;
		}
	}
	return *this;	
}

Histogram &Histogram::Normalize(double val) {
	if (totalVals == val)
		return *this;
	
	if (totalVals > 0)
		values *= val/totalVals;
	totalVals = val;
	return *this;
}

double Histogram::Compare(const Histogram &hist) {
	ASSERT(hist.values.size() == values.size());
	double res = 0;
	ptrdiff_t size = values.size();
	for(ptrdiff_t i = 0; i < size; i++) 
		res += min(values[i], hist.values[i]);
	res /= totalVals;
	return res;
}

}