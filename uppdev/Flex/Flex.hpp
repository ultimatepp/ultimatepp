template <class T>
void Flex<T>::Destroy(int from, int count)
{
	DestroyArray(data, data + count);
}

template <class T>
void Flex<T>::Free()
{
	if(data) {
		DestroyArray(data, data + count);
		delete[] (byte *)data;
		delete offset;
		data = NULL;
	}
}

template <class T>
void Flex<T>::Init()
{
	SHIFT = sizeof(T) < 16 ? 8 : sizeof(T) < 64 ? 7 : sizeof(T) < 256 ? 6 : 5;
	NBLK = 1 << SHIFT;
	MASK = NBLK - 1;
	blk_items = blk_count = 0;
	data = NULL;
	static int zero = 0;
	offset = &zero;
	alloc = count = 0;
}

template <class T>
void Flex<T>::Clear()
{
	Free();
	Init();
}

template <class T>
void Flex<T>::Expand(int n)
{
	unsigned ns = count + n;
	ns = ns + (ns >> 1);
	alloc = max(max(1, int(16 / sizeof(T))), (int)ns);
	T *newdata = (T *) new byte[sizeof(T) * alloc];
	int rawsz = count * sizeof(T);
	int *newoffset;
	const unsigned growk = 18;
	if(growk * blk_count > NBLK * sizeof(T)) {
		T *t = newdata;
		for(int i = 0; i < blk_count; i++) {
			int off = offset[i] & MASK;
			T *bpos = data + (i << SHIFT);
			int sz = NBLK - off;
			memcpy(t, bpos + off, sz * sizeof(T));
			t += sz;
			memcpy(t, bpos, off * sizeof(T));
			t += off;
		}
		memcpy(t, data + blk_items, (count - blk_items) * sizeof(T));
		do {
			SHIFT++;
			blk_count = (unsigned)count >> SHIFT;
		}
		while(growk * blk_count > NBLK * sizeof(T));
		NBLK = 1 << SHIFT;
		MASK = NBLK - 1;
		blk_items = blk_count << SHIFT;
		newoffset = new int[(alloc >> SHIFT) + 1];
		int bpos = 0;
		for(int i = 0; i <= blk_count; i++) {
			newoffset[i] = bpos;
			bpos += NBLK;
		}
	}
	else {
		memcpy(newdata, data, rawsz);
		newoffset = new int[(alloc >> SHIFT) + 1];
		memcpy(newoffset, offset, ((count >> SHIFT) + 1) * sizeof(int));
	}
	if(data) {
		delete[] offset;
		delete[] (byte *) data;
	}
	data = newdata;
	offset = newoffset;
}

template <class T>
T *Flex<T>::RawInsert(int i)
{
	Move(i + 1, i, count - i);
	count++;
	return &data[i];
}

template <class T>
void Flex<T>::Insert(int i, const T& x)
{
	ASSERT(i >= 0 && i <= count);
	if(count >= alloc)
		Expand(count + 1);
	if(i >= blk_items)
		DeepCopyConstruct(RawInsert(i), x);
	else {
		T *p = RawInsert(blk_items);
		int tn = i >> SHIFT;
		int bpos = blk_items - NBLK;
		int ii = blk_count - 1;
		while(ii > tn) {
			offset[ii] = ((offset[ii] - 1) & MASK) + bpos;
			T *q = &data[bpos + (offset[ii] & MASK)];
			*((Data_S_<sizeof(T)>*)p) = *((Data_S_<sizeof(T)>*)q);
			p = q;
			--ii;
			bpos -= NBLK;
		}
		int boff = offset[ii] & MASK;
		int ex = (boff - 1) & MASK;
		*((Data_S_<sizeof(T)>*)p) = *((Data_S_<sizeof(T)>*)&data[bpos + ex]);
		i = (i + boff) & MASK;
		if(i > ex) {
			Move(bpos + 1, bpos, ex);
			Move(bpos, bpos + NBLK - 1, 1);
			ex = NBLK - 1;
		}
		Move(bpos + i + 1, bpos + i, ex - i);
		DeepCopyConstruct(&data[bpos + i], x);
	}
	if(count > blk_items + NBLK) {
		blk_items += NBLK;
		blk_count++;
		offset[blk_count] = blk_items;
	}
}

template <class T>
void Flex<T>::Remove(int i, const T& x)
{
	ASSERT(i >= 0 && i <= count);
	if(i >= blk_items)
		RawRemove(i);
	else {
		int tn = i >> SHIFT;

	}
	if(count < blk_items) {
	}
}


#define DDUMP(i)   //DUMP(i)
#define DDUMPC(i)  //DUMPC(i)
#define DLOG(i)    //LOG(i)


template<class T> template <class Less>
inline int Flex<T>::LBound(const T& x, int l, int h, const Less& less) const
{
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(data[mid], x))
			l = mid + 1;
		else
			h = mid;
	}
	return l;
}

template <class T> template<class Less>
int Flex<T>::FindLowerBound0(const T& x, const Less& less) const
{
	int l = 0;
	int h = blk_count + 1;
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(data[offset[mid]], x))
			l = mid + 1;
		else
			h = mid;
	}
	if(l == blk_count + 1)
		return LBound(x, blk_items, GetCount(), less);
	if(l == 0)
		return 0;
	int bpos = (l - 1) << SHIFT;
	int off = offset[l - 1];
	if(less(data[bpos + NBLK - 1], x))
		return LBound(x, bpos, off, less) + bpos + NBLK - off;
	else
		return LBound(x, off, bpos + NBLK, less) - off + bpos;
}

template<class T> template <class Less>
int Flex<T>::UBound(const T& x, int l, int h, const Less& less) const
{
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(x, data[mid]))
			h = mid;
		else
			l = mid + 1;
	}
	return h;
}

template <class T> template<class Less>
int Flex<T>::FindUpperBound(const T& x, const Less& less) const
{
	if(GetCount() == 0)
		return 0;
	int l = 0;
	int h = blk_count + 1;
	while(l < h) {
		int mid = unsigned(l + h) >> 1;
		if(less(x, data[offset[mid]]))
			h = mid;
		else
			l = mid + 1;
	}
	if(l == blk_count + 1)
		return UBound(x, blk_items, GetCount(), less);
	if(l == 0)
		return 0;
	int bpos = (l - 1) << SHIFT;
	int off = offset[l];
	if(less(data[bpos], x))
		return bpos + NBLK - off + UBound(x, bpos, off, less);
	else
		return bpos + UBound(x, off, bpos + NBLK, less);
}
