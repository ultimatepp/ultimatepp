template <class I, class Less>
void FinalSort__(I begin, I end, const Less& less)
{
	if(begin == end)
		return;
	I last = end;
	--last;
	while(!(begin == last)) {
		I best = last;
		I next = last;
		I ptr = last;
		for(;;) {
			if(less(*best, *--ptr)) {  // best holds, scan for better candidate
				do
					if(ptr == begin) { // best is the final minimum
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				while(less(*best, *--ptr));
				if(ptr == begin) { // begin is the final minimum, best is 2nd least
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr; // mark position after new best as the new end of sorted array
				++next;     // it will hold only if all subseqent iterations define new best (descending order)
			}
			else
			if(ptr == begin) { // begin is the final minimum
				begin = next;  // and everything is sorted up to next
				break;
			}
			best = ptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class T, class Less>
void FinalSort__(T& c, const Less& less)
{
	FinalSort__(c.begin(), c.end(), less);
}

template <class T>
void FinalSort__(T& c)
{
	typedef ValueTypeOf<T> VT;
	FinalSort__(c.begin(), c.end(), std::less<VT>());
}

template <class I, class Less>
force_inline
void OrderIter2__(I a, I b, const Less& less)
{
	if(less(*b, *a))
		IterSwap(a, b);
}

dword  Random(dword n);

template <class I, class Less>
void Sort__(I l, I h, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			FinalSort__(l, h, less);
			return;
		}
		int pass = 4;
		for(;;) {
			I middle = l + (count >> 1);        // get the middle element
			OrderIter2__(l, middle, less);      // sort l, middle, h-1 to find median of 3
			OrderIter2__(middle, h - 1, less);
			OrderIter2__(l, middle, less);      // median is now in middle
			IterSwap(l + 1, middle);            // move median pivot to l + 1
			I ii = l + 1;
			for(I i = l + 2; i != h - 1; ++i)   // do partitioning; already l <= pivot <= h - 1
				if(less(*i, *(l + 1)))
					IterSwap(++ii, i);
			IterSwap(ii, l + 1);                // put pivot back in between partitions
			I iih = ii;
			while(iih + 1 != h && !less(*ii, *(iih + 1))) // Find middle range of elements equal to pivot
				++iih;
			if(pass > 5 || min(ii - l, h - iih) > (max(ii - l, h - iih) >> pass)) { // partition sizes ok or we have done max attempts
				if(ii - l < h - iih - 1) {       // recurse on smaller partition, tail on larger
					Sort__(l, ii, less);
					l = iih + 1;
				}
				else {
					Sort__(iih + 1, h, less);
					h = ii;
				}
				break;
			}
			IterSwap(l, l + (int)Random(count));     // try some other random elements for median pivot
			IterSwap(middle, l + (int)Random(count));
			IterSwap(h - 1, l + (int)Random(count));
			pass++;
		}
	}
}

template <class Range, class Less>
void Sort(Range&& c, const Less& less)
{
	Sort__(c.begin(), c.end(), less);
}

template <class Range>
void Sort(Range&& c)
{
	Sort__(c.begin(), c.end(), std::less<ValueTypeOf<Range>>());
}

template <class T>
struct StableSortItem__ {
	const T& value;
	int      index;

	StableSortItem__(const T& value, int index) : value(value), index(index) {}
};

template <class II, class T>
struct StableSortIterator__ {
	II          ii;
	int        *vi;

	typedef StableSortIterator__<II, T> Iter;

	Iter&       operator ++ ()               { ++ii; ++vi; return *this; }
	Iter&       operator -- ()               { --ii; --vi; return *this; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi + i); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi - i); }
	int         operator -  (Iter b) const   { return (int)(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii <  b.ii; }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*ii, *vi); }

	friend void IterSwap(Iter a, Iter b)     { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	StableSortIterator__(II ii, int *vi) : ii(ii), vi(vi) {}
};

template <class T, class Less>
struct StableSortLess__ {
	const Less& less;

	bool operator()(const StableSortItem__<T>& a, const StableSortItem__<T>& b) const {
		if(less(a.value, b.value)) return true;
		return less(b.value, a.value) ? false : a.index < b.index;
	}

	StableSortLess__(const Less& less) : less(less) {}
};

template <class Range, class Less>
void StableSort(Range&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef ValueTypeOf<Range> VT;
	typedef decltype(begin) I;
	int count = (int)(uintptr_t)(end - begin);
	Buffer<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	Sort__(StableSortIterator__<I, VT>(begin, ~h), StableSortIterator__<I, VT>(end, ~h + count),
	       StableSortLess__<VT, Less>(less));
}

template <class Range>
void StableSort(Range&& r)
{
	StableSort(r, std::less<ValueTypeOf<Range>>());
}

template <class II, class VI, class K>
struct IndexSortIterator__
{
	typedef IndexSortIterator__<II, VI, K> Iter;

	IndexSortIterator__(II ii, VI vi) : ii(ii), vi(vi) {}

	Iter&       operator ++ ()               { ++ii; ++vi; return *this; }
	Iter&       operator -- ()               { --ii; --vi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi + i); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi - i); }
	int         operator -  (Iter b) const   { return (int)(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii <  b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); }

	II          ii;
	VI          vi;
};

