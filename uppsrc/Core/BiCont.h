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
	T       *AddHead0()              { Add0(); return &vector[start = Ix(alloc - 1)/*(start + alloc - 1) % alloc*/]; }
	T       *AddTail0()              { Add0(); return &vector[EI()]; }
	void     Zero()                  { start = items = alloc = 0; vector = NULL; }
	void     Free();
	void     Pick(BiVector&& x)      { vector = pick(x.vector); start = x.start; items = x.items;
	                                   alloc = x.alloc; ((BiVector&)x).items = -1; }
	void     Copy(T *dst, int start, int count) const;

public:
	static_assert(is_trivially_relocatable<T> || is_upp_guest<T>);

	int      GetCount() const        { return items; }
	bool     IsEmpty() const         { return items == 0; }
	void     Clear();

	T&       AddHead()               { return *new(AddHead0()) T; }
	T&       AddTail()               { return *new(AddTail0()) T; }
	template <class... Args>
	T&       CreateHead(Args&&... args) { return *new(AddHead0()) T(std::forward<Args>(args)...); }
	template <class... Args>
	T&       CreateTail(Args&&... args) { return *new(AddTail0()) T(std::forward<Args>(args)...); }
	void     AddHead(const T& x)     { new(AddHead0()) T(x); }
	void     AddTail(const T& x)     { new(AddTail0()) T(x); }
	void     AddHead(T&& x)          { new(AddHead0()) T(pick(x)); }
	void     AddTail(T&& x)          { new(AddTail0()) T(pick(x)); }
	T&       Head()                  { ASSERT(items > 0); return vector[start]; }
	T&       Tail()                  { ASSERT(items > 0); return vector[EI()]; }
	const T& Head() const            { ASSERT(items > 0); return vector[start]; }
	const T& Tail() const            { ASSERT(items > 0); return vector[EI()]; }
	void     DropHead()              { (&Head())->~T(); items--; start = Ix(1); }
	void     DropTail()              { (&Tail())->~T(); items--; }
	T        PopHead()               { T x = Head(); DropHead(); return x; }
	T        PopTail()               { T x = Tail(); DropTail(); return x; }
	void     DropHead(int n)         { while(n-- > 0) BiVector<T>::DropHead(); }
	void     DropTail(int n)         { while(n-- > 0) BiVector<T>::DropTail(); }
	const T& operator[](int i) const { ASSERT(i >= 0 && i < items); return vector[Ix(i)]; }
	T&       operator[](int i)       { ASSERT(i >= 0 && i < items); return vector[Ix(i)]; }
	void     Shrink();
	void     Reserve(int n);
	int      GetAlloc() const        { return alloc; }

	void     Serialize(Stream& s);
	String   ToString() const;
	hash_t   GetHashValue() const    { return HashBySerialize(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	BiVector(const BiVector& src, int)          { DeepCopy0(src); }
	BiVector(BiVector&& src)                    { Pick(pick(src)); }
	void operator=(BiVector&& src)              { if(this != &src) { Free(); Pick(pick(src)); } }
	BiVector()                                  { Zero(); }
	~BiVector()                                 { Free(); } // gcc4.0 workaround!!

	BiVector(std::initializer_list<T> init);

	typedef ConstIIterator<BiVector> ConstIterator;
	typedef IIterator<BiVector>      Iterator;

	ConstIterator    begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    end() const                { return ConstIterator(*this, GetCount()); }
	Iterator         begin()                    { return Iterator(*this, 0); }
	Iterator         end()                      { return Iterator(*this, GetCount()); }

	friend void Swap(BiVector& a, BiVector& b)  { UPP::Swap(a.vector, b.vector);
	                                              UPP::Swap(a.start, b.start);
	                                              UPP::Swap(a.items, b.items);
	                                              UPP::Swap(a.alloc, b.alloc); }

#ifdef DEPRECATED
	void     AddHeadPick(T&& x)      { new(AddHead0()) T(pick(x)); }
	void     AddTailPick(T&& x)      { new(AddTail0()) T(pick(x)); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }
	typedef T        ValueType;
	STL_BI_COMPATIBILITY(BiVector<T>)
#endif
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
	void     AddHead(const T& x)           { bv.AddHead(new T(x)); }
	void     AddTail(const T& x)           { bv.AddTail(new T(x)); }
	T&       AddHead(T *newt)              { bv.AddHead(newt); return *newt; }
	T&       AddTail(T *newt)              { bv.AddTail(newt); return *newt; }
	template <class... Args>
	T& CreateHead(Args&&... args)          { T *q = new T(std::forward<Args>(args)...); bv.AddHead(q); return *q; }
	template <class... Args>
	T& CreateTail(Args&&... args)          { T *q = new T(std::forward<Args>(args)...); bv.AddTail(q); return *q; }
	template <class TT, class... Args>
	TT& CreateHead(Args&&... args)         { TT *q = new TT(std::forward<Args>(args)...); bv.AddHead(q); return *q; }
	template <class TT, class... Args>
	TT& CreateTail(Args&&... args)         { TT *q = new TT(std::forward<Args>(args)...); bv.AddTail(q); return *q; }
	T&       AddHead(One<T>&& one)         { ASSERT(one); return AddHead(one.Detach()); }
	T&       AddTail(One<T>&& one)         { ASSERT(one); return AddTail(one.Detach()); }
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

	void     Serialize(Stream& s);
	String   ToString() const;
	hash_t   GetHashValue() const    { return HashBySerialize(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	BiArray(const BiArray& v, int)           { DeepCopy0(v); }

	BiArray(BiArray&& src) : bv(pick(src.bv)){}
	void operator=(BiArray&& src)            { if(this != &src) { Free(); bv = pick(src.bv); } }
	BiArray()                                {}
	~BiArray()                               { Free(); }

	BiArray(std::initializer_list<T> init);

	typedef ConstIIterator<BiArray> ConstIterator;
	typedef IIterator<BiArray>      Iterator;

	ConstIterator    begin() const              { return ConstIterator(*this, 0); }
	ConstIterator    end() const                { return ConstIterator(*this, GetCount()); }
	Iterator         begin()                    { return Iterator(*this, 0); }
	Iterator         end()                      { return Iterator(*this, GetCount()); }

	friend void Swap(BiArray& a, BiArray& b)    { UPP::Swap(a.bv, b.bv); }

#ifdef DEPRECATED
	void     AddHeadPick(T&& x)            { bv.AddHead(new T(pick(x))); }
	void     AddTailPick(T&& x)            { bv.AddTail(new T(pick(x))); }
	ConstIterator    GetIter(int pos) const     { return ConstIterator(*this, pos); }
	Iterator         GetIter(int pos)           { return Iterator(*this, pos); }
	typedef T        ValueType;
	STL_BI_COMPATIBILITY(BiArray<T>)
#endif
};
