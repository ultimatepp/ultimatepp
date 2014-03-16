template <class T>
class BiVector : MoveableAndDeepCopyOption< BiVector<T> > {
protected:
	T       *vector;
	int      start;
	int      items;
	int      alloc;

	int      Ix(int i) const         { return i + start < alloc ? i + start : i + start - alloc; }

	int      EI() const              { return Ix(items - 1); }
	void     ReAlloc(int newalloc);
	void     Add0();
	void     DeepCopy0(const BiVector& src);
	T       *AddHead0()              { AssertMoveable<T>(); Add0(); return &vector[start = Ix(alloc - 1)/*(start + alloc - 1) % alloc*/]; }
	T       *AddTail0()              { AssertMoveable<T>(); Add0(); return &vector[EI()]; }
	void     Free();
	void     Pick(BiVector rval_ x) { vector = pick(x.vector); start = x.start; items = x.items;
	                                  alloc = x.alloc; ((BiVector&)x).items = -1; }
	void     Copy(T *dst, int start, int count) const;

public:
	int      GetCount() const        { return items; }
	bool     IsEmpty() const         { return items == 0; }
	void     Clear();

	T&       AddHead()               { return *new(AddHead0()) T; }
	T&       AddTail()               { return *new(AddTail0()) T; }
	void     AddHead(const T& x)     { new(AddHead0()) T(x); }
	void     AddTail(const T& x)     { new(AddTail0()) T(x); }
	void     AddHeadPick(T rval_ x) { new(AddHead0()) T(x); }
	void     AddTailPick(T rval_ x) { new(AddTail0()) T(x); }
	T&       Head()                  { ASSERT(items > 0); return vector[start]; }
	T&       Tail()                  { ASSERT(items > 0); return vector[EI()]; }
	const T& Head() const            { ASSERT(items > 0); return vector[start]; }
	const T& Tail() const            { ASSERT(items > 0); return vector[EI()]; }
	void     DropHead()              { (&Head())->T::~T(); items--; start = Ix(1); }
	void     DropTail()              { (&Tail())->T::~T(); items--; }
	void     DropHead(int n)         { while(n-- > 0) BiVector<T>::DropHead(); }
	void     DropTail(int n)         { while(n-- > 0) BiVector<T>::DropTail(); }
	const T& operator[](int i) const { ASSERT(i >= 0 && i < items); return vector[Ix(i)]; }
	T&       operator[](int i)       { ASSERT(i >= 0 && i < items); return vector[Ix(i)]; }
	void     Shrink();
	void     Reserve(int n);
	int      GetAlloc() const        { return alloc; }

#ifdef UPP
	void     Serialize(Stream& s);
	String   ToString() const;
	bool     operator==(const BiVector<T>& b) const { return IsEqualArray(*this, b); }
	bool     operator!=(const BiVector<T>& b) const { return !operator==(b); }
	int      Compare(const BiVector<T>& b) const    { return CompareArray(*this, b); }
	bool     operator<=(const BiVector<T>& x) const { return Compare(x) <= 0; }
	bool     operator>=(const BiVector<T>& x) const { return Compare(x) >= 0; }
	bool     operator<(const BiVector<T>& x) const  { return Compare(x) < 0; }
	bool     operator>(const BiVector<T>& x) const  { return Compare(x) > 0; }
#endif

	bool     IsPicked() const                   { return items < 0; }

	BiVector(const BiVector& src, int)          { DeepCopy0(src); }
	BiVector(BiVector rval_ src)                { Pick(pick(src)); }
	void operator=(BiVector rval_ src)          { Free(); Pick(pick(src)); }
	BiVector()                                  { start = items = alloc = 0; vector = NULL; }
	~BiVector()                                 { Free(); } // gcc4.0 workaround!!

	typedef ConstIIterator<BiVector> ConstIterator;
	typedef IIterator<BiVector>      Iterator;

	typedef T        ValueType;

