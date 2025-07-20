template <class T>
void StreamContainerRaw(Stream& s, T& cont)
{ // optimised version for fundamental types, for Vector
	int n = cont.GetCount();
	s / n;
	if(n < 0) {
		s.LoadError();
		return;
	}
	if(s.IsLoading()) {
		cont.Clear();
		cont.Reserve(min(n, int(256*1024 / sizeof(T)))); // protect against invalid streams...
		
		while(n > 0) {
			int count = min(n, 65536);
			int q = cont.GetCount();
			cont.InsertN(q, count);
			s.SerializeRaw(cont.begin() + q, count);
			n -= count;
		}
	}
	else
		s.SerializeRaw(cont.begin(), n);
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
		cont.Reserve(min(n, int(256*1024 / sizeof(T)))); // protect against invalid streams...
		while(n--)
			s % cont.Add();
	}
	else
	{
		for(auto ptr = cont.begin(); n--; ++ptr)
			s % *ptr;
	}
}

template <>
inline void StreamContainer(Stream& s, Vector<byte>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<word>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<int16>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<dword>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<int>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<uint64>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<float>& cont)
{
	StreamContainerRaw(s, cont);
}

template <>
inline void StreamContainer(Stream& s, Vector<double>& cont)
{
	StreamContainerRaw(s, cont);
}

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
bool Vector<T>::ReAlloc(int newalloc)
{
	ASSERT(newalloc >= items);
	size_t sz0 = (size_t)newalloc * sizeof(T);
	size_t sz = sz0;
	void *newvector;
	bool  alloced = true;
	if(MemoryTryRealloc(vector, sz)) {
		newvector = vector;
		vector = NULL;
		alloced = false;
	}
	else
		newvector = newalloc ? MemoryAllocSz(sz) : NULL;
	alloc = newalloc == INT_MAX ? INT_MAX // maximum alloc reached
	        : (int)((sz - sz0) / sizeof(T) + newalloc); // adjust alloc to real memory size
	if(vector && newvector)
		Relocate((T *)newvector, vector, items);
	vector = (T *)newvector;
	return alloced;
}

template <class T>
void Vector<T>::ReAllocF(int newalloc)
{
	void *prev = vector;
	if(ReAlloc(newalloc) && prev)
		MemoryFree(prev);
}

template <class T>
bool Vector<T>::GrowSz()
{
	if(alloc == INT_MAX)
		Panic("Too many items in container!");
#ifdef _DEBUG
	return ReAlloc(max(alloc + 1, alloc >= INT_MAX / 2 ? INT_MAX : 2 * alloc));
#else
	return ReAlloc(max(alloc + 1, alloc >= int((int64)2 * INT_MAX / 3) ? INT_MAX : alloc + (alloc >> 1)));
#endif
}

template <class T>
void Vector<T>::GrowF()
{
	void *prev = vector;
	if(GrowSz())
		MemoryFree(prev);
}

template <class T>
void Vector<T>::Pick(Vector<T>&& v)
{
	vector = v.vector;
	items = v.items;
	alloc = v.alloc;
	v.Zero();
}

#ifdef DEPRECATED
template <class T>
int  Vector<T>::GetIndex(const T& item) const
{
	if(vector == NULL) return -1;
	int n = &item - vector;
	return n >= 0 && n < items ? n : -1;
}
#endif

template <class T>
void Vector<T>::Reserve(int n)
{
	if(n > alloc)
		ReAllocF(n);
}

template <class T>
void Vector<T>::Free() {
	if(vector && items >= 0)
		Destroy((T *)vector, (T *)vector + items);
	RawFree(vector);
}

template <class T>
void Vector<T>::Clear() {
	Free();
	alloc = items = 0;
	vector = NULL;
}

template <class T>
void Vector<T>::__DeepCopy(const Vector& src) {
	items = alloc = src.items;
	if(src.vector) {
		vector = RawAlloc(alloc);
		DeepCopyConstruct(vector, src.vector, src.vector + items);
	}
	else
		vector = NULL;
}

template <class T>
T& Vector<T>::GrowAdd(const T& x) {
	T *prev = vector;
	bool b = GrowSz();
	T *q = new(Rdd()) T(x);
	if(b) RawFree(prev);
	return *q;
}

template <class T>
T& Vector<T>::GrowAdd(T&& x) {
	T *prev = vector;
	bool b = GrowSz();
	T *q = ::new(Rdd()) T(pick(x));
	if(b) RawFree(prev);
	return *q;
}

