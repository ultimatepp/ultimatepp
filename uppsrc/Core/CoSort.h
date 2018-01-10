template <class I, class Less>
void CoSort__(CoWork& cw, I l, I h, const Less& less)
{
	const int PARALLEL_THRESHOLD = 80;

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
				if(ii - l < h - iih - 1) {       // schedule or recurse on smaller partition, tail on larger
					if(ii - l < PARALLEL_THRESHOLD) // too small to run in parallel?
						Sort__(l, ii, less); // resolve in this thread
					else
						cw & [=, &cw] { CoSort__(cw, l, ii, less); }; // schedule for parallel execution
					l = iih + 1;
				}
				else {
					if(h - iih - 1 < PARALLEL_THRESHOLD) // too small to run in parallel?
						Sort__(iih + 1, h, less); // resolve in this thread
					else
						cw & [=, &cw] { CoSort__(cw, iih + 1, h, less); }; // schedule for parallel execution
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

template <class I, class Less>
void CoSort__(I l, I h, const Less& less)
{
	CoWork cw;
	CoSort__(cw, l, h, less);
}

template <class Range, class Less>
void CoSort(Range&& c, const Less& less)
{
	CoSort__(c.begin(), c.end(), less);
}

template <class Range>
void CoSort(Range&& c)
{
	CoSort__(c.begin(), c.end(), std::less<ValueTypeOf<Range>>());
}

template <class Range, class Less>
void CoStableSort(Range&& r, const Less& less)
{
	auto begin = r.begin();
	auto end = r.end();
	typedef ValueTypeOf<Range> VT;
	typedef decltype(begin) I;
	int count = (int)(uintptr_t)(end - begin);
	Buffer<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	CoSort__(StableSortIterator__<I, VT>(begin, ~h), StableSortIterator__<I, VT>(end, ~h + count),
	         StableSortLess__<VT, Less>(less));
}

template <class Range>
void CoStableSort(Range&& r)
{
	CoStableSort(r, std::less<ValueTypeOf<Range>>());
}

template <class MasterRange, class Range2, class Less>
void CoIndexSort(MasterRange&& r, Range2&& r2, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ValueTypeOf<MasterRange> VT;
	if(r.GetCount() == 0)
		return;
	CoSort__(IndexSortIterator__<I, I2, VT>(r.begin(), r2.begin()),
	         IndexSortIterator__<I, I2, VT>(r.end(), r2.end()),
		     less);
}

template <class MasterRange, class Range2>
void CoIndexSort(MasterRange&& r, Range2&& r2)
{
	CoIndexSort(r, r2, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Less>
void CoStableIndexSort(MasterRange&& r, Range2&& r2, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef ValueTypeOf<MasterRange> VT;
	if(r.GetCount() == 0)
		return;
	CoStableSort(SubRange(IndexSortIterator__<I, I2, VT>(r.begin(), r2.begin()),
		                  IndexSortIterator__<I, I2, VT>(r.end(), r2.end())).Write(),
	             less);
}

template <class MasterRange, class Range2>
void CoStableIndexSort(MasterRange&& r, Range2&& r2)
{
	CoStableIndexSort(r, r2, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Less>
void CoIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ValueTypeOf<MasterRange> VT;
	CoSort__(IndexSort2Iterator__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	         IndexSort2Iterator__<I, I2, I3, VT>(r.end(), r2.end(), r3.end()),
		     less);
}

template <class MasterRange, class Range2, class Range3>
void CoIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3)
{
	CoIndexSort2(r, r2, r3, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Less>
void CoStableIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3, const Less& less)
{
	ASSERT(r.GetCount() == r2.GetCount());
	ASSERT(r.GetCount() == r3.GetCount());
	if(r.GetCount() == 0)
		return;
	typedef decltype(r.begin()) I;
	typedef decltype(r2.begin()) I2;
	typedef decltype(r3.begin()) I3;
	typedef ValueTypeOf<MasterRange> VT;
	CoStableSort(SubRange(IndexSort2Iterator__<I, I2, I3, VT>(r.begin(), r2.begin(), r3.begin()),
	                    IndexSort2Iterator__<I, I2, I3, VT>(r.end(), r2.end(), r3.end())).Write(),
		         less);
}

template <class MasterRange, class Range2, class Range3>
void CoStableIndexSort2(MasterRange&& r, Range2&& r2, Range3&& r3)
{
	CoStableIndexSort2(r, r2, r3, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Range4, class Less>
void CoIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4, const Less& less)
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
	CoSort__(IndexSort3Iterator__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	         IndexSort3Iterator__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end()),
		     less);
}

template <class MasterRange, class Range2, class Range3, class Range4>
void CoIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4)
{
	CoIndexSort3(r, r2, r3, r4, std::less<ValueTypeOf<MasterRange>>());
}

template <class MasterRange, class Range2, class Range3, class Range4, class Less>
void CoStableIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4, const Less& less)
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
	CoStableSort(SubRange(IndexSort3Iterator__<I, I2, I3, I4, VT>(r.begin(), r2.begin(), r3.begin(), r4.begin()),
	                      IndexSort3Iterator__<I, I2, I3, I4, VT>(r.end(), r2.end(), r3.end(), r4.end())).Write(),
		         less);
}

template <class MasterRange, class Range2, class Range3, class Range4>
void CoStableIndexSort3(MasterRange&& r, Range2&& r2, Range3&& r3, Range4&& r4)
{
	CoStableIndexSort3(r, r2, r3, r4, std::less<ValueTypeOf<MasterRange>>());
}

template <class Range, class Less>
Vector<int> CoGetSortOrder(const Range& r, const Less& less)
{
	auto begin = r.begin();
	Vector<int> index;
	index.SetCount(r.GetCount());
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	typedef SortOrderIterator__<decltype(begin), ValueTypeOf<Range>> It;
	CoSort__(It(index.begin(), begin), It(index.end(), begin), less);
	return index;
}

template <class Range>
Vector<int> CoGetSortOrder(const Range& r)
{
	return CoGetSortOrder(r, std::less<ValueTypeOf<Range>>());
}

template <class Range, class Less>
Vector<int> CoGetStableSortOrder(const Range& r, const Less& less)
{
	Vector<int> index;
	index.SetCount(r.GetCount());
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	auto begin = r.begin();
	typedef ValueTypeOf<Range> VT;
	typedef StableSortOrderIterator__<decltype(begin), VT> It;
	CoSort__(It(index.begin(), begin), It(index.end(), begin), StableSortLess__<VT, Less>(less));
	return index;
}

template <class Range>
Vector<int> CoGetStableSortOrder(const Range& r)
{
	return CoGetSortOrder(r, std::less<ValueTypeOf<Range>>());
}

template <class Map, class Less>
void CoSortByKey(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	CoIndexSort(k, v, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void CoSortByKey(Map& map)
{
	CoSortByKey(map, std::less<typename Map::KeyType>());
}

template <class Map, class Less>
void CoSortByValue(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	CoIndexSort(v, k, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void CoSortByValue(Map& map)
{
	CoSortByValue(map, std::less<ValueTypeOf<Map>>());
}

template <class Map, class Less>
void CoStableSortByKey(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	CoStableIndexSort(k, v, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void CoStableSortByKey(Map& map)
{
	CoStableSortByKey(map, std::less<typename Map::KeyType>());
}

template <class Map, class Less>
void CoStableSortByValue(Map& map, const Less& less)
{
	typename Map::KeyContainer k = map.PickKeys();
	typename Map::ValueContainer v = map.PickValues();
	CoStableIndexSort(v, k, less);
	map = Map(pick(k), pick(v));
}

template <class Map>
void CoStableSortByValue(Map& map)
{
	CoStableSortByValue(map, std::less<ValueTypeOf<Map>>());
}

template <class Index, class Less>
void CoSortIndex(Index& index, const Less& less)
{
	typename Index::ValueContainer k = index.PickKeys();
	CoSort(k, less);
	index = Index(pick(k));
}

template <class Index>
void CoSortIndex(Index& index)
{
	CoSortIndex(index, std::less<ValueTypeOf<Index>>());
}

template <class Index, class Less>
void CoStableSortIndex(Index& index, const Less& less)
{
	typename Index::ValueContainer k = index.PickKeys();
	CoStableSort(k, less);
	index = Index(pick(k));
}

template <class Index>
void CoStableSortIndex(Index& index)
{
	CoStableSortIndex(index, std::less<ValueTypeOf<Index>>());
}
