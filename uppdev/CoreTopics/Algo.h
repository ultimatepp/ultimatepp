template <class T>
inline int sgn(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T tabs(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline int cmp(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

template <class I>
void Reverse(I start, I end)
{
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
}

template <class C>
void Reverse(C& container)
{
	Reverse(container.Begin(), container.End());
}

template <class T, class V>
void Sum(V& sum, T ptr, T end)

{
	while(ptr != end)
		sum += *ptr++;
}

template <class T>
typename T::ValueType Sum(const T& c, const typename T::ValueType& init = typename T::ValueType())
{
	typename T::ValueType sum = init;
	Sum(sum, c.Begin(), c.End());
	return sum;
}

template <class T>
typename T::ValueType Sum0(const T& c)
{
	typename T::ValueType sum = 0;
	Sum(sum, c.Begin(), c.End());
	return sum;
}

template <class T>
T MinElement(T ptr, T end)
{
	ASSERT(ptr != end);
	T min = ptr;
	while(++ptr != end)
		if(*ptr < *min) min = ptr;
	return min;
}

template <class C>
int MinIndex(const C& c)
{
	if(c.GetCount() == 0)
		return -1;
	typename C::ValueType m = c[0];
	int mi = 0;
	for(int i = 1; i < c.GetCount(); i++)
		if(c[i] < m) {
			m = c[i];
			mi = i;
		}
	return mi;
}

template <class C>
int MaxIndex(const C& c)
{
	if(c.GetCount() == 0)
		return -1;
	typename C::ValueType m = c[0];
	int mi = 0;
	for(int i = 1; i < c.GetCount(); i++)
		if(c[i] > m) {
			m = c[i];
			mi = i;
		}
	return mi;
}

template <class T>
const typename T::ValueType& Min(const T& c)
{
	return *MinElement(c.Begin(), c.End());
}

template <class T>
T MaxElement(T ptr, T end)
{
	ASSERT(ptr != end);
	T max = ptr;
	while(++ptr != end)
		if(*max < *ptr) max = ptr;
	return max;
}

template <class T>
const typename T::ValueType& Max(const T& c)
{
	return *MaxElement(c.Begin(), c.End());
}

template<class T>
struct StdEqual
{
	bool operator () (const T& a, const T& b) const { return a == b; }
};

template<class T>
struct StdLess {
	bool operator () (const T& a, const T& b) const { return a < b; }
};

template<class T>
struct StdGreater
{
	bool operator () (const T& a, const T& b) const { return a > b; }
};

/*
template <class T, class C>
bool Compare(T ptr1, T end1, T ptr2, T end2, const C& compare)
{
	for(; ptr1 != end1 && ptr2 != end2; ++ptr1, ++ptr2)
		if(!compare(*ptr1, *ptr2)) return false;
	return ptr1 == end1 && ptr2 == end2;
}

template <class T, class C>
bool Compare(const T& c1, const T& c2, const C& compare)
{
	return Compare(c1.Begin(), c1.End(), c2.Begin(), c2.End(), compare);
}

template <class T>
bool Compare(const T& c1, const T& c2)
{
	typedef typename T::ValueType VT;
	return Compare(c1, c2, StdEqual<VT>());
}
*/

template <class T, class C>
bool IsEqual(T ptr1, T end1, T ptr2, T end2, const C& equal)
{
	for(; ptr1 != end1 && ptr2 != end2; ++ptr1, ++ptr2)
		if(!equal(*ptr1, *ptr2)) return false;
	return ptr1 == end1 && ptr2 == end2;
}

template <class T, class C>
bool IsEqual(const T& c1, const T& c2, const C& equal)
{
	return IsEqual(c1.Begin(), c1.End(), c2.Begin(), c2.End(), equal);
}

template <class T>
bool IsEqual(const T& c1, const T& c2)
{
	typedef typename T::ValueType VT;
	return IsEqual(c1, c2, StdEqual<VT>());
}

template <class T, class V, class C>
T Find(T ptr, T end, const V& value, const C& equal)
{
	while(ptr != end) {
		if(equal(*ptr, value)) return ptr;
		ptr++;
	}
	return NULL;
}

template <class T, class V>
T Find(T ptr, T end, const V& value)
{
	return Find(ptr, end, value, StdEqual<T>());
}

template <class T, class V, class C>
int FindIndex(const T& cont, const V& value, const C& equal)
{
	for(int i = 0; i < cont.GetCount(); i++)
		if(equal(cont[i], value)) return i;
	return -1;
}

template <class T, class V>
int FindIndex(const T& cont, const V& value)
{
	for(int i = 0; i < cont.GetCount(); i++)
		if(cont[i] == value) return i;
	return -1;
}

template <class I, class K, class L>
int BinFindIndex(I begin, I end, const K& key, const L& less)
{
	if(begin == end)
		return 0;
	int min = 0;
	int max = end - begin;

	while(min < max)
	{
		int mid = (max + min) >> 1;
		if(less(*(begin + mid), key))
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class L>
inline int BinFindIndex(const C& container, const K& key, const L& less)
{
	return BinFindIndex(container.Begin(), container.End(), key, less);
}

template <class C, class K>
inline int BinFindIndex(const C& container, const K& key)
{
	typedef typename C::ValueType VT;
	return BinFindIndex(container, key, StdLess<VT>());
}

template <class I, class K, class L>
inline I BinFind(I begin, I end, const K& key, const L& less)
{
	return begin + BinFindIndex(begin, end, key, less);
}

template <class C, class K, class L>
inline typename C::ConstIterator BinFind(const C& container, const K& key, const L& less)
{
	return BinFind(container.Begin(), container.End(), key, less);
}

template <class C, class K>
inline typename C::ConstIterator BinFind(const C& container, const K& key)
{
	typedef typename C::ValueType VT;
	return BinFind(container, key, StdLess<VT>());
}

// -------------------------------------------------------------------

template <class I, class C>
int IterCompare(I a, I a_end, I b, I b_end, const C& compare)
{
	for(;;) {
		if(a >= a_end)
			return b < b_end ? -1 : 0;
		if(b >= b_end)
			return a < a_end ? 1 : 0;
		int q = compare(*a++, *b++);
		if(q)
			return q;
	}
}

template <class C, class T, class L>
int FindLowerBound(const C& v, int pos, int count, const T& val, const L& less)
{
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(v[m], val)) {
			pos = m + 1;
			count = count - half - 1;
		}
		else
			count = half;
	}
	return pos;
}

template <class I, class T, class L>
I FindLowerBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + FindLowerBound(begin, 0, end - begin, val, less);
}

template <class I, class T>
I FindLowerBoundIter(I begin, I end, const T& val)
{
	return begin + FindLowerBound(begin, 0, end - begin, val, StdLess<T>());
}

template <class C, class T, class L>
int FindLowerBound(const C& v, const T& val, const L& less)
{
	return FindLowerBound(v, 0, v.GetCount(), val, less);
}

template <class C, class T>
int FindLowerBound(const C& v, const T& val)
{
	return FindLowerBound(v, val, StdLess<typename C::ValueType>());
}

template <class C, class T, class L>
int FindUpperBound(const C& v, int pos, int count, const T& val, const L& less)
{
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(val, v[m]))
			count = half;
		else {
			pos = m + 1;
			count = count - half - 1;
		}
	}
	return pos;
}