template <class T> inline
void Vector<T>::AddN(int n)
{
	ASSERT(n >= 0);
	if(items + n <= alloc) {
		Construct(vector + items, vector + items + n);
		items += n;
	}
	else
		SetCountR(items + n);
}

template <class T>
void Vector<T>::Trim(int n)
{
	ASSERT(n >= 0 && n <= items);
	Destroy(vector + n, vector + items);
	items = n;
}

template <class T>
void Vector<T>::SetCount(int n) {
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		Trim(n);
	else {
		if(n > alloc) ReAllocF(n);
		Construct(vector + items, vector + n);
		items = n;
	}
}

template <class T>
void Vector<T>::SetCount(int n, const T& init) {
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		Destroy(vector + n, vector + items);
	else {
		if(n > alloc) {
			T *prev = vector;
			bool b = ReAlloc(n); // because init can be in original vector
			DeepCopyConstructFill(vector + items, vector + n, init);
			if(b) RawFree(prev);
		}
		else
			DeepCopyConstructFill(vector + items, vector + n, init);
	}
	items = n;
}

template <class T>
void Vector<T>::SetCountR(int n) {
	if(n > alloc)
		ReAllocF(alloc + max(alloc, n - items));
	SetCount(n);
}

template <class T>
void Vector<T>::SetCountR(int n, const T& init) {
	ASSERT(n >= 0);
	if(n == items) return;
	if(n < items)
		Destroy(vector + n, vector + items);
	else
		if(n > alloc) {
			T *prev = vector;
			bool b = ReAlloc(alloc + max(alloc, n - items));  // because init can be in original vector
			DeepCopyConstructFill(vector + items, vector + n, init);
			if(b) RawFree(prev);
		}
		else
			DeepCopyConstructFill(vector + items, vector + n, init);
	items = n;
}

template <class T>
void Vector<T>::Remove(int q, int count) {
	ASSERT(q >= 0 && q <= items - count && count >= 0);
	if(count == 0) return;
	Destroy(vector + q, vector + q + count);
	RemoveRelocate(vector + q, vector + q + count, items - q - count);
	items -= count;
}

template <class T>
void Vector<T>::Remove(const int *sorted_list, int n)
{
	if(!n) return;
	int pos = *sorted_list;
	int npos = pos;
	for(;;) {
		ASSERT(pos < items);
		if(pos == *sorted_list) {
			Destruct(vector + pos);
			pos++;
			sorted_list++;
			if(--n == 0) break;
			ASSERT(*sorted_list >= pos);
		}
		else
			Relocate(vector + npos++, vector + pos++);
	}
	while(pos < items)
		Relocate(vector + npos++, vector + pos++);
	items = npos;
}

template <class T>
void Vector<T>::Remove(const Vector<int>& v)
{
	Remove(v, v.GetCount());
}

template <class T>
template <class Condition>
void Vector<T>::RemoveIf(Condition c)
{
	int i = 0;
	for(; i < items; i++) // run to the first element without moving
		if(c(i)) {
			Destruct(vector + i);
			break;
		}
	int ti = i++;
	for(; i < items; i++)
		if(c(i))
			Destruct(vector + i);
		else
			Relocate(vector + ti++, vector + i);
	items = ti;
}

template <class T>
void Vector<T>::RawInsert(int q, int count)
{
	ASSERT(count >= 0);
	ASSERT(q >= 0 && q <= items);
	if(!count) return;
	if(items + count > alloc) {
		T *newvector = RawAlloc(alloc = max(alloc + count, int(alloc + ((unsigned)alloc >> 1))));
		if(vector) {
			Relocate(newvector, vector, q);
			Relocate(newvector + q + count, vector + q, items - q);
			RawFree(vector);
		}
		vector = newvector;
	}
	else
		InsertRelocate(vector + q + count, vector + q, items - q);
	items += count;
}

template <class T>
void Vector<T>::InsertN(int q, int count) {
	ASSERT(count >= 0);
	ASSERT(q >= 0 && q <= items);
	RawInsert(q, count);
	Construct(vector + q, vector + q + count);
}

template <class T>
void Vector<T>::Insert(int q, const T& x, int count) {
	if(!count) return;
	ASSERT(!(&x >= vector && &x < vector + items));
	RawInsert(q, count);
	DeepCopyConstructFill(vector + q, vector + q + count, x);
}

