#ifdef UPP
template <class T>
void StreamContainer(Stream& s, T& cont)
{
	int n = cont.GetCount();
	s / n;
	if(n < 0) {
		s.LoadError();
		return;
	}
	if(s.IsLoading())
	{
		cont.Clear();
		while(n--)
			s % cont.Add();
	}
	else
	{
		for(typename T::Iterator ptr = cont.Begin(); n--; ++ptr)
			s % *ptr;
	}
}
#endif

template <class I, class Less>
void ForwardSort(I begin, I end, const Less& less)
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
void ForwardSort(T& c, const Less& less)
{
	ForwardSort(c.Begin(), c.End(), less);
}

template <class T>
void ForwardSort(T& c)
{
	typedef typename T::ValueType VT;
	ForwardSort(c.Begin(), c.End(), std::less<VT>());
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
void Sort(I l, I h, const Less& less)
{
	for(;;) {
		int count = int(h - l);
		if(count < 2)
			return;
		if(count < 8) {                         // Final optimized SelectSort
			ForwardSort(l, h, less);
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
					Sort(l, ii, less);
					l = iih + 1;
				}
				else {
					Sort(iih + 1, h, less);
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

template <class T, class Less>
void Sort(T& c, const Less& less)
{
	Sort(c.Begin(), c.End(), less);
}

template <class T>
void Sort(T& c)
{
	typedef typename T::ValueType VT;
	Sort(c.Begin(), c.End(), std::less<VT>());
}

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

template <class T, class Less>
struct StableSortLess_ {
	const Less& less;

	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		if(less(a.value, b.value)) return true;
		return less(b.value, a.value) ? false : a.index < b.index;
	}

	StableSortLess_(const Less& less) : less(less) {}
};

template <class I, class Less, class T>
void StableSort_(I begin, I end, const Less& less, const T *)
{
	int count = (int)(uintptr_t)(end - begin);
	Buffer<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	Sort(StableSortIterator<I, T>(begin, ~h),
	     StableSortIterator<I, T>(end, ~h + count),
	     StableSortLess_<T, Less>(less));
}

template <class I, class Less>
void StableSort(I begin, I end, const Less& less)
{
	if(begin != end)
		StableSort_(begin, end, less, &*begin);
}

template <class T, class Less>
void StableSort(T& c, const Less& less)
{
	StableSort(c.Begin(), c.End(), less);
}

template <class T>
void StableSort(T& c)
{
	StableSort(c.Begin(), c.End(), std::less<typename T::ValueType>());
}

template <class T, class Cmp>
struct StableSortLessCmp_ {
	const Cmp& cmp;
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		int q = cmp(a.value, b.value);
		return q ? q < 0 : a.index < b.index;
	}

	StableSortLessCmp_(const Cmp& cmp) : cmp(cmp) {}
};

template <class I, class Cmp, class T>
void StableSortCmp_(I begin, I end, const Cmp& cmp, const T *)
{
	int count = end - begin;
	Buffer<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	Sort(StableSortIterator<I, T>(begin, ~h),
	     StableSortIterator<I, T>(end, ~h + count),
	     StableSortLessCmp_<T, Cmp>(cmp));
}

template <class I, class Cmp>
void StableSortCmp(I begin, I end, const Cmp& cmp)
{
	if(begin != end)
		StableSortCmp_(begin, end, cmp, &*begin);
}

template <class T, class Cmp>
void StableSortCmp(T& c, const Cmp& cmp)
{
	StableSortCmp(c.Begin(), c.End(), cmp);
}

template <class T>
struct StdCmp {
	int operator()(const T& a, const T& b) const {
		return SgnCompare(a, b);
	}
};

template <class T>
void StableSortCmp(T& c)
{
	StableSortCmp(c.Begin(), c.End(), StdCmp<typename T::ValueType>());
}

template <class II, class VI, class K>
struct IndexSortIterator
{
	typedef IndexSortIterator<II, VI, K> Iter;

	IndexSortIterator(II ii, VI vi) : ii(ii), vi(vi) {}

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

template <class II, class VI, class K, class Less>
inline void __IndexSort(II begin, II end, VI pair, const Less& less, const K *)
{
	Sort(IndexSortIterator<II, VI, K>(begin, pair),
		IndexSortIterator<II, VI, K>(end, pair + (int)(ptrdiff_t)(end - begin)),
		less);
}

template <class II, class VI, class Less>
inline void IndexSort(II begin, II end, VI pair, const Less& less)
{
	if(begin != end)
		__IndexSort(begin, end, pair, less, &*begin);
}

template <class KC, class VC, class Less>
inline void IndexSort(KC& keys, VC& values, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values.GetCount());
	if(keys.GetCount() >= 2)
		__IndexSort(keys.Begin(), keys.End(), values.Begin(), less, (KT *)0);
}

template <class KC, class VC>
inline void IndexSort(KC& keys, VC& values)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__IndexSort(keys.Begin(), keys.End(), values.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class K, class Less>
inline void __StableIndexSort(II begin, II end, VI pair, const Less& less, const K *)
{
	StableSort(IndexSortIterator<II, VI, K>(begin, pair),
		IndexSortIterator<II, VI, K>(end, pair + (end - begin)),
		less);
}

template <class II, class VI, class Less>
inline void StableIndexSort(II begin, II end, VI pair, const Less& less)
{
	if(begin != end)
		__StableIndexSort(begin, end, pair, less, &*begin);
}

template <class KC, class VC, class Less>
inline void StableIndexSort(KC& keys, VC& values, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSort(keys.Begin(), keys.End(), values.Begin(), less, (KT *)0);
}

template <class KC, class VC>
inline void StableIndexSort(KC& keys, VC& values)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSort(keys.Begin(), keys.End(), values.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class K, class Cmp>
inline void __StableIndexSortCmp(II begin, II end, VI pair, const Cmp& cmp, const K *)
{
	StableSortCmp(IndexSortIterator<II, VI, K>(begin, pair),
	              IndexSortIterator<II, VI, K>(end, pair + (end - begin)),
	              cmp);
}

template <class II, class VI, class Cmp>
inline void StableIndexSortCmp(II begin, II end, VI pair, const Cmp& cmp)
{
	if(begin != end)
		__StableIndexSortCmp(begin, end, pair, cmp, &*begin);
}

template <class KC, class VC, class Cmp>
inline void StableIndexSortCmp(KC& keys, VC& values, const Cmp& cmp)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSortCmp(keys.Begin(), keys.End(), values.Begin(), cmp, (KT *)0);
}

template <class KC, class VC>
inline void StableIndexSortCmp(KC& keys, VC& values)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSortCmp(keys.Begin(), keys.End(), values.Begin(), StdCmp<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class K>
struct IndexSort2Iterator
{
	typedef IndexSort2Iterator<II, VI, WI, K> Iter;

	IndexSort2Iterator(II ii, VI vi, WI wi) : ii(ii), vi(vi), wi(wi) {}

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

template <class II, class VI, class WI, class K, class Less>
inline void __IndexSort2(II begin, II end, VI pair1, WI pair2, const Less& less, const K *)
{
	int count = end - begin;
	Sort(IndexSort2Iterator<II, VI, WI, K>(begin, pair1, pair2),
	     IndexSort2Iterator<II, VI, WI, K>(end, pair1 + count, pair2 + count),
	     less);
}

template <class II, class VI, class WI, class Less>
inline void IndexSort2(II begin, II end, VI pair1, WI pair2, const Less& less)
{
	if(begin != end)
		__IndexSort2(begin, end, pair1, pair2, less, &*begin);
}

template <class KC, class VC, class WC, class Less>
inline void IndexSort2(KC& keys, VC& values1, WC& values2, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount());
	if(keys.GetCount() >= 2)
		__IndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), less, (KT *)0);
}

template <class KC, class VC, class WC>
inline void IndexSort2(KC& keys, VC& values1, WC& values2)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__IndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class K, class Less>
inline void __StableIndexSort2(II begin, II end, VI pair1, WI pair2, const Less& less, const K *)
{
	int count = end - begin;
	StableSort(IndexSort2Iterator<II, VI, WI, K>(begin, pair1, pair2),
	           IndexSort2Iterator<II, VI, WI, K>(end, pair1 + count, pair2 + count),
	           less);
}

template <class II, class VI, class WI, class Less>
inline void StableIndexSort2(II begin, II end, VI pair1, WI pair2, const Less& less)
{
	if(begin != end)
		__StableIndexSort2(begin, end, pair1, pair2, less, &*begin);
}

template <class KC, class VC, class WC, class Less>
inline void StableIndexSort2(KC& keys, VC& values1, WC& values2, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), less, (KT *)0);
}

