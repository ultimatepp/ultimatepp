template <class I, class Less>
void CoSort__(CoWork& cw, I l, I h, const Less& less)
{
	int count = int(h - l);
	I middle = l + (count >> 1);        // get the middle element

	for(;;) {
		if(count < 200) { // too little elements to gain anything with parellel processing
			Sort__(l, h, less);
			return;
		}

		if(count > 1000) {
			middle = l + (count >> 1); // iterators cannot point to the same object!
			I q = l + 1 + (int)Random((count >> 1) - 2);
			I w = middle + 1 + (int)Random((count >> 1) - 2);
			OrderIter5__(l, q, middle, w, h - 1, less);
		}
		else
			OrderIter3__(l, middle, h - 1, less);

		I pivot = h - 2;
		IterSwap(pivot, middle); // move median pivot to h - 2
		I i = l;
		I j = h - 2; // l, h - 2, h - 1 already sorted above
		for(;;) { // Hoare’s partition (modified):
			while(less(*++i, *pivot));
			do
				if(!(i < j)) goto done;
			while(!less(*--j, *pivot));
			IterSwap(i, j);
		}
	done:
		IterSwap(i, h - 2);                 // put pivot back in between partitions

		I ih = i;
		while(ih + 1 != h && !less(*i, *(ih + 1))) // Find middle range of elements equal to pivot
			++ih;

		int count_l = i - l;
		if(count_l == 1) // this happens if there are many elements equal to pivot, filter them out
			for(I q = ih + 1; q != h; ++q)
				if(!less(*i, *q))
					IterSwap(++ih, q);

		int count_h = h - ih - 1;

		if(count_l < count_h) {       // recurse on smaller partition, tail on larger
			cw & [=, &cw] { CoSort__(cw, l, i, less); };
			l = ih + 1;
			count = count_h;
		}
		else {
			cw & [=, &cw] { CoSort__(cw, ih + 1, h, less); };
			h = i;
			count = count_l;
		}

		if(count > 8 && min(count_l, count_h) < (max(count_l, count_h) >> 2)) // If unbalanced,
			middle = l + 1 + Random(count - 2); // randomize the next step
		else
			middle = l + (count >> 1); // the middle is probably still the best guess otherwise
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