template <class I, class T, class L>
I FindUpperBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + FindUpperBound(begin, 0, end - begin, val, less);
}

template <class I, class T>
I FindUpperBoundIter(I begin, I end, const T& val)
{
	return begin + FindUpperBound(begin, 0, (int)(end - begin), val, StdLess<T>());
}

template <class C, class T, class L>
int FindUpperBound(const C& v, const T& val, const L& less)
{
	return FindUpperBound(v, 0, v.GetCount(), val, less);
}

template <class C, class T>
int FindUpperBound(const C& v, const T& val)
{
	return FindUpperBound(v, val, StdLess<typename C::ValueType>());
}

template <class C, class T, class L>
int FindBinary(const C& v, const T& val, int pos, int count, const L& less)
{
	int i = FindLowerBound(v, pos, count, val, less);
	return i < count && !less(val, v[i]) ? i : -1;
}

template <class I, class T, class L>
I FindBinaryIter(I begin, I end, const T& val, const L& less)
{
	int q = FindUpperBound(begin, begin, end, val, less);
	return q < 0 ? NULL : begin + q;
}

template <class I, class T>
I FindBinaryIter(I begin, I end, const T& val)
{
	return FindBinaryIter(begin, end, val, StdLess<T>());
}

template <class C, class T, class L>
int FindBinary(const C& v, const T& val, const L& less)
{
	return FindBinary(v, val, 0, v.GetCount(), less);
}