template <class MasterRange, class Range2, class Less>
void IndexSort(MasterRange&& r, Range2&& r2, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ValueTypeOf<MasterRange> VT;
	if(r.GetCount() == 0)
		return;
	Sort__(IndexSortIterator__<I, I2, VT>(r.begin(), r2.begin()),
	       IndexSortIterator__<I, I2, VT>(r.end(), r2.end()),
		   less);
}

template <class MasterRange, class Range2>
void IndexSort(MasterRange&& r, Range2&& r2)
{
	IndexSort(r, r2, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Less>
void StableIndexSort(MasterRange&& r, Range2&& r2, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ValueTypeOf<MasterRange> VT;
	if(r.GetCount() == 0)
		return;
	StableSort(SubRange(IndexSortIterator__<I, I2, VT>(r.begin(), r2.begin()),
		                IndexSortIterator__<I, I2, VT>(r.end(), r2.end())).Write(),
	           less);
}

template <class MasterRange, class Range2>
void StableIndexSort(MasterRange&& r, Range2&& r2)
{
	StableIndexSort(r, r2, std::less<ValueTypeOf<MasterRange>>());
}

template <class II, class VI, class WI, class K>
struct IndexSort2Iterator__
{
	typedef IndexSort2Iterator__<II, VI, WI, K> Iter;

	IndexSort2Iterator__(II ii, VI vi, WI wi) : ii(ii), vi(vi), wi(wi) {}

	Iter&       operator ++ ()               { ++ii; ++vi; ++wi; return *this; }
	Iter&       operator -- ()               { --ii; --vi; --wi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi + i, wi + i); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi - i, wi - i); }
	int         operator -  (Iter b) const   { return (int)(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii <  b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); }

	II          ii;
	VI          vi;
	WI          wi;
};

template <class MasterRange, class Range2, class Range3, class Less>
void IndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ValueTypeOf<MasterRange> VT;
	Sort__(IndexSort2Iterator__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	       IndexSort2Iterator__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		   less);
}

template <class MasterRange, class Range2, class Range3>
void IndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3)
{
	IndexSort2(r, r2, r3, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Less>
void StableIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ValueTypeOf<MasterRange> VT;
	StableSort(SubRange(IndexSort2Iterator__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    IndexSort2Iterator__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).Write(),
		       less);
}

template <class MasterRange, class Range2, class Range3>
void StableIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3)
{
	StableIndexSort2(r, r2, r3, std::less<ValueTypeOf<MasterRange>>());
}

template <class II, class VI, class WI, class XI, class K>
struct IndexSort3Iterator__
{
	typedef IndexSort3Iterator__<II, VI, WI, XI, K> Iter;

	IndexSort3Iterator__(II ii, VI vi, WI wi, XI xi) : ii(ii), vi(vi), wi(wi), xi(xi) {}

	Iter&       operator ++ ()               { ++ii; ++vi; ++wi; ++xi; return *this; }
	Iter&       operator -- ()               { --ii; --vi; --wi; --xi; return *this; }
	const K&    operator *  () const         { return *ii; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi + i, wi + i, xi + i); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi - i, wi - i, xi - i); }
	int         operator -  (Iter b) const   { return (int)(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii <  b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); IterSwap(a.vi, b.vi); IterSwap(a.wi, b.wi); IterSwap(a.xi, b.xi); }

	II          ii;
	VI          vi;
	WI          wi;
	XI          xi;
};

template <class MasterRange, class Range2, class Range3, class Range4, class Less>
void IndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	ASSERT(r.GetCount() == r4.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ValueTypeOf<MasterRange> VT;
	Sort__(IndexSort3Iterator__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	       IndexSort3Iterator__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		   less);
}

