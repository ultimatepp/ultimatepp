template <class C, class MC>
inline size_t CoChunk__(C count, MC max_chunk)
{
	return min(max(count / CPU_Cores() / 2, (C)1), (C)max_chunk);
}

template <class Iter, class Lambda>
void CoFor(Iter begin, Iter end, const Lambda& lambda, int max_chunk = INT_MAX)
{
	size_t chunk = CoChunk__(end - begin, max_chunk);
	CoWork co;
	while(begin < end) {
		Iter e = Iter(begin + min(chunk, size_t(end - begin)));
		co & [=] {
			lambda(begin, e);
		};
		begin = e;
	}
}

template <class Range, class Lambda>
void CoFor(Range& r, const Lambda& lambda)
{
	size_t chunk = CoChunk__(r.GetCount(), INT_MAX);
	CoWork co;
	auto begin = r.begin();
	auto end = r.end();
	while(begin < end) {
		auto e = begin + min(chunk, size_t(end - begin));
		co & [=] {
			lambda(SubRange(begin, e));
		};
		begin = e;
	}
}

template <class Range, class Lambda>
void CoFor(const Range& r, const Lambda& lambda, int max_chunk = INT_MAX)
{
	size_t chunk = CoChunk__(r.GetCount(), max_chunk);
	CoWork co;
	auto begin = r.begin();
	auto end = r.end();
	while(begin < end) {
		auto e = begin + min(chunk, size_t(end - begin));
		co & [=] {
			lambda(SubRange(begin, e));
		};
		begin = e;
	}
}

template <class Range, class Accumulator>
void CoAccumulate(Range r, Accumulator& result)
{
	typedef ConstIteratorOf<Range> I;
	CoFor(r.begin(), r.end(),
		[=, &result](I i, I e) {
			Accumulator h;
			while(i < e)
				h(*i++);
			CoWork::FinLock();
			result(h);
		}
	);
}

template <class Range>
ValueTypeOf<Range> CoSum(const Range& r, const ValueTypeOf<Range>& zero)
{
	typedef ValueTypeOf<Range> VT;
	typedef ConstIteratorOf<Range> I;
	VT sum = zero;
	CoFor(r.begin(), r.end(),
		[=, &sum](I i, I e) {
			VT h = zero;
			while(i < e)
				h += *i++;
			CoWork::FinLock();
			sum += h;
		}
	);
	return sum;
}

template <class T>
ValueTypeOf<T> CoSum(const T& c)
{
	return CoSum(c, (ValueTypeOf<T>)0);
}

template <class Range, class Better>
int CoFindBest(const Range& r, const Better& better)
{
	if(r.GetCount() <= 0)
		return -1;
	typedef ConstIteratorOf<Range> I;
	I best = r.begin();
	CoFor(r.begin() + 1, r.end(),
		[=, &best](I i, I e) {
			I b = i++;
			while(i < e) {
				if(better(*i, *b))
					b = i;
				i++;
			}
			CoWork::FinLock();
			if(better(*b, *best) || !better(*best, *b) && b < best)
				best = b;
		}
	);
	return best - r.begin();
}

template <class Range>
int CoFindMin(const Range& r)
{
	return CoFindBest(r, std::less<ValueTypeOf<Range>>());
}

template <class Range>
const ValueTypeOf<Range>& CoMin(const Range& r)
{
	return r[CoFindMin(r)];
}

template <class Range>
const ValueTypeOf<Range>& CoMin(const Range& r, const ValueTypeOf<Range>& def)
{
	int q = CoFindMin(r);
	return q < 0 ? def : r[q];
}


template <class Range>
int CoFindMax(const Range& r)
{
	return CoFindBest(r, std::greater<ValueTypeOf<Range>>());
}

template <class Range>
const ValueTypeOf<Range>& CoMax(const Range& r)
{
	return r[CoFindMax(r)];
}

template <class Range>
const ValueTypeOf<Range>& CoMax(const Range& r, const ValueTypeOf<Range>& def)
{
	int q = CoFindMax(r);
	return q < 0 ? def : r[q];
}

template <class Range, class Match>
int CoFindMatch(const Range& r, const Match& eq, int from = 0)
{
	int count = r.GetCount();
	std::atomic<int> found;
	found = count;
	CoFor(from, count,
		[=, &found](int i0, int e0) {
			auto i = r.begin() + i0;
			auto e = r.begin() + e0;
			while(i < e) {
				if(found < i)
					break;
				if(eq(*i)) {
					CoWork::FinLock();
					if(i < found)
						found = i;
					return;
				}
				i++;
			}
		}
	);
	return found < (int)count ? (int)found : -1;
}

template <class Range, class V>
int CoFindIndex(const Range& r, const V& value, int from = 0)
{
	return CoFindMatch(r, [=](const V& m) { return m == value; }, from);
}