template <class KC, class VC, class WC>
inline void StableIndexSort2(KC& keys, VC& values1, WC& values2)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class K, class Cmp>
inline void __StableIndexSort2Cmp(II begin, II end, VI pair1, WI pair2, const Cmp& cmp, const K *)
{
	int count = end - begin;
	StableSortCmp(IndexSort2Iterator<II, VI, WI, K>(begin, pair1, pair2),
	              IndexSort2Iterator<II, VI, WI, K>(end, pair1 + count, pair2 + count),
	              cmp);
}

template <class II, class VI, class WI, class Cmp>
inline void StableIndexSort2Cmp(II begin, II end, VI pair1, WI pair2, const Cmp& cmp)
{
	if(begin != end)
		__StableIndexSort2Cmp(begin, end, pair1, pair2, cmp, &*begin);
}

template <class KC, class VC, class WC, class Cmp>
inline void StableIndexSort2Cmp(KC& keys, VC& values1, WC& values2, const Cmp& cmp)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSort2Cmp(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), cmp, (KT *)0);
}

template <class KC, class VC, class WC>
inline void StableIndexSort2Cmp(KC& keys, VC& values1, WC& values2)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSort2Cmp(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), StdCmp<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class XI, class K>
struct IndexSort3Iterator
{
	typedef IndexSort3Iterator<II, VI, WI, XI, K> Iter;

