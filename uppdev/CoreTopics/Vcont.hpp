void VectorReAlloc_(void *vector_, int newalloc, int sizeofT);
void VectorReAllocF_(void *vector_, int newalloc, int sizeofT);
void VectorGrow_(void *vector_, int sizeofT);
void VectorGrowF_(void *vector_, int sizeofT);

template <class T>
T * Vector<T>::RawAlloc(int& n)
{
	size_t sz0 = n * sizeof(T);
	size_t sz = sz0;
	void *q = MemoryAllocSz(sz);
	n += (int)((sz - sz0) / sizeof(T));
	return (T *)q;
}

template <class T>
void Vector<T>::ReAlloc(int newalloc)
{
	VectorReAlloc_(this, newalloc, sizeof(T));
}

template <class T>
void Vector<T>::ReAllocF(int newalloc)
{
	VectorReAllocF_(this, newalloc, sizeof(T));
}

template <class T>
void Vector<T>::Grow()
{
	VectorGrow_(this, sizeof(T));
}

template <class T>
void Vector<T>::GrowF()
{
	VectorGrowF_(this, sizeof(T));
}

template <class T>
void Vector<T>::Pick(pick_ Vector<T>& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	SetPicked(v);
}

template <class T>
int  Vector<T>::GetIndex(const T& item) const
{
	Chk();
	if(vector == NULL) return -1;
	int n = &item - vector;
	return n >= 0 && n < items ? n : -1;
}

template <class T>
void Vector<T>::Reserve(int n)
{
	if(n > alloc)
		ReAllocF(n);
}

template <class T>
void Vector<T>::Free() {
	if(vector && items >= 0)
		DestroyArray((T *)vector, (T *)vector + items);
	RawFree(vector);
}

template <class T>
void Vector<T>::Clear() {
	if(vector && items >= 0)
		SetCount(0);
	else {
		alloc = items = 0;
		vector = NULL;
	}
}

template <class T>
void Vector<T>::__DeepCopy(const Vector& src) {
	src.Chk();
	items = alloc = src.items;
	if(src.vector) {
		vector = RawAlloc(alloc);
		DeepCopyConstructArray(vector, src.vector, src.vector + items);
	}
	else
		vector = NULL;
}

template <class T>
void Vector<T>::GrowAdd(const T& x) {
	T *prev = vector;
	Grow();
	DeepCopyConstruct(Rdd(), x);
	RawFree(prev);
}

template <class T>
void Vector<T>::GrowAddPick(pick_ T& x) {
	T *prev = vector;
	Grow();
	::new(Rdd()) T(x);
	RawFree(prev);
}

template <class T> inline
void Vector<T>::AddN(int n)
{
	Chk();
	ASSERT(n >= 0);
	if(items + n <= alloc) {
		ConstructArray(vector + items, vector + items + n);
		items += n;
	}
	else
		SetCountR(items + n);
}

template <class T>
void Vector<T>::Trim(int n)
{
	ASSERT(n >= 0 && n <= items);
	DestroyArray(vector + n, vector + items);
	items = n;
}

template <class T>
void Vector<T>::SetCount(int n) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		Trim(n);
	else {
		if(n > alloc) ReAllocF(n);
		ConstructArray(vector + items, vector + n);
		items = n;
	}
}

template <class T>
void Vector<T>::SetCount(int n, const T& init) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray(vector + n, vector + items);
	else {
		if(n > alloc) {
			T *prev = vector;
			ReAlloc(n);
			DeepCopyConstructFill(vector + items, vector + n, init);
			RawFree(prev);
		}
		else
			DeepCopyConstructFill(vector + items, vector + n, init);
	}
	items = n;
}

template <class T>
void Vector<T>::SetCountR(int n) {
	Chk();
	if(n + items > alloc)
		ReAllocF(alloc + ntl_max(alloc, n - items));
	SetCount(n);
}

