#ifndef _StableSort_QStableSort_h_
#define _StableSort_QStableSort_h_

template <class T>
struct StableSortItem {
	const T& value;
	int      index;

	StableSortItem(const T& value, int index) : value(value), index(index) {}
};

template <class II, class T>
struct StableSortIterator {
	II          ii;
	int        *vi;

	typedef StableSortIterator<II, T> Iter;

	Iter&       operator ++ ()               { ++ii; ++vi; return *this; }
	Iter&       operator -- ()               { --ii; --vi; return *this; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi + i); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi - i); }
	int         operator -  (Iter b) const   { return (int)(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii <  b.ii; }

	StableSortItem<T> operator*() const      { return StableSortItem<T>(*ii, *vi); }

	friend void IterSwap(Iter a, Iter b)     { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	StableSortIterator(II ii, int *vi) : ii(ii), vi(vi) {}
};

template <class T>
struct StableSortLessCmp {
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		int q = SgnCompare(a.value, b.value);
		return q ? q < 0 : a.index < b.index;
	}
};

template <class C>
void QStableSortCmp(C& c)
{
	Buffer<int> h(c.GetCount());
	for(int i = 0; i < c.GetCount(); i++)
		h[i] = i;
	Sort(StableSortIterator<typename C::Iterator, typename C::ValueType>(c.Begin(), ~h),
	     StableSortIterator<typename C::Iterator, typename C::ValueType>(c.End(), ~h + c.GetCount()),
	     StableSortLessCmp<typename C::ValueType>());
}

template <class T>
struct StableSortLess {
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		if(a.value < b.value) return true;
		return b.value < a.value ? false : a.index < b.index;
	}
};

template <class C>
void QStableSort(C& c)
{
	Buffer<int> h(c.GetCount());
	for(int i = 0; i < c.GetCount(); i++)
		h[i] = i;
	Sort(StableSortIterator<typename C::Iterator, typename C::ValueType>(c.Begin(), ~h),
	     StableSortIterator<typename C::Iterator, typename C::ValueType>(c.End(), ~h + c.GetCount()),
	     StableSortLess<typename C::ValueType>());
}

#endif