template <class MasterRange, class Range2, class Range3, class Range4>
void IndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4)
{
	IndexSort3(r, r2, r3, r4, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Range4, class Less>
void StableIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	ASSERT(r.GetCount() == r4.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef decltype(r4.begin()) I4;
	typedef ValueTypeOf<MasterRange> VT;
	StableSort(SubRange(IndexSort3Iterator__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                    IndexSort3Iterator__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).Write(),
		       less);
}

template <class MasterRange, class Range2, class Range3, class Range4>
void StableIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4)
{
	StableIndexSort3(r, r2, r3, r4, std::less<ValueTypeOf<MasterRange>>());
}

template <class I, class V>
struct SortOrderIterator__ : PostfixOps< SortOrderIterator__<I, V> >
{
	typedef SortOrderIterator__<I, V> Iter;

	SortOrderIterator__(int *ii, I vi) : ii(ii), vi(vi) {}

	Iter&       operator ++ ()               { ++ii; return *this; }
	Iter&       operator -- ()               { --ii; return *this; }
	const V&    operator *  () const         { return *(vi + *ii); }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi); }
	int         operator -  (Iter b) const   { return int(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii < b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); }

	int        *ii;
	I           vi;
};

template <class Range, class Less>
Vector<int> GetSortOrder(const Range& r, const Less& less)
{
	auto begin = r.begin();
	Vector<int> index;
	index.SetCount(r.GetCount());
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	typedef SortOrderIterator__<decltype(begin), ValueTypeOf<Range>> It;
	Sort__(It(index.begin(), begin), It(index.end(), begin), less);
	return index;
}

template <class Range>
Vector<int> GetSortOrder(const Range& r)
{
	return GetSortOrder(r, std::less<ValueTypeOf<Range>>());
}

template <class I, class T>
struct StableSortOrderIterator__ : PostfixOps< StableSortOrderIterator__<I, T> >
{
	typedef StableSortOrderIterator__<I, T> Iter;

	StableSortOrderIterator__(int *ii, I vi) : ii(ii), vi(vi) {}

	Iter&       operator ++ ()               { ++ii; return *this; }
	Iter&       operator -- ()               { --ii; return *this; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi); }
	int         operator -  (Iter b) const   { return int(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii < b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); }

	StableSortItem__<T> operator*() const    { return StableSortItem__<T>(*(vi + *ii), *ii); }

	int        *ii;
	I           vi;
};

template <class Range, class Less>
Vector<int> GetStableSortOrder(const Range& r, const Less& less)
{
	Vector<int> index;
	index.SetCount(r.GetCount());
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	auto begin = r.begin();
	typedef ValueTypeOf<Range> VT;
	typedef StableSortOrderIterator__<decltype(begin), VT> It;
	Sort__(It(index.begin(), begin), It(index.end(), begin), StableSortLess__<VT, Less>(less));
	return index;
}

template <class Range>
Vector<int> GetStableSortOrder(const Range& r)
{
	return GetStableSortOrder(r, std::less<ValueTypeOf<Range>>());
}

template <class Map, class Less>
void SortByKey(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	IndexSort(k, v, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void SortByKey(Map& map)
{
	SortByKey(map, std::less<typename Map::KeyType>());
}

template <class Map, class Less>
void SortByValue(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	IndexSort(v, k, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void SortByValue(Map& map)
{
	SortByValue(map, std::less<ValueTypeOf<Map>>());
}

template <class Map, class Less>
void StableSortByKey(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	StableIndexSort(k, v, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void StableSortByKey(Map& map)
{
	StableSortByKey(map, std::less<typename Map::KeyType>());
}

template <class Map, class Less>
void StableSortByValue(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	StableIndexSort(v, k, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void StableSortByValue(Map& map)
{
	StableSortByValue(map, std::less<ValueTypeOf<Map>>());
}

template <class Index, class Less>
void SortIndex(Index& index, const Less& less)
{
	typename Index::ValueContainer k = index.PickKeys();
	Sort(k, less);
	index = Index(pick(k));
}

template <class Index>
void SortIndex(Index& index)
{
	SortIndex(index, std::less<ValueTypeOf<Index>>());
}

template <class Index, class Less>
void StableSortIndex(Index& index, const Less& less)
{
	typename Index::ValueContainer k = index.PickKeys();
	StableSort(k, less);
	index = Index(pick(k));
}

template <class Index>
void StableSortIndex(Index& index)
{
	StableSortIndex(index, std::less<ValueTypeOf<Index>>());
}
