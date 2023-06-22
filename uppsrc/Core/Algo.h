template <class T>
inline int sgn(T a) { return a > 0 ? 1 : a < 0 ? -1 : 0; }

template <class T>
inline T tabs(T a) { return (a >= 0 ? a : -a); }

// there is a general mess about abs, this is not ideal but best we can do now... as long as we
// do not want abs(1.1) == 1 surprise in GCC
template <class T> inline T abs(T a) { return (a >= 0 ? a : -a); }

template <class T>
inline int cmp(const T& a, const T& b) { return a > b ? 1 : a < b ? -1 : 0; }

template <class Range>
void Reverse(Range&& r)
{
	auto start = r.begin();
	auto end = r.end();
	if(start != end && --end != start)
		do
			IterSwap(start, end);
		while(++start != end && --end != start);
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

template <class Range, class V>
int Count(const Range& r, const V& val)
{
	int count = 0;
	for(const auto& x : r)
		if(x == val)
			count++;
	return count;
}

template <class Range, class Predicate>
int CountIf(const Range& r, const Predicate& p)
{
	int count = 0;
	for(const auto& x : r)
		if(p(x))
			count++;
	return count;
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

template <class Range1, class Range2, class Cmp>
int CompareRanges(const Range1& a, const Range2& b, Cmp cmp)
{
	int n = min(a.GetCount(), b.GetCount());
	for(int i = 0; i < n; i++) {
		int q = cmp(a[i], b[i]);
		if(q)
			return q;
	}
	return cmp(a.GetCount(), b.GetCount());
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

template <class Range, class C>
int FindMatch(const Range& r, const C& match, int from = 0)
{
	for(int i = from; i < r.GetCount(); i++)
		if(match(r[i])) return i;
	return -1;
}

template <class Range, class V>
int FindIndex(const Range& r, const V& value, int from = 0)
{
	for(int i = from; i < r.GetCount(); i++)
		if(r[i] == value) return i;
	return -1;
}

template <class Range, class Predicate>
Vector<int> FindAll(const Range& r, Predicate match, int from = 0)
{
	Vector<int> ndx;
	for(int i = from; i < r.GetCount(); i++)
		if(match(r[i]))
			ndx.Add(i);
	return ndx;
}

template <class Range, class Predicate>
Vector<int> FindAlli(const Range& r, Predicate match, int from = 0)
{
	Vector<int> ndx;
	for(int i = from; i < r.GetCount(); i++)
		if(match(i))
			ndx.Add(i);
	return ndx;
}

template <class Range, class T, class Less>
int FindLowerBound(const Range& r, const T& val, const Less& less)
{
	int pos = 0;
	int count = r.GetCount();
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(r[m], val)) {
			pos = m + 1;
			count = count - half - 1;
		}
		else
			count = half;
	}
	return pos;
}

template <class Range, class T>
int FindLowerBound(const Range& r, const T& val)
{
	return FindLowerBound(r, val, std::less<T>());
}

template <class Range, class T, class L>
int FindUpperBound(const Range& r, const T& val, const L& less)
{
	int pos = 0;
	int count = r.GetCount();
	while(count > 0) {
		int half = count >> 1;
		int m = pos + half;
		if(less(val, r[m]))
			count = half;
		else {
			pos = m + 1;
			count = count - half - 1;
		}
	}
	return pos;
}

template <class Range, class T>
int FindUpperBound(const Range& r, const T& val)
{
	return FindUpperBound(r, val, std::less<T>());
}

template <class Range, class T, class L>
int FindBinary(const Range& r, const T& val, const L& less)
{
	int i = FindLowerBound(r, val, less);
	return i < r.GetCount() && !less(val, r[i]) ? i : -1;
}

template <class Range, class T>
int FindBinary(const Range& r, const T& val)
{
	return FindBinary(r, val, std::less<T>());
}

template <class Container, class T>
void LruAdd(Container& lru, T value, int limit = 10)
{
	int q = FindIndex(lru, value);
	if(q >= 0)
		lru.Remove(q);
	lru.Insert(0, value);
	if(lru.GetCount() > limit)
		lru.SetCount(limit);
}

template <class C = Vector<int>, class V>
C MakeIota(V end, V start = 0, V step = 1)
{
	C x;
	x.Reserve((end - start) / step);
	for(V i = start; i < end; i += step)
		x.Add(i);
	return x;
}

dword Random(dword n);

template <class Range>
void RandomShuffle(Range& r)
{
	for(int i = r.GetCount() - 1; i > 0; i--)
		Swap(r[i], r[Random(i)]);
}
