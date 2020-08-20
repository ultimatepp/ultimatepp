#ifndef _STEM4U_TSP_h_
#define _STEM4U_TSP_h_

namespace Upp {
	
template<typename T>
T factSum(T val) {
	if (val <= 0)
		throw std::invalid_argument("factSum() accepts only nonzero positive numbers");
	T ret = 0;
	while (val > 0)
		ret += val--;
	return ret; 
}

class TriangularIndex {
public:
	TriangularIndex()		  	{};
	TriangularIndex(int _len)  	{len = _len;}
	
	inline int GetIndex(int row, int col) const {
		ASSERT(row != col);
		ASSERT(row >= 0 && col >= 0);
		if (row > col)
			Swap(row, col);
		return col + len*row - factSum(row+1);
	}
	inline int operator()(int row, int col) const  {return GetIndex(row, col);}

	int GetNumData() const 		{return factSum(len-1);}
	
private:
	int len;
};

template<typename T>
T TSP_TotalDistance(const Vector<T> &distances, const Vector<int> &order) {
	ASSERT(order.size() >= 3);
	TriangularIndex ind(order.size()-1);
	T ret = 0;
	for (int i = 0; i < order.size()-1; ++i) 
		ret += distances[ind(order[i], order[i+1])];
	return ret;
}

template<typename T>
void TSP_NearestNeighbor(const Vector<T> &distances, int sz, Vector<int> &order) {
	Vector<int> list(sz-1);
	for (int i = 0; i < sz-1; ++i)
		list[i] = i+1;
	
	TriangularIndex ind(sz);
	
	order.Clear();
	int point = 0;
	order << point;
	while (list.size() > 0) {
		T mind = std::numeric_limits<T>::max();
		int idmin;
		for (int i = 0; i < list.size(); ++i) {
			T d = distances[ind(point, list[i])];
			if (mind > d) {
				mind = d;
				idmin = i;
			}
		}
		point = list[idmin];
		order << point;
		list.Remove(idmin);
	}
	order << 0;
}

template<typename T>
void TSP_2_Opt(const Vector<T> &distances, int sz, Vector<int> &order) {
	TriangularIndex ind(sz);

	int idchange;
	do {
		idchange = -1;
		T bestSaving = 0;
		
		for (int i = 0; i < order.size()-1 && idchange < 0; ++i) {
			for (int j = 0; j < order.size()-1; ++j) {
				if (i != j && order[i]   != order[j] && order[i]   != order[j+1]
						   && order[i+1] != order[j] && order[i+1] != order[j+1]) {
					T saving = distances[ind(order[i],   order[i+1])] 
							 + distances[ind(order[j],   order[j+1])] 
							 - distances[ind(order[i],   order[j])] 
							 - distances[ind(order[i+1], order[j+1])];
					if (saving > 0 && saving > bestSaving) {
						idchange = j;
						bestSaving = saving;
					}
				}
			}
			if (idchange >= 0) {
				Swap(order[i+1], order[idchange]);
				break;
			}
		}
	} while (idchange >= 0);
}

template<typename T>
T TSP(const Vector<T> &distances, int sz, Vector<int> &order) {
	if (order.IsEmpty()) 
		TSP_NearestNeighbor(distances, sz, order);

	TSP_2_Opt(distances, sz, order);
	return TSP_TotalDistance(distances, order);
}

template<typename T>
T TSP(const Vector<Point_<T>>& points, Vector<int> &order) {
	const int sz = points.size();
	TriangularIndex ind(sz);
	Vector<T> distances(ind.GetNumData());

	for (int r = 0; r < sz; ++r)
		for (int c = r+1; c < sz; ++c)
			distances[ind(r, c)] = Distance(points[r], points[c]);

	return TSP(distances, order);
}

template<typename T>
T TSP(const Vector<Vector<T>>& matrix, Vector<int> &order) {
	const int sz = matrix.size();
	TriangularIndex ind(sz);
	Vector<T> distances(ind.GetNumData());

	for (int r = 0; r < sz; ++r)
		for (int c = r+1; c < sz; ++c)
			distances[ind(r, c)] = matrix[r][c];

	return TSP(distances, sz, order);
}
	
}

#endif
