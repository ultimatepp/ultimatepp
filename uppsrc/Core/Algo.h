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

template <class Range>
struct RangeValueType {
	typedef typename std::remove_reference<decltype(*((Range *)0)->begin())>::type type;
};

template <class T>
typename RangeValueType<T>::type Sum(const T& c, const typename RangeValueType<T>::type& zero)
{
	typename RangeValueType<T>::type sum = zero;
	Sum(sum, c.Begin(), c.End());
	return sum;
}

template <class T>
typename RangeValueType<T>::type Sum(const T& c)
{
	return Sum(c, (typename RangeValueType<T>::type)0);
}

template <class C, class Pred>
int FindBest(const C& c, int pos, int count, const Pred& pred)
{
	if(count == 0)
		return -1;
	const typename C::ValueType *m = &c[pos];
	int mi = pos;
	for(int i = 1; i < count; i++) {
		int p = i + pos;
		if(pred(c[p], *m)) {
			m = &c[p];
			mi = p;
		}
	}
	return mi;
}

template <class C>
int FindMin(const C& c, int pos, int count)
{
	return FindBest(c, pos, count, std::less<typename C::ValueType>());
}

template <class C>
int FindMin(const C& c)
{
	return FindMin(c, 0, c.GetCount());
}

template <class C>
const typename C::ValueType& Min(const C& c)
{
	return c[FindMin(c)];
}

template <class C>
const typename C::ValueType& Min(const C& c, const typename C::ValueType& def)
{
	int q = FindMin(c);
	return q < 0 ? def : c[q];
}

template <class C>
int FindMax(const C& c, int pos, int count)
{
	return FindBest(c, pos, count, std::greater<typename C::ValueType>());
}

template <class C>
int FindMax(const C& c)
{
	return FindMax(c, 0, c.GetCount());
}

template <class C>
const typename C::ValueType& Max(const C& c)
{
	return c[FindMax(c)];
}

template <class C>
const typename C::ValueType& Max(const C& c, const typename C::ValueType& def)
{
	int q = FindMax(c);
	return q < 0 ? def : c[q];
}

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
	return IsEqual(c1, c2, std::equal<VT>());
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
	return Find(ptr, end, value, std::equal<V>());
}

template <class T, class V, class C>
int FindIndex(const T& cont, const V& value, const C& equal, int from = 0)
{
	for(int i = from; i < cont.GetCount(); i++)
		if(equal(cont[i], value)) return i;
	return -1;
}

template <class T, class V>
int FindIndex(const T& cont, const V& value, int from = 0)
{
	for(int i = from; i < cont.GetCount(); i++)
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
	return BinFindIndex(container, key, std::less<VT>());
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
	return BinFind(container, key, std::less<VT>());
}

// -------------------------------------------------------------------

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
	return begin + FindLowerBound(begin, 0, end - begin, val, std::less<T>());
}

template <class C, class T, class L>
int FindLowerBound(const C& v, const T& val, const L& less)
{
	return FindLowerBound(v, 0, v.GetCount(), val, less);
}

template <class C, class T>
int FindLowerBound(const C& v, const T& val)
{
	return FindLowerBound(v, val, std::less<typename C::ValueType>());
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
	return begin + FindUpperBound(begin, 0, (int)(end - begin), val, std::less<T>());
}

template <class C, class T, class L>
int FindUpperBound(const C& v, const T& val, const L& less)
{
	return FindUpperBound(v, 0, v.GetCount(), val, less);
}

template <class C, class T>
int FindUpperBound(const C& v, const T& val)
{
	return FindUpperBound(v, val, std::less<typename C::ValueType>());
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
	int q = FindBinary(begin, val, 0, end - begin, less);
	return q < 0 ? NULL : begin + q;
}

template <class I, class T>
I FindBinaryIter(I begin, I end, const T& val)
{
	return FindBinaryIter(begin, end, val, std::less<T>());
}

template <class C, class T, class L>
int FindBinary(const C& v, const T& val, const L& less)
{
	return FindBinary(v, val, 0, v.GetCount(), less);
}

template <class C, class T>
int FindBinary(const C& v, const T& val)
{
	return FindBinary(v, val, std::less<typename C::ValueType>());
}

template <class C, class T>
void LruAdd(C& lru, T value, int limit = 10)
{
	int q = FindIndex(lru, value);
	if(q >= 0)
		lru.Remove(q);
	lru.Insert(0, value);
	if(lru.GetCount() > limit)
		lru.SetCount(limit);
}

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