template <class T>
void Vector<T>::SetCountR(int n, const T& init) {
	Chk();
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		DestroyArray(vector + n, vector + items);
	else
		if(n > alloc) {
			T *prev = vector;
			ReAlloc(alloc + ntl_max(alloc, n - items));
			DeepCopyConstructFill(vector + items, vector + n, init);
			RawFree(prev);
		}
		else
			DeepCopyConstructFill(vector + items, vector + n, init);
	items = n;
}

template <class T>
void Vector<T>::Remove(int q, int count) {
	Chk();
	ASSERT(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	DestroyArray(vector + q, vector + q + count);
	memmove(vector + q, vector + q + count, (items - q - count) * sizeof(T));
	items -= count;
}

template <int size>
class Data_S_ {
	byte filler[size];
};

template <class T>
void Vector<T>::Remove(const int *sorted_list, int n)
{
	Chk();
	if(!n) return;
	int pos = *sorted_list;
	int npos = pos;
	for(;;) {
		ASSERT(pos < items);
		if(pos == *sorted_list) {
			(vector + pos)->T::~T();
			pos++;
			sorted_list++;
			if(--n == 0) break;
			ASSERT(*sorted_list >= pos);
		}
		else
			*((Data_S_<sizeof(T)>*)vector + npos++)
				= *((Data_S_<sizeof(T)>*)vector + pos++);
	}
	while(pos < items)
		*((Data_S_<sizeof(T)>*)vector + npos++) = *((Data_S_<sizeof(T)>*)vector + pos++);
	items = npos;
}

template <class T>
void Vector<T>::Remove(const Vector<int>& v)
{
	Remove(v, v.GetCount());
}

template <class T>
void Vector<T>::RawInsert(int q, int count)
{
	ASSERT(count >= 0);
	ASSERT(q >= 0 && q <= items);
	if(!count) return;
	if(items + count > alloc) {
		T *newvector = RawAlloc(alloc = alloc + ntl_max(alloc, count));
		if(vector) {
			memcpy(newvector, vector, q * sizeof(T));
			memcpy(newvector + q + count, vector + q, (items - q) * sizeof(T));
			RawFree(vector);
		}
		vector = newvector;
	}
	else
		memmove(vector + q + count, vector + q, (items - q) * sizeof(T));
	items += count;
}

template <class T>
void Vector<T>::InsertN(int q, int count) {
	ASSERT(count >= 0);
	ASSERT(q >= 0 && q <= items);
	RawInsert(q, count);
	ConstructArray(vector + q, vector + q + count);
}

template <class T>
void Vector<T>::Insert(int q, const T& x, int count) {
	if(!count) return;
	ASSERT(&x < vector || &x > vector + items);
	RawInsert(q, count);
	DeepCopyConstructFill(vector + q, vector + q + count, x);
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x, int offset, int count) {
	ASSERT(offset >= 0 && count >= 0 && offset + count <= x.GetCount());
	ASSERT(!vector || x.vector != vector);
	RawInsert(q, count);
	DeepCopyConstructArray(vector + q, x.vector + offset, x.vector + offset + count);
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x) {
	if(!x.GetCount()) return;
	Insert(q, x, 0, x.GetCount());
}

template <class T>
void Vector<T>::InsertPick(int i, pick_ Vector<T>& v) {
	Chk();
	v.Chk();
	ASSERT(!vector || v.vector != vector);
	if(v.items) {
		RawInsert(i, v.items);
		memcpy(vector + i, v.vector, sizeof(T) * v.items);
	}
	RawFree(v.vector);
	SetPicked(v);
}

template <class T>
void Vector<T>::Set(int i, const T& x, int count) {
	Chk();
	ASSERT(i >= 0 && count >= 0);
	if(count == 0) return;
	if(&x >= vector && &x < vector + items) {
		T copy = x;
		At(i + count - 1);
		Fill(vector + i, vector + i + count, copy);
	}
	else {
		At(i + count - 1);
		Fill(vector + i, vector + i + count, x);
	}
}

// ------------------

template <class T>
void Array<T>::Free() {
	if(IsPicked()) return;
	for(int i = 0; i < vector.GetCount(); i++)
		delete (T *) vector[i];
}

template <class T>
void Array<T>::__DeepCopy(const Array<T>& v) {
	int n = v.GetCount();
	vector.SetCount(n);
	for(int i = 0; i < n; i++)
		vector[i] = DeepCopyNew(v[i]);
}

template <class T>
void  Array<T>::Trim(int n)
{
	ASSERT(n >= 0 && n <= GetCount());
	Del(vector.Begin() + n, vector.End());
	vector.Trim(n);
}

template <class T>
void Array<T>::SetCount(int n) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	Del(vector.Begin() + n, vector.End());
	vector.SetCount(n);
	Init(vector.Begin() + lc, vector.Begin() + n);
}