template <class C, class T>
int FindBinary(const C& v, const T& val)
{
	return FindBinary(v, val, StdLess<typename C::ValueType>());
}

template <class I, class K, class X>
int BinFindCompIndex(I begin, I end, const K& key, const X& comp)
{
	if(begin == end) // empty array
		return 0;
	int min = 0;
	int max = end - begin;
	while(min < max)
	{
		int mid = (max + min) >> 1;
		if(comp.Compare(key, *(begin + mid)) > 0)
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class X>
inline int BinFindCompIndex(const C& container, const K& key, const X& comp)
{
	return BinFindCompIndex(container.Begin(), container.End(), key, comp);
}

template <class I, class K, class X>
inline I BinFindComp(I begin, I end, const K& key, const X& comp)
{
	return begin + BinFindCompIndex(begin, end, key, comp);
}

template <class C, class K, class X>
inline typename C::ConstIterator BinFindComp(const C& container, const K& key, const X& comp)
{
	return BinFindComp(container.Begin(), container.End(), key, comp);
}

template <class T, class V>
void Append(T& dst, V ptr, V end)
{
	for(; ptr != end; ++ptr)
		dst.Add(*ptr);
}

template <class T, class V>
void Append(T& dst, V ptr, int n)
{
	for(; n--; ++ptr)
		dst.Add(*ptr);
}

template <class T, class V>
void Append(T& dst, const V& src)
{
	Append(dst, src.Begin(), src.End());
}

template <class C, class I>
C& FindAppend(C& dest, I begin, I end)
{
	for(; begin != end; ++begin)
		dest.FindAdd(*begin);
	return dest;
}

template <class C, class S>
inline C& FindAppend(C& dest, const S& source)
{
	return FindAppend(dest, source.Begin(), source.End());
}

template <class C, class L>
C& AppendSorted(C& dest, const C& src, const L& less)
{
	if(src.IsEmpty())
		return dest;
	if(dest.IsEmpty())
	{
		dest <<= src;
		return dest;
	}
	if(!less(*src, dest.Top()))
	{
		dest.Append(src);
		return dest;
	}
	if(!less(*dest, src.Top()))
	{
		dest.Insert(0, src);
		return dest;
	}
	int dc = dest.GetCount();
	int sc = src.GetCount();
	dest.SetCount(dc + sc);
	typename C::Iterator de = dest.End();
	typename C::ConstIterator se = src.End(), pe = dest.GetIter(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.Begin())
			{ // dest items are finished
				*--de = *se;
				while(se != src.Begin())
					*--de = *--se;
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(se == src.Begin())
				return dest; // src items are finished, dest items are in place
			--se;
		}
	}
	return dest;
}

template <class C>
C& AppendSorted(C& dest, const C& src)
{
	typedef typename C::ValueType VT;
	return AppendSorted(dest, src, StdLess<VT>());
}

template <class C, class L>
C& UnionSorted(C& dest, const C& src, const L& less)
{
	if(src.IsEmpty())
		return dest;
	if(dest.IsEmpty())
	{
		dest <<= src;
		return dest;
	}
	if(less(dest.Top(), *src))
	{
		dest.Append(src);
		return dest;
	}
	if(less(src.Top(), *dest))
	{
		dest.Insert(0, src);
		return dest;
	}
	int dc = dest.GetCount();
	int sc = src.GetCount();
	dest.SetCount(dc + sc);
	typename C::Iterator de = dest.End();
	typename C::ConstIterator se = src.End(), pe = dest.GetIter(dc);
	--se;
	--pe;
	for(;;)
	{
		if(less(*se, *pe))
		{
			*--de = *pe;
			if(pe == dest.Begin())
			{ // dest items are finished
				*--de = *se;
				while(se != src.Begin())
					*--de = *--se;
				dest.Remove(0, dest.GetIndex(*de));
				return dest;
			}
			--pe;
		}
		else
		{
			*--de = *se;
			if(!less(*pe, *se))
			{
				if(pe == dest.Begin())
				{
					while(se != src.Begin())
						*--de = *--se;
					dest.Remove(0, dest.GetIndex(*de));
					return dest;
				}
				--pe;
			}
			if(se == src.Begin())
			{
				int pi = (pe - dest.Begin()) + 1;
				dest.Remove(pi, (de - dest.Begin()) - pi);
				return dest; // src items are finished, dest items are in place
			}
			--se;
		}
	}
	return dest;
}

template <class C>
C& UnionSorted(C& dest, const C& src)
{
	typedef typename C::ValueType VT;
	return UnionSorted(dest, src, StdLess<VT>());
}

template <class C, class L>
C& RemoveSorted(C& from, const C& what, const L& less)
{
	if(from.IsEmpty() || what.IsEmpty() ||
	   less(from.Top(), *what.Begin()) || less(what.Top(), *from.Begin()))
		return from;
	typename C::ConstIterator we = what.End(), wp = BinFind(what, from[0], less);
	if(wp == we)
		return from;
	typename C::Iterator fp = from.Begin() + BinFindIndex(from, *wp), fe = from.End(), fd = fp;
	if(fp == fe)
	{
		from.Clear();
		return from;
	}
	for(;;)
	{
		while(less(*fp, *wp))
		{
			*fd = *fp;
			++fd;
			if(++fp == fe)
			{
				from.SetCount(fd - from.Begin());
				return from;
			}
		}
		if(less(*wp, *fp))
		{
			do
				if(++wp == we)
				{
					Copy(fd, fp, fe);
					fd += (fe - fp);
					from.SetCount(fd - from.Begin());
					return from;
				}
			while(less(*wp, *fp));
		}
		else
		{
			const typename C::ValueType& value = *fp;
			while(!less(value, *fp))
				if(++fp == fe)
				{
					from.SetCount(fd - from.Begin());
					return from;
				}
			do
				if(++wp == we)
				{
					Copy(fd, fp, fe);
					fd += (fe - fp);
					from.SetCount(fd - from.Begin());
					return from;
				}
			while(!less(value, *wp));
		}
	}
}

template <class C>
C& RemoveSorted(C& from, const C& what)
{
	typedef typename C::ValueType VT;
	return RemoveSorted(from, what, StdLess<VT>());
}

template <class D, class S, class L>
D& IntersectSorted(D& dest, const S& src, const L& less)
{
	if(dest.IsEmpty())
		return dest;
	if(src.IsEmpty() || less(dest.Top(), src[0]) || less(src.Top(), dest[0]))
	{ // empty source set or disjunct intervals
		dest.Clear();
		return dest;
	}
	typename S::ConstIterator ss = BinFind(src, dest[0], less), se = src.End();
	if(ss == se)
	{
		dest.Clear();
		return dest;
	}
	typename D::ConstIterator ds = BinFind(dest, src[0], less), de = dest.End();
	if(ds == de)
	{
		dest.Clear();
		return dest;
	}
	typename D::Iterator d = dest.Begin();
	int count = 0;
	for(;;)
	{
		if(less(*ss, *ds))
		{
			if(++ss == se)
				break;
		}
		else
		{
			if(!less(*ds, *ss))
			{
				*d = *ds;
				++d;
				count++;
			}
			if(++ds == de)
				break;
		}
	}
	dest.SetCount(count);
	return dest;
}

template <class D, class S>
D& IntersectSorted(D& dest, const S& src)
{
	typedef typename D::ValueType VT;
	return IntersectSorted(dest, src, StdLess<VT>());
}

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
	I limit = end;
	--limit;
	while(!(begin == limit))
	{
		for(I best = limit, next = limit, ptr = limit;; best = ptr)
			if(!less(*best, *--ptr))
			{
				if(ptr == begin)
				{
					begin = next;
					break;
				}
			}
			else
			{
				do
				{
					if(ptr == begin)
					{
						IterSwap(begin, best);
						++begin;
						goto NEXT_ITEM;
					}
				}
				while(less(*best, *--ptr));
				if(ptr == begin)
				{
					IterSwap(++begin, best);
					++begin;
					break;
				}
				next = ptr;
				++next;
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
	ForwardSort(c.Begin(), c.End(), StdLess<VT>());
}

enum
{
	__SORT_THRESHOLD = 16,
	__SORT_MEDIAN_PASSES = 2,
};

template <class I, class Less>
void Sort(I begin, I end, const Less& less)
{
	int count;
	while((count = (int)(end - begin)) > __SORT_THRESHOLD) {
		int expected = count >> 1, deviation = expected - (expected >> 8);
		I b = begin, e = end, m = b + expected;
		for(int pass = 1;; pass++) {
			for(;; ++b) {
				while(less(*m, *--e))
					;
				while(less(*b, *m))
					++b;
				if(!(b < e))
					break;
				if(m == b) m = e;
				else if(m == e) m = b;
				IterSwap(b, e);
			}
			if(pass >= __SORT_MEDIAN_PASSES)
				break;
			int pos = (int)(b - begin);
			if(pos <= expected - deviation)
				e = end;
			else if(pos >= expected + deviation) {
				e = b;
				b = begin;
			}
			else
				break;
			m = b + 1 + (int)((unsigned)rand() % (e - b - 2));
		}
		if(b - begin < end - e) {
			Sort(begin, b, less);
			begin = b;
		}
		else {
			Sort(b, end, less);
			end = b;
		}
	}
	if(count >= 2)
		ForwardSort(begin, end, less);
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
	Sort(c.Begin(), c.End(), StdLess<VT>());
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
	StableSort(c.Begin(), c.End(), StdLess<typename T::ValueType>());
}

template <class T, class Cmp>
struct StableSortLessCmp_ {
	const Cmp& cmp;
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		int q = SgnCompare(a.value, b.value);
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
	StableSort(c.Begin(), c.End(), StdCmp<typename T::ValueType>());
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
		IndexSortIterator<II, VI, K>(end, pair + (end - begin)),
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
		__IndexSort(keys.Begin(), keys.End(), values.Begin(), StdLess<KT>(), (KT *)0);
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
		__StableIndexSort(keys.Begin(), keys.End(), values.Begin(), StdLess<KT>(), (KT *)0);
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
		__IndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), StdLess<KT>(), (KT *)0);
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
		__StableIndexSort2(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), StdLess<KT>(), (KT *)0);
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
		__IndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), StdLess<KT>(), (KT *)0);
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
		__StableIndexSort3(keys.Begin(), keys.End(), values1.Begin(), values2.Begin(), values3.Begin(), StdLess<KT>(), (KT *)0);
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
	return GetSortOrder(container.Begin(), container.End(), StdLess<V>());
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
	return GetStableSortOrder(container.Begin(), container.End(), StdLess<V>());
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

