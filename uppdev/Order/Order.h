#ifndef _Order_Order_h_
#define _Order_Order_h_

#include <Core/Core.h>

using namespace Upp;

template <class T>
class Order {
	Vector<T> data;

public:
	int      Find(const T& x);
	int      FindAdd(const T& x);
	void     Add(const T& x);
	const T& operator[](int i) const          { return data[i]; }
	int      GetCount() const                 { return data.GetCount(); }
	int      FindUpperBound(const T& x) const;
	int      FindLowerBound(const T& x) const;
};

template <class T>
inline int Order<T>::FindUpperBound(const T& x) const
{
	int l = 0;
	int h = data.GetCount();
	while(l < h) {
		int mid = (l + h) >> 1;
		if(x < data[mid])
			h = mid;
		else
			l = mid + 1;
	}
	return l;
}

template <class T>
inline int Order<T>::FindLowerBound(const T& x) const
{
	int l = 0;
	int h = data.GetCount();
	while(l < h) {
		int mid = (l + h) >> 1;
		if(data[mid] < x)
			l = mid + 1;
		else
			h = mid;
	}
	return l;
}

template <class T>
void Order<T>::Add(const T& x)
{
	data.Insert(FindUpperBound(x), x);
}

template <class T>
int Order<T>::Find(const T& x)
{
	int i = FindLowerBound(x);
	return i < GetCount() && data[i] == x ? i : -1;
}

template <class T>
int Order<T>::FindAdd(const T& x)
{
	int i = FindUpperBound(x);
	if(i && data[i - 1] == x)
		return i - 1;
	data.Insert(i, x);
	return i;
}

#endif