	IndexSort3Iterator(II ii, VI vi, WI wi, XI xi) : ii(ii), vi(vi), wi(wi), xi(xi) {}

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

template <class II, class VI, class WI, class XI, class K, class Less>
inline void __IndexSort3(II begin, II end, VI pair1, WI pair2, XI pair3, const Less& less, const K *)
{
	int count = end - begin;
	Sort(IndexSort3Iterator<II, VI, WI, XI, K>(begin, pair1, pair2, pair3),
		IndexSort3Iterator<II, VI, WI, XI, K>(end, pair1 + count, pair2 + count, pair3 + count),
		less);
}

template <class II, class VI, class WI, class XI, class Less>
inline void IndexSort3(II begin, II end, VI pair1, WI pair2, XI pair3, const Less& less)
{
	if(begin != end)
		__IndexSort3(begin, end, pair1, pair2, pair3, less, &*begin);
}

template <class KC, class VC, class WC, class XC, class Less>
inline void IndexSort3(KC& keys, VC& values1, WC& values2, XC& values3, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount()
		&& keys.GetCount() == values3.GetCount());
	if(keys.GetCount() >= 2)
		__IndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), less, (KT *)0);
}

template <class KC, class VC, class WC, class XC>
inline void IndexSort3(KC& keys, VC& values1, WC& values2, XC& values3)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__IndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class XI, class K, class Less>
inline void __StableIndexSort3(II begin, II end, VI pair1, WI pair2, XI pair3, const Less& less, const K *)
{
	int count = end - begin;
	StableSort(IndexSort3Iterator<II, VI, WI, XI, K>(begin, pair1, pair2, pair3),
	           IndexSort3Iterator<II, VI, WI, XI, K>(end, pair1 + count, pair2 + count, pair3 + count),
	           less);
}