template <class T>
T& Vector<T>::Insert(int q, T&& x)
{
	ASSERT(&x < vector || &x > vector + items);
	RawInsert(q, 1);
	::new(vector + q) T(pick(x));
	return Get(q);
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x, int offset, int count) {
	ASSERT(offset >= 0 && count >= 0 && offset + count <= x.GetCount());
	ASSERT(!vector || x.vector != vector);
	RawInsert(q, count);
	DeepCopyConstruct(vector + q, x.vector + offset, x.vector + offset + count);
}

template <class T>
void Vector<T>::Insert(int i, std::initializer_list<T> init)
{
	RawInsert(i, (int)init.size());
	T *t = vector + i;
	for(const auto& q : init)
		new(t++) T(clone(q));
}

template <class T>
template <class Range>
void Vector<T>::InsertRange(int i, const Range& r)
{
	RawInsert(i, r.GetCount());
	T *t = vector + i;
	for(const auto& q : r)
		new(t++) T(clone(q));
}

template <class T>
void Vector<T>::Insert(int q, const Vector& x) {
	if(!x.GetCount()) return;
	Insert(q, x, 0, x.GetCount());
}

template <class T>
void Vector<T>::Insert(int i, Vector<T>&& v) {
	ASSERT(!vector || v.vector != vector);
	if(v.items) {
		RawInsert(i, v.items);
		Relocate(vector + i, v.vector, v.items);
	}
	RawFree(v.vector);
	v.Zero();
}

template <class T>
void Vector<T>::InsertSplit(int i, Vector<T>& v, int from)
{
	ASSERT(!vector || v.vector != vector && from <= v.GetCount());
	int n = v.GetCount() - from;
	if(n) {
		RawInsert(i, n);
		Relocate(vector + i, v.vector + from, n);
		v.items = from;
	}
}

template <class T>
void Vector<T>::Set(int i, const T& x, int count) {
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

template <class T>
template <class Range>
void Vector<T>::SetRange(int i, const Range& r)
{
	int count = r.GetCount();
	ASSERT(i >= 0 && count >= 0);
	if(count == 0) return;
	At(i + count - 1);
	count += i;
	for(; i < count; i++)
		vector[i] = clone(r[i]);
}

#ifdef UPP
template <class T>
void Vector<T>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeContainer(xio, itemtag, *this);
}

template <class T>
void Vector<T>::Jsonize(JsonIO& jio)
{
	JsonizeArray<Vector<T>>(jio, *this);
}

template <class C>
String AsStringArray(const C& v)
{
	String r;
	r << '[';
	for(int i = 0; i < v.GetCount(); i++) {
		if(i)
			r << ", ";
		r << v[i];
	}
	r << ']';
	return r;
}

template <class T>
String Vector<T>::ToString() const
{
	return AsStringArray(*this);
}

#endif

// ------------------

template <class T>
void Array<T>::Free() {
	for(int i = 0; i < vector.GetCount(); i++)
		delete (T *) vector[i];
}

template <class T>
void Array<T>::__DeepCopy(const Array<T>& v) {
	int n = v.GetCount();
	vector.SetCount(n);
	for(int i = 0; i < n; i++)
		vector[i] = new T(clone(v[i]));
}

template <class T>
void  Array<T>::Trim(int n)
{
	ASSERT(n >= 0 && n <= GetCount());
	Del(vector.begin() + n, vector.end());
	vector.Trim(n);
}

template <class T>
void Array<T>::SetCount(int n) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	if(n < lc)
		Del(vector.begin() + n, vector.end());
	vector.SetCount(n);
	Init(vector.begin() + lc, vector.begin() + n);
}

template <class T>
void Array<T>::SetCount(int n, const T& init) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	if(n < lc)
		Del(vector.begin() + n, vector.end());
	vector.SetCount(n);
	Init(vector.begin() + lc, vector.begin() + n, init);
}

template <class T>
void Array<T>::SetCountR(int n) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	if(n < lc)
		Del(vector.begin() + n, vector.end());
	vector.SetCountR(n);
	Init(vector.begin() + lc, vector.begin() + n);
}

template <class T>
void Array<T>::SetCountR(int n, const T& init) {
	ASSERT(n >= 0);
	int  lc = vector.GetCount();
	if(n < lc)
		Del(vector.begin() + n, vector.end());
	vector.SetCountR(n);
	Init(vector.begin() + lc, vector.begin() + n, init);
}

#ifdef DEPRECATED
template <class T>
int  Array<T>::GetIndex(const T& item) const {
	for(PointerType const *ptr = vector.begin(); ptr < vector.end(); ptr++)
		if(*ptr == (PointerType)&item) return (int)(ptr - vector.begin());
	return -1;
}
#endif

