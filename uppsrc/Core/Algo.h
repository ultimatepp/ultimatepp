template <class T>
inline int sgn(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T tabs(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline int cmp(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

template <class Range>
void Reverse(Range& r)
{
	auto start = r.begin();
	auto end = r.end();
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
}

template <class Range>
void Reverse(Range&& r)
{
	Reverse(r);
}

template <class Range, class Accumulator>
void Accumulate(const Range& r, Accumulator& a)
{
	for(const auto& e : r)
		a(e);
}

template <class Range>
ValueTypeOf<Range> Sum(const Range& r, const ValueTypeOf<Range>& zero)
{
	ValueTypeOf<Range> sum = zero;
	for(const auto& e : r)
		sum += e;
	return sum;
}

template <class T>
ValueTypeOf<T> Sum(const T& c)
{
	return Sum(c, (ValueTypeOf<T>)0);
}

template <class Range, class Pred>
int FindBest(const Range& r, const Pred& pred)
{
	int count = r.GetCount();
	if(count == 0)
		return -1;
	auto *m = &r[0];
	int mi = 0;
	for(int i = 1; i < count; i++)
		if(pred(r[i], *m)) {
			m = &r[i];
			mi = i;
		}
	return mi;
}

template <class Range>
int FindMin(const Range& r)
{
	return FindBest(r, std::less<ValueTypeOf<Range>>());
}

template <class Range>
const ValueTypeOf<Range>& Min(const Range& r)
{
	return r[FindMin(r)];
}

template <class Range>
const ValueTypeOf<Range>& Min(const Range& r, const ValueTypeOf<Range>& def)
{
	int q = FindMin(r);
	return q < 0 ? def : r[q];
}


template <class Range>
int FindMax(const Range& r)
{
	return FindBest(r, std::greater<ValueTypeOf<Range>>());
}

template <class Range>
const ValueTypeOf<Range>& Max(const Range& r)
{
	return r[FindMax(r)];
}

template <class Range>
const ValueTypeOf<Range>& Max(const Range& r, const ValueTypeOf<Range>& def)
{
	int q = FindMax(r);
	return q < 0 ? def : r[q];
}

template <class Range1, class Range2>
bool IsEqualRange(const Range1& a, const Range2& b)
{
	if(a.GetCount() != b.GetCount())
		return false;
	for(int i = 0; i < a.GetCount(); i++)
		if(!(a[i] == b[i]))
			return false;
	return true;
}

template <class Range1, class Range2>
int CompareRanges(const Range1& a, const Range2& b)
{
	int n = min(a.GetCount(), b.GetCount());
	for(int i = 0; i < n; i++) {
		int q = SgnCompare(a[i], b[i]);
		if(q)
			return q;
	}
	return SgnCompare(a.GetCount(), b.GetCount());
}

template <class T, class V, class C>
int FindMatch(const T& cont, const C& match, int from = 0)
{
	for(int i = from; i < cont.GetCount(); i++)
		if(match(cont[i])) return i;
	return -1;
}

template <class T, class V>
int FindIndex(const T& cont, const V& value, int from = 0)
{
	for(int i = from; i < cont.GetCount(); i++)
		if(cont[i] == value) return i;
	return -1;
}

template <class Range, class T, class Less>
int FindLowerBound(const Range& v, const T& val, const Less& less = std::less<T>())
{
	int pos = 0;
	int count = v.GetCount();
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

template <class Range, class T>
int FindLowerBound(const Range& v, const T& val)
{
	return FindLowerBound(v, val, std::less<T>());
}

template <class Range, class T, class L>
int FindUpperBound(const Range& v, const T& val, const L& less = std::less<T>())
{
	int pos = 0;
	int count = v.GetCount();
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

template <class Range, class T>
int FindUpperBound(const Range& v, const T& val)
{
	return FindUpperBound(v, val, std::less<T>());
}

template <class C, class T, class L>
int FindBinary(const C& v, const T& val, const L& less)
{
	int i = FindLowerBound(v, val, less);
	return i < v.GetCount() && !less(val, v[i]) ? i : -1;
}

template <class C, class T>
int FindBinary(const C& v, const T& val)
{
	return FindBinary(v, val, std::less<T>());
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