template <class II, class VI, class WI, class XI, class Less>
inline void StableIndexSort3(II begin, II end, VI pair1, WI pair2, XI pair3, const Less& less)
{
	if(begin != end)
		__StableIndexSort3(begin, end, pair1, pair2, pair3, less, &*begin);
}

template <class KC, class VC, class WC, class XC, class Less>
inline void StableIndexSort3(KC& keys, VC& values1, WC& values2, XC& values3, const Less& less)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount()
		&& keys.GetCount() == values3.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), less, (KT *)0);
}

template <class KC, class VC, class WC, class XC>
inline void StableIndexSort3(KC& keys, VC& values1, WC& values2, XC& values3)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), std::less<KT>(), (KT *)0);
}

template <class II, class VI, class WI, class XI, class K, class Cmp>
inline void __StableIndexSort3Cmp(II begin, II end, VI pair1, WI pair2, XI pair3, const Cmp& cmp, const K *)
{
	int count = end - begin;
	StableSortCmp(IndexSort3Iterator<II, VI, WI, XI, K>(begin, pair1, pair2, pair3),
	              IndexSort3Iterator<II, VI, WI, XI, K>(end, pair1 + count, pair2 + count, pair3 + count),
	              cmp);
}

template <class II, class VI, class WI, class XI, class Cmp>
inline void StableIndexSort3Cmp(II begin, II end, VI pair1, WI pair2, XI pair3, const Cmp& cmp)
{
	if(begin != end)
		__StableIndexSort3Cmp(begin, end, pair1, pair2, pair3, cmp, &*begin);
}

template <class KC, class VC, class WC, class XC, class Cmp>
inline void StableIndexSort3Cmp(KC& keys, VC& values1, WC& values2, XC& values3, const Cmp& cmp)
{
	typedef typename KC::ValueType KT;
	ASSERT(keys.GetCount() == values1.GetCount() && keys.GetCount() == values2.GetCount()
		&& keys.GetCount() == values3.GetCount());
	if(keys.GetCount() >= 2)
		__StableIndexSort3Cmp(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), cmp, (KT *)0);
}

template <class KC, class VC, class WC, class XC>
inline void StableIndexSort3Cmp(KC& keys, VC& values1, WC& values2, XC& values3)
{
	typedef typename KC::ValueType KT;
	if(keys.GetCount() >= 2)
		__StableIndexSort3Cmp(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), StdCmp<KT>(), (KT *)0);
}


template <class I, class V>
struct SortOrderIterator : PostfixOps< SortOrderIterator<I, V> >
{
	typedef SortOrderIterator<I, V> Iter;

	SortOrderIterator(int *ii, I vi) : ii(ii), vi(vi) {}

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

template <class I, class V, class Less>
inline void __SortOrder(int *begin, int *end, I data, const Less& less, const V *)
{
	Sort(SortOrderIterator<I, V>(begin, data), SortOrderIterator<I, V>(end, data), less);
}

template <class I, class Less>
inline Vector<int> GetSortOrder(I begin, I end, const Less& less)
{
	Vector<int> index;
	index.SetCount((int)(end - begin));
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	if(begin != end)
		__SortOrder(index.Begin(), index.End(), begin, less, &*begin);
	return index;
}

template <class C, class Less>
inline Vector<int> GetSortOrder(const C& container, const Less& less)
{
	return GetSortOrder(container.Begin(), container.End(), less);
}

template <class C>
inline Vector<int> GetSortOrder(const C& container)
{
	typedef typename C::ValueType V;
	return GetSortOrder(container.Begin(), container.End(), std::less<V>());
}

template <class I, class T>
struct StableSortOrderIterator : PostfixOps< StableSortOrderIterator<I, T> >
{
	typedef StableSortOrderIterator<I, T> Iter;