template <class T>
void Array<T>::Move(int i1, int i2) {
	PointerType q = vector[i1];
	vector.Remove(i1);
	vector.Insert(i2 - (i2 > i1)) = (T*)q;
}

template <class T>
void Array<T>::Remove(int i, int count) {
	ASSERT(i + count <= GetCount() && count >= 0 && i >= 0);
	Del(vector.begin() + i, vector.begin() + i + count);
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
template <class Condition>
void Array<T>::RemoveIf(Condition c)
{
	int ti = 0;
	for(int i = 0; i < vector.GetCount(); i++)
		if(c(i))
			delete (T *)vector[i];
		else
			vector[ti++] = vector[i];
	vector.Trim(ti);
}


template <class T>
void Array<T>::Set(int i, const T& x, int count) {
	ASSERT(i >= 0 && count >= 0);
	if(i + count >= GetCount())
		SetCountR(i + count);
	for(PointerType *ptr = vector.begin() + i; ptr < vector.begin() + i + count; ptr++) {
		delete (T *) *ptr;
		*ptr = new T(x);
	}
}

template <class T>
void Array<T>::InsertN(int i, int count) {
	ASSERT(i >= 0 && count >= 0);
	vector.InsertN(i, count);
	Init(vector.begin() + i, vector.begin() + i + count);
}

template <class T>
void Array<T>::Insert(int i, const T& x, int count) {
	vector.InsertN(i, count);
	Init(vector.begin() + i, vector.begin() + i + count, x);
}

template <class T>
template <class Range>
void Array<T>::InsertRange(int i, const Range& r)
{
	vector.InsertN(i, r.GetCount());
	T **t = GetPtr(i);
	for(const auto& x : r)
		*t++ = new T(clone(x));
}

template <class T>
T& Array<T>::Insert(int i, T&& x)
{
	vector.InsertN(i, 1);
	vector[i] = new T(pick(x));
	return Get(i);
}

template <class T>
T& Array<T>::Insert(int i, T *newt) {
	vector.InsertN(i, 1);
	vector[i] = newt;
	return *newt;
}

template <class T>
void Array<T>::Insert(int i, const Array& x) {
	Insert(i, x, 0, x.GetCount());
}

template <class T>
void Array<T>::Insert(int i, const Array& x, int offset, int count) {
	vector.InsertN(i, count);
	for(int q = 0; q < count; q++)
		vector[q + i] = new T(x[q + offset]);
}

template <class T>
void Array<T>::Insert(int i, std::initializer_list<T> init)
{
	vector.InsertN(i, (int)init.size());
	for(const auto& q : init)
		vector[i++] = new T(q);
}

#ifdef UPP
template <class T>
void Array<T>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeContainer(xio, itemtag, *this);
}

template <class T>
void Array<T>::Jsonize(JsonIO& jio)
{
	JsonizeArray<Array<T>>(jio, *this);
}

template <class T>
String Array<T>::ToString() const
{
	return AsStringArray(*this);
}

#endif

// ------------------

template <class T>
void BiVector<T>::ReAlloc(int newalloc) {
	ASSERT(items <= newalloc && items >= 0);
	T *newvector = newalloc ? (T *) MemoryAlloc(newalloc * sizeof(T)) : NULL;
	if(items) {
		int end = start + items;
		if(end <= alloc)
			Relocate(newvector, vector + start, end - start);
		else {
			Relocate(newvector, vector + start, alloc - start);
			Relocate(newvector + alloc - start, vector, end - alloc);
		}
		MemoryFree(vector);
	}
	vector = newvector;
	alloc = newalloc;
	start = 0;
}