template <class T>
void Array<T>::SetCount(int n, const T& init) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	Del(vector.Begin() + n, vector.End());
	vector.SetCount(n);
	Init(vector.Begin() + lc, vector.Begin() + n, init);
}

template <class T>
void Array<T>::SetCountR(int n) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	Del(vector.Begin() + n, vector.End());
	vector.SetCountR(n);
	Init(vector.Begin() + lc, vector.Begin() + n);
}

template <class T>
void Array<T>::SetCountR(int n, const T& init) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	Del(vector.Begin() + n, vector.End());
	vector.SetCountR(n);
	Init(vector.Begin() + lc, vector.Begin() + n, init);
}

template <class T>
int  Array<T>::GetIndex(const T& item) const {
	for(void * const *ptr = vector.Begin(); ptr < vector.End(); ptr++)
		if(*ptr == (void *)&item) return (int)(ptr - vector.Begin());
	return -1;
}

template <class T>
void Array<T>::Move(int i1, int i2) {
	void *q = vector[i1];
	vector.Remove(i1);
	vector.Insert(i2 - (i2 > i1)) = q;
}

template <class T>
void Array<T>::Remove(int i, int count) {
	ASSERT(i + count <= GetCount() && count >= 0 && i >= 0);
	Del(vector.Begin() + i, vector.Begin() + i + count);
	vector.Remove(i, count);
}

template <class T>
void Array<T>::Remove(const int *sorted_list, int n)
{
	const int *q = sorted_list;
	const int *e = sorted_list + n;
	while(q < e) {
		ASSERT(*q >= 0 && *q < GetCount());
		delete (T *)vector[*q++];
	}
	vector.Remove(sorted_list, n);
}

template <class T>
void Array<T>::Remove(const Vector<int>& sorted_list)
{
	Remove(sorted_list, sorted_list.GetCount());
}

template <class T>
void Array<T>::Set(int i, const T& x, int count) {
	ASSERT(i >= 0 && count >= 0);
	if(i + count >= GetCount())
		SetCountR(i + count);
	for(void **ptr = vector.Begin() + i; ptr < vector.Begin() + i + count; ptr++) {
		delete (T *) *ptr;
		*ptr = new T(x);
	}
}

template <class T>
void Array<T>::InsertN(int i, int count) {
	ASSERT(i >= 0 && count >= 0);
	vector.InsertN(i, count);
	Init(vector.Begin() + i, vector.Begin() + i + count);
}

template <class T>
void Array<T>::Insert(int i, const T& x, int count) {
	vector.InsertN(i, count);
	Init(vector.Begin() + i, vector.Begin() + i + count, x);
}

template <class T>
void Array<T>::Insert(int i, T *newt) {
	vector.InsertN(i, 1);
	vector[i] = newt;
}

template <class T>
void Array<T>::Insert(int i, const Array& x) {
	Insert(i, x, 0, x.GetCount());
}

