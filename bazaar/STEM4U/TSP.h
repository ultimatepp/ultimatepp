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
T TSP_NearestNeighbor(const Vector<T> &distances, int sz, Vector<int> &order) {
	Vector<int> list(sz-1);
	for (int i = 0; i < sz-1; ++i)
		list[i] = i+1;
	
	TriangularIndex ind(sz);
	
	T total = 0;
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
		total += mind;
	}
	order << 0;
	return total;
}

void OrderToConnections(const Vector<int> &order, Vector<int> &left, Vector<int> &right) {
	left = clone(order);
	left.Remove(left.size()-1);
	right = clone(order);
	right.Remove(0);
}

void ConnectionsToOrder(const Vector<int> &left, const Vector<int> &right, Vector<int> &order) {
	int next = 0;
	Vector<bool> removed(left.size(), false);
	for (int id = 0; id <= order.size(); id++) {
		for (int i = 0; i < left.size(); ++i) {
			if (!removed[i]) {
				if (left[i] == next) { 
					order[id] = next;
					next = right[i];
					removed[i] = true;
					break;
				} else if (right[i] == next) {
					order[id] = next;
					next = left[i];
					removed[i] = true;
					break;
				}
			}
		}
	}
}
	
template<typename T>
T TSP_2_Opt(const Vector<T> &distances, int sz, Vector<int> &order) {
	TriangularIndex ind(sz);

	Vector<int> left, right;
	OrderToConnections(order, left, right);
	
	while (true) {
		int idchange = -1;
		T bestSaving = 0;
		
		for (int i = 0; i < order.size()-1 && idchange < 0; ++i) {
			for (int j = i+1; j < order.size()-1; ++j) {
				if (i != j && order[i]   != order[j] && order[i]   != order[j+1]
						   && order[i+1] != order[j] && order[i+1] != order[j+1]) {
					T saving = distances[ind(order[i],   order[i+1])] 
							 + distances[ind(order[j],   order[j+1])] 
							 - distances[ind(order[i],   order[j])] 
							 - distances[ind(order[i+1], order[j+1])];
					if (saving > bestSaving) {
						idchange = j;
						bestSaving = saving;
					}
				}
			}
			if (idchange >= 0) {
				Swap(right[i], left[idchange]);
				ConnectionsToOrder(left, right, order);
				OrderToConnections(order, left, right);
				break;
			}
		}
		if (idchange < 0)
			break;
	}
	return TSP_TotalDistance(distances, order);
}

enum TSP_Init {TSP_USER, TSP_NEAREST_NEIGHBOR, TSP_CONSECUTIVE, TSP_RANDOM};

template<typename T>
T TSP(const Vector<T> &distances, int sz, Vector<int> &order, TSP_Init init) {
	if (init == TSP_NEAREST_NEIGHBOR)
		TSP_NearestNeighbor(distances, sz, order);
	else if (init == TSP_CONSECUTIVE) {
		order.Clear();
		for (int i = 0; i < sz; ++i)
			order << i;
		order << 0;
	} else if (init == TSP_RANDOM) {
		order.Clear();
		for (int i = 1; i < sz; ++i)
			order << i;
		Shuffle(order);
		order.Insert(0, 0);
		order << 0;
	}
	ASSERT(order.size()-1 == sz);
	return TSP_2_Opt(distances, sz, order);
}

template<typename T>
T TSP(const Vector<Point_<T>>& points, Vector<int> &order, TSP_Init init) {
	const int sz = points.size();
	TriangularIndex ind(sz);
	Vector<T> distances(ind.GetNumData());

	for (int r = 0; r < sz; ++r)
		for (int c = r+1; c < sz; ++c)
			distances[ind(r, c)] = Distance(points[r], points[c]);

	return TSP(distances, sz, order, init);
}

template<typename T>
T TSP(const Vector<Vector<T>>& matrix, Vector<int> &order, TSP_Init init) {
	const int sz = matrix.size();
	TriangularIndex ind(sz);
	Vector<T> distances(ind.GetNumData());

	for (int r = 0; r < sz; ++r)
		for (int c = r+1; c < sz; ++c)
			distances[ind(r, c)] = matrix[r][c];

	return TSP(distances, sz, order, init);
}

}

#endif
