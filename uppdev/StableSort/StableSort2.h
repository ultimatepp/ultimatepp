// This does not work!

template <class I, class Comp>
inline bool I2less(I a, I b, int *ia, int *ib, const Comp& cmp)
{
	int q = cmp(*a, *b);
	return q == 0 ? *ia < *ib : q < 0;
}


template <class I, class Comp>
void I2ForwardSort(I begin, I end, int *ibegin, int *iend, const Comp& cmp)
{
	if(begin == end)
		return;
	I limit = end;
	int *ilimit = iend;
	--limit, --ilimit;
	while(!(begin == limit))
	{
		I best = limit, next = limit, ptr = limit;
		int *ibest = limit, *inext = limit, *iptr = limit;
		for(;;) {
			if(!I2less(best, --ptr, ibest, --iptr, cmp)) {
				if(ptr == begin) {
					begin = next, ibegin = inext;
					break;
				}
			}
			else {
				do {
					if(ptr == begin) {
						IterSwap(begin, best), IterSwap(ibegin, ibest);
						++begin, ++ibegin;
						goto NEXT_ITEM;
					}
				}
				while(I2less(best, --ptr, ibest, --iptr, cmp));
				if(ptr == begin) {
					IterSwap(++begin, best), IterSwap(++ibegin, ibest);
					++begin, ++ibegin;
					break;
				}
				next = ptr, inext = iptr;
				++next, ++inext;
			}
			best = ptr, ibest = iptr;
		}
	NEXT_ITEM:
		;
	}
}

template <class I, class Comp>
void QStableSort_(I begin, I end, int *ibegin, int *iend, const Comp& cmp)
{
	int count;
	while((count = (int)(end - begin)) > __SORT_THRESHOLD) {
		int expected = count >> 1, deviation = expected - (expected >> 8);
		I b = begin, e = end, m = b + expected;
		int *ib = ibegin, *ie = iend, *im = ib + expected;
		for(int pass = 1;; pass++) {
			for(;; ++b) {
				while(I2less(m, --e, im, --ie, cmp))
					;
				while(I2less(b, m, ib, im, cmp))
					++b, ++ib;
				if(!(b < e))
					break;
				if(m == b)
					m = e, im = ie;
				else
				if(m == e)
					m = b, im = ib;
				IterSwap(b, e);
				IterSwap(ib, ie);
			}
			if(pass >= __SORT_MEDIAN_PASSES)
				break;
			int pos = (int)(b - begin);
			if(pos <= expected - deviation) {
				e = end;
				ie = iend;
			}
			else if(pos >= expected + deviation) {
				e = b;
				b = begin;
				ie = ib;
				ib = ibegin;
			}
			else
				break;
			pos = 1 + (int)((unsigned)rand() % (e - b - 2));
			m = b + pos;
			im = ib + pos;
		}
		if(b - begin < end - e) {
			QStableSort_(begin, b, ibegin, ib, cmp);
			begin = b;
			ibegin = ib;
		}
		else {
			QStableSort_(b, end, ibegin, iend, cmp);
			end = b;
			iend = ib;
		}
	}
	if(count >= 2)
		I2ForwardSort(begin, end, ibegin, iend, cmp);
}

template <class T>
struct StdCompare {
	int operator()(const T& a, const T& b) const { return SgnCompare(a, b); }
};

template <class C>
void QStableSort(C& c)
{
	Buffer<int> temp(c.GetCount());
	for(int i = 0; i < c.GetCount(); i++)
		temp[i] = i;
	QStableSort_(c.Begin(), c.End(), ~temp, ~temp + c.GetCount(), StdCompare<C::ValueType>());
}