	StableSortOrderIterator(int *ii, I vi) : ii(ii), vi(vi) {}

	Iter&       operator ++ ()               { ++ii; return *this; }
	Iter&       operator -- ()               { --ii; return *this; }
	Iter        operator +  (int i) const    { return Iter(ii + i, vi); }
	Iter        operator -  (int i) const    { return Iter(ii - i, vi); }
	int         operator -  (Iter b) const   { return int(ii - b.ii); }
	bool        operator == (Iter b) const   { return ii == b.ii; }
	bool        operator != (Iter b) const   { return ii != b.ii; }
	bool        operator <  (Iter b) const   { return ii < b.ii; }
	friend void IterSwap    (Iter a, Iter b) { IterSwap(a.ii, b.ii); }

	StableSortItem<T> operator*() const      { return StableSortItem<T>(*(vi + *ii), *ii); }

	int        *ii;
	I           vi;
};

template <class I, class T, class Less>
inline void __StableSortOrder(int *ibegin, int *iend, I data, const Less& less, const T *)
{
	Sort(StableSortOrderIterator<I, T>(ibegin, data),
	     StableSortOrderIterator<I, T>(iend, data),
	     StableSortLess_<T, Less>(less));
}

template <class I, class Less>
inline Vector<int> GetStableSortOrder(I begin, I end, const Less& less)
{
	Vector<int> index;
	index.SetCount((int)(end - begin));
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	if(begin != end)
		__StableSortOrder(index.Begin(), index.End(), begin, less, &*begin);
	return index;
}

template <class C, class Less>
inline Vector<int> GetStableSortOrder(const C& container, const Less& less)
{
	return GetStableSortOrder(container.Begin(), container.End(), less);
}

template <class C>
inline Vector<int> GetStableSortOrder(const C& container)
{
	typedef typename C::ValueType V;
	return GetStableSortOrder(container.Begin(), container.End(), std::less<V>());
}

template <class I, class T, class Cmp>
inline void __StableSortOrderCmp(int *ibegin, int *iend, I data, const Cmp& cmp, const T *)
{
	Sort(StableSortOrderIterator<I, T>(ibegin, data),
	     StableSortOrderIterator<I, T>(iend, data),
	     StableSortLessCmp_<T, Cmp>(cmp));
}

template <class I, class Cmp>
inline Vector<int> GetStableSortOrderCmp(I begin, I end, const Cmp& cmp)
{
	Vector<int> index;
	index.SetCount((int)(end - begin));
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	if(begin != end)
		__StableSortOrderCmp(index.Begin(), index.End(), begin, cmp, &*begin);
	return index;
}

template <class C, class Cmp>
inline Vector<int> GetStableSortOrderCmp(const C& container, const Cmp& cmp)
{
	return GetStableSortOrderCmp(container.Begin(), container.End(), cmp);
}

template <class C>
inline Vector<int> GetStableSortOrderCmp(const C& container)
{
	typedef typename C::ValueType V;
	return GetStableSortOrderCmp(container.Begin(), container.End(), StdCmp<V>());
}

template <class O, class T, class R>
class FieldRelationCls {
	O        T::*member;
	const R& relation;

public:
	FieldRelationCls(O (T::*member), const R& relation) : member(member), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const { return relation(t1.*member, t2.*member); }
};

template <class O, class T, class R>
inline FieldRelationCls<O, T, R> FieldRelation(O (T::*member), const R& relation)
{ return FieldRelationCls<O, T, R>(member, relation); }

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
	SortByValue(map, std::less<typename Map::ValueType>());
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
	StableSortByValue(map, std::less<typename Map::ValueType>());
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
	SortIndex(index, std::less<typename Index::ValueType>());
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
	StableSortIndex(index, std::less<typename Index::ValueType>());
}