template <class T>
void BiVector<T>::DeepCopy0(const BiVector& src) {
	alloc = items = src.items;
	vector = alloc ? (T *) MemoryAlloc(alloc * sizeof(T)) : NULL;
	if(items) {
		int end = src.start + src.items;
		if(end <= src.alloc)
			DeepCopyConstruct(vector, src.vector + src.start, src.vector + end);
		else {
			DeepCopyConstruct(vector, src.vector + src.start, src.vector + src.alloc);
			DeepCopyConstruct(vector + src.alloc - src.start, src.vector,
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
		ReAlloc(max(2 * items, 4));
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
			Destroy(vector + start, vector + end);
		else {
			Destroy(vector + start, vector + alloc);
			Destroy(vector, vector + end - alloc);
		}
		MemoryFree(vector);
	}
}

template <class T>
void BiVector<T>::Serialize(Stream& s) {
	int n = items;
	s / n;
	if(n < 0)
		s.LoadError();
	if(s.IsLoading()) {
		Clear();
		while(n--)
			s % AddTail();
	}
	else
		for(int i = 0; i < items; i++)
			s % operator[](i);
}

template <class T>
String BiVector<T>::ToString() const
{
	return AsStringArray(*this);
}

template <class T>
BiVector<T>::BiVector(std::initializer_list<T> init)
{
	start = items = alloc = 0; vector = NULL;

	start = 0;
	alloc = items = (int)init.size();
	if(!alloc) {
		vector = NULL;
		return;
	}
	vector = (T *) MemoryAlloc(alloc * sizeof(T));
	T *t = vector;
	for(const auto& q : init)
		new (t++) T(q);
}

// ------------------

template <class T>
void BiArray<T>::Free() {
	for(int i = 0; i < GetCount(); i++)
		delete (T *) bv[i];
}

template <class T>
void BiArray<T>::DeepCopy0(const BiArray<T>& v) {
	int n = v.GetCount();
	bv.Clear();
	bv.Reserve(v.GetCount());
	for(int i = 0; i < n; i++)
		bv.AddTail() = new T(clone(v[i]));
}

template <class T>
void BiArray<T>::Serialize(Stream& s) {
	int n = bv.GetCount();
	s / n;
	if(n < 0)
		s.LoadError();
	if(s.IsLoading()) {
		Clear();
		while(n--)
			s % AddTail();
	}
	else
		for(int i = 0; i < bv.GetCount(); i++)
			s % operator[](i);
}

template <class T>
String BiArray<T>::ToString() const
{
	return AsStringArray(*this);
}

template <class T>
BiArray<T>::BiArray(std::initializer_list<T> init)
{
	for(const auto& q : init)
		AddTail(q);
}

inline
void   Bits::Set(int i, dword bits, int count)
{
	ASSERT(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return;
	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		Expand(q);
	if(i == 0 && count == 32) {
		bp[q] = bits;
		return;
	}
	dword mask = (dword)-1 >> (32 - count); // does not work for count == 0
	bits &= mask;
	bp[q] = (bp[q] & ~(mask << i)) | (bits << i);
	if(i + count <= 32)
		return;
	if(++q >= alloc)
		Expand(q);
	i = 32 - i;
	bp[q] = (bp[q] & ~(mask >> i)) | (bits >> i);
}

inline
dword Bits::Get(int i, int count)
{
	ASSERT(i >= 0 && count >= 0 && count <= 32);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return 0;
	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		return 0;
	if(i == 0 && count == 32)
		return bp[q];
	dword mask = (dword)-1 >> (32 - count); // does not work for count == 0
	dword ret = (bp[q] & (mask << i)) >> i;
	if(i + count <= 32 || ++q >= alloc)
		return ret;
	i = 32 - i;
	return ret | ((bp[q] & (mask >> i)) << i);
}

inline
void Bits::Set64(int i, uint64 bits, int count)
{
	if(count > 32) {
		Set(i, LODWORD(bits), 32);
		Set(i + 32, HIDWORD(bits), count - 32);
	}
	else
		Set(i, LODWORD(bits), count);
}

inline
uint64 Bits::Get64(int i, int count)
{
	if(count > 32) {
		dword l = Get(i, 32);
		return MAKEQWORD(l, Get(i + 32, count - 32));
	}
	else
		return Get(i, count);
}

inline
void Bits::SetN(int i, bool b, int count)
{
	ASSERT(i >= 0);
	if(!count) // note: this also avoids problem with (dword)-1 >> 32 being undefined
		return;
	
	dword bits = (dword)0 - b;

	int q = i >> 5;
	i &= 31;
	if(q >= alloc)
		Expand(q);

	int n = min(32 - i, count); // number of bits to dword boundary (or count if smaller)
	dword mask = (dword)-1 >> (32 - n);
	bp[q] = (bp[q] & ~(mask << i)) | ((bits & mask) << i);
	count -= n;
	if(!count)
		return;
	
	int dw_count = count >> 5;
	if(++q + dw_count >= alloc)
		Expand(q + dw_count);
	while(dw_count) {
		bp[q++] = bits;
		dw_count--;
	}
	
	count = count & 31; // remaining bits to set
	if(!count)
		return;
	if(q >= alloc)
		Expand(q);
	mask = (dword)-1 >> (32 - count);
	bp[q] = (bp[q] & ~mask) | (bits & mask);
}