template <class T>
void Array<T>::Insert(int i, const Array& x, int offset, int count) {
	vector.InsertN(i, count);
	for(int q = 0; q < count; q++)
		vector[q + i] = DeepCopyNew(x[q + offset]);
}

// ------------------

template <class T, int NBLK>
Segtor<T, NBLK>::Segtor(const Segtor& s, int) {
	items = s.items;
	block.SetCount((items + NBLK - 1) / NBLK);
	int q = items / NBLK;
	int r = items % NBLK;
	int i;
	for(i = 0; i < q; i++) {
		T *a = (T *) s.block[i].item;
		DeepCopyConstructArray((T *) block[i].item, a, a + NBLK);
	}
	if(r) {
		T *a = (T *) s.block[q].item;
		DeepCopyConstructArray((T *) block[i].item, a, a + r);
	}
}

template <class T, int NBLK>
void Segtor<T, NBLK>::Free() {
	int q = items / NBLK;
	int r = items % NBLK;
	int i;
	for(i = 0; i < q; i++) {
		T *a = (T *) block[i].item;
		DestroyArray(a, a + NBLK);
	}
	if(r) {
		T *a = (T *) block[i].item;
		DestroyArray(a, a + r);
	}
}

template <class T, int NBLK>
Segtor<T, NBLK>::~Segtor() {
	if(IsPicked()) return;
	Free();
}

template <class T, int NBLK>
void Segtor<T, NBLK>::DoRange(unsigned beg, unsigned end, void (*fn)(T*, const T*)) {
	ASSERT(beg <= end);
	int bblk = beg / NBLK, bidx = beg % NBLK;
	int eblk = end / NBLK, eidx = end % NBLK;
	if(eblk == block.GetCount()) {
		ASSERT(eidx == 0);
		eblk--;
		eidx = NBLK;
	}
	ASSERT(eblk < block.GetCount());
	T *tp = (T *)block[bblk].item;
	if(bblk != eblk) {
		(*fn)(tp + bidx, tp + NBLK);
		while(++bblk < eblk) {
			tp = (T *)block[bblk].item;
			(*fn)(tp, tp + NBLK);
		}
		tp = (T *)block[bblk].item;
		(*fn)(tp, tp + eidx);
	}
	else
		(*fn)(tp + bidx, tp + eidx);
}

template <class T, int NBLK>
void Segtor<T, NBLK>::Fill(unsigned beg, unsigned end, const T& x) {
	ASSERT(beg <= end);
	int bblk = beg / NBLK, bidx = beg % NBLK;
	int eblk = end / NBLK, eidx = end % NBLK;
	if(eblk == block.GetCount()) {
		ASSERT(eidx == 0);
		eblk--;
		eidx = NBLK;
	}
	ASSERT(eblk < block.GetCount());
	T *tp = (T *)block[bblk].item;
	if(bblk != eblk) {
		DeepCopyConstructFill(tp + bidx, tp + NBLK, x);
		while(++bblk < eblk) {
			tp = (T *)block[bblk].item;
			DeepCopyConstructFill(tp, tp + NBLK, x);
		}
		tp = (T *)block[bblk].item;
		DeepCopyConstructFill(tp, tp + eidx, x);
	}
	else
		DeepCopyConstructFill(tp + bidx, tp + eidx, x);
}

template <class T, int NBLK>
void Segtor<T, NBLK>::SetCount(int n) {
	Del(n);
	block.SetCount((n + NBLK - 1) / NBLK);
	Init(n);
}

template <class T, int NBLK>
void Segtor<T, NBLK>::SetCount(int n, const T& init) {
	Del(n);
	block.SetCount((n + NBLK - 1) / NBLK);
	Init(n, init);
}

template <class T, int NBLK>
void Segtor<T, NBLK>::Clear() {
	if(!IsPicked())
		Free();
	items = 0;
	block.Clear();
}

template <class T, int NBLK>
void Segtor<T, NBLK>::Set(int i, const T& x, int count) {
	ASSERT(i >= 0 && count >= 0);
	DoIndex(i + count - 1);
	Iterator q(*this, i);
	while(count--)
		*q++ = x;
}