	ConstIterator    Begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    End() const                { return ConstIterator(*this, GetCount()); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         Begin()                    { return Iterator(*this, 0); }
	Iterator         End()                      { return Iterator(*this, GetCount()); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }

	friend void Swap(BiVector& a, BiVector& b)  { UPP::Swap(a.vector, b.vector);
	                                              UPP::Swap(a.start, b.start);
	                                              UPP::Swap(a.items, b.items);
	                                              UPP::Swap(a.alloc, b.alloc); }

	STL_BI_COMPATIBILITY(BiVector<T>)
};

template <class T>
class BiArray : MoveableAndDeepCopyOption< BiArray<T> > {
protected:
	BiVector<void *> bv;

	void     Free();
	void     DeepCopy0(const BiArray<T>& v);

public:
	int      GetCount() const              { return bv.GetCount(); }
	bool     IsEmpty() const               { return GetCount() == 0; }
	void     Clear()                       { Free(); bv.Clear(); }

	T&       AddHead()                     { T *q = new T; bv.AddHead(q); return *q; }
	T&       AddTail()                     { T *q = new T; bv.AddTail(q); return *q; }
	void     AddHead(const T& x)           { bv.AddHead(DeepCopyNew(x)); }
	void     AddTail(const T& x)           { bv.AddTail(DeepCopyNew(x)); }
	void     AddHeadPick(T rval_ x)       { bv.AddHead(new T(x)); }
	void     AddTailPick(T rval_ x)       { bv.AddTail(new T(x)); }
	T&       AddHead(T *newt)              { bv.AddHead(newt); return *newt; }
	T&       AddTail(T *newt)              { bv.AddTail(newt); return *newt; }
	template <class TT> TT& CreateHead()   { TT *q = new TT; bv.AddHead(q); return *q; }
	template <class TT> TT& CreateTail()   { TT *q = new TT; bv.AddTail(q); return *q; }
	T&       Head()                        { return *(T *) bv.Head(); }
	T&       Tail()                        { return *(T *) bv.Tail(); }
	const T& Head() const                  { return *(const T *) bv.Head(); }
	const T& Tail() const                  { return *(const T *) bv.Tail(); }
	void     DropHead()                    { delete (T*) bv.Head(); bv.DropHead(); }
	void     DropTail()                    { delete (T*) bv.Tail(); bv.DropTail(); }
	T       *DetachHead()                  { T *q = (T*) bv.Head(); bv.DropHead(); return q; }
	T       *DetachTail()                  { T *q = (T*) bv.Tail(); bv.DropTail(); return q; }

	T&       operator[](int i)             { return *(T *) bv[i]; }
	const T& operator[](int i) const       { return *(const T *) bv[i]; }

	void     Shrink()                      { bv.Shrink(); }
	void     Reserve(int n)                { bv.Reserve(n); }
	int      GetAlloc() const              { return bv.GetAlloc(); }

#ifdef UPP
	void     Serialize(Stream& s);
	String   ToString() const;
	bool     operator==(const BiArray<T>& b) const { return IsEqualArray(*this, b); }
	bool     operator!=(const BiArray<T>& b) const { return !operator==(b); }
	int      Compare(const BiArray<T>& b) const    { return CompareArray(*this, b); }
	bool     operator<=(const BiArray<T>& x) const { return Compare(x) <= 0; }
	bool     operator>=(const BiArray<T>& x) const { return Compare(x) >= 0; }
	bool     operator<(const BiArray<T>& x) const  { return Compare(x) < 0; }
	bool     operator>(const BiArray<T>& x) const  { return Compare(x) > 0; }
#endif

	bool     IsPicked() const                { return bv.IsPicked(); }

	BiArray(const BiArray& v, int)           { DeepCopy0(v); }

	BiArray(BiArray rval_ src) : bv(pick(src.bv))  {}
	void operator=(BiArray rval_ src)        { Free(); bv = pick(src.bv); }
	BiArray()                                {}
	~BiArray()                               { Free(); }

	typedef ConstIIterator<BiArray> ConstIterator;
	typedef IIterator<BiArray>      Iterator;

	typedef T        ValueType;

	ConstIterator    Begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    End() const                { return ConstIterator(*this, GetCount()); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         Begin()                    { return Iterator(*this, 0); }
	Iterator         End()                      { return Iterator(*this, GetCount()); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }

	friend void Swap(BiArray& a, BiArray& b)    { UPP::Swap(a.bv, b.bv); }

	STL_BI_COMPATIBILITY(BiArray<T>)
};