template <class DC, class I, class F>
void GetFieldContainer(DC& dest, I begin, I end, F field)
{
	for(; begin != end; ++begin)
		dest.Add((*begin).*field);
}

template <class DC, class SC, class F>
void GetFieldContainer(DC& dest, const SC& src, F field)
{ GetFieldContainer<DC, typename SC::ConstIterator, F>(dest, src.Begin(), src.End(), field); }

template <class I, class F, class O, class E>
I FindField(I begin, I end, F field, const O& object, const E& equal)
{
	for(; begin != end && !equal((*begin).*field, object); ++begin)
		;
	return begin;
}

template <class I, class F, class O>
I FindField(I begin, I end, F field, const O& object)
{ return FindField(begin, end, field, object, StdEqual<O>()); }

template <class C, class F, class O, class E>
int FindFieldIndex(const C& container, F field, const O& object, const E& equal)
{
	int i = 0;
	for(typename C::ConstIterator b = container.Begin(), e = container.End(); b != e; ++b, ++i)
		if(equal((*b).*field, object))
			return i;
	return -1;
}

template <class C, class F, class O>
int FindFieldIndex(const C& container, F field, const O& object)
{ return FindFieldIndex(container, field, object, StdEqual<O>()); }

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

template <class M, class T, class R>
class MethodRelationCls {
	M        method;
	const R& relation;

public:
	MethodRelationCls(M method, const R& relation) : method(method), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const {
		return relation((t1.*method)(), (t2.*method)());
	}
};

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)(), T, R>
	MethodRelation(O (T::*method)(), const R& relation)
{ return MethodRelationCls<O (T::*)(), T, R>(method, relation); }

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)() const, T, R>
	MethodRelation(O (T::*method)() const, const R& relation)
{ return MethodRelationCls<O (T::*)() const, T, R>(method, relation); }

template <class C, class T>
void LruAdd(C& lru, T value, int limit = 10) {
	int q = FindIndex(lru, value);
	if(q >= 0)
		lru.Remove(q);
	lru.Insert(0, value);
	if(lru.GetCount() > limit)
		lru.SetCount(limit);
}