template <class T, int NBLK>
int Segtor<T, NBLK>::GetIndex(const T& item) const {
	for(int i = 0; i < block.GetCount(); i++) {
		T *q = (T *) block[i].item;
		if(&item >= q && &item < q + NBLK)
			return &item - q + NBLK * i;
	}
	return -1;
}

// ------------------

template <class T>
void BiVector<T>::ReAlloc(int newalloc) {
	ASSERT(items <= newalloc && items >= 0);
	T *newvector = newalloc ? (T *) new byte[newalloc * sizeof(T)] : NULL;
	if(items) {
		int end = start + items;
		if(end <= alloc)
			memcpy(newvector, vector + start, (end - start) * sizeof(T));
		else {
			memcpy(newvector, vector + start, (alloc - start) * sizeof(T));
			memcpy(newvector + alloc - start, vector, (end - alloc) * sizeof(T));
		}
		delete[] (byte *)vector;
	}
	vector = newvector;
	alloc = newalloc;
	start = 0;
}

template <class T>
void BiVector<T>::DeepCopy0(const BiVector& src) {
	alloc = items = src.items;
	vector = alloc ? (T *) new byte[alloc * sizeof(T)] : NULL;
	if(items) {
		int end = src.start + src.items;
		if(end <= src.alloc)
			DeepCopyConstructArray(vector, src.vector + src.start, src.vector + end);
		else {
			DeepCopyConstructArray(vector, src.vector + src.start, src.vector + src.alloc);
			DeepCopyConstructArray(vector + src.alloc - src.start, src.vector,
				                   src.vector + end - src.alloc);
		}
	}
	start = 0;
}

template <class T>
void BiVector<T>::Clear() {
	Free();
	start = items = alloc = 0;
	vector = NULL;
}

template <class T>
void BiVector<T>::Add0() {
	ASSERT(items >= 0);
	if(items >= alloc)
		ReAlloc(ntl_max(2 * items, 4));
	items++;
}

template <class T>
void BiVector<T>::Shrink() {
	ASSERT(items >= 0);
	if(items < alloc)
		ReAlloc(items);
}

template <class T>
void BiVector<T>::Reserve(int n) {
	ASSERT(items >= 0);
	n += items;
	if(n > alloc)
		ReAlloc(n);
}

template <class T>
void BiVector<T>::Free() {
	if(vector && items >= 0) {
		int end = start + items;
		if(end <= alloc)
			DestroyArray(vector + start, vector + end);
		else {
			DestroyArray(vector + start, vector + alloc);
			DestroyArray(vector, vector + end - alloc);
		}
		delete[] (byte *)vector;
	}
}

#ifdef UPP
template <class T>
void BiVector<T>::Serialize(Stream& s) {
	int n = items;
	s / n;
	if(s.IsLoading()) {
		Clear();
		while(n--)
			s % AddTail();
	}
	else
		for(int i = 0; i < items; i++)
			s % operator[](i);
}
#endif

// ------------------

template <class T>
void BiArray<T>::Free() {
	if(!bv.IsPicked())
		for(int i = 0; i < GetCount(); i++)
			delete (T *) bv[i];
}

template <class T>
void BiArray<T>::DeepCopy0(const BiArray<T>& v) {
	int n = v.GetCount();
	bv.Clear();
	bv.Reserve(v.GetCount());
	for(int i = 0; i < n; i++)
		bv.AddTail() = DeepCopyNew(v[i]);
}

#ifdef UPP
template <class T>
void BiArray<T>::Serialize(Stream& s) {
	int n = bv.GetCount();
	s / n;
	if(s.IsLoading()) {
		Clear();
		while(n--)
			s % AddTail();
	}
	else
		for(int i = 0; i < bv.GetCount(); i++)
			s % operator[](i);
}
#endif
