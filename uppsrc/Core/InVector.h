template <class K, class T, class Less, class Data>
class SortedAMap;

template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;

struct InVectorSlave__ {
	virtual void Clear() = 0;
	virtual void Count(int n) = 0;
	virtual void Split(int blki, int nextsize) = 0;
	virtual void AddFirst() = 0;
	virtual void Insert(int blki, int pos) = 0;
	virtual void Join(int blki) = 0;
	virtual void Remove(int blki, int pos, int n) = 0;
	virtual void RemoveBlk(int blki, int n) = 0;
	virtual void Index(int blki, int n) = 0;
	virtual void Reindex() = 0;
};

template <class T>
class InVector : public MoveableAndDeepCopyOption< InVector<T> > {
public:
	class ConstIterator;
	class Iterator;

	template <class K, class TT, class Less, class Data> friend class SortedAMap;
	template <class TT> friend struct Slaved_InVector__;
	template <class TT> friend struct Slaved_InArray__;

private:
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;

	int    count;
	int    hcount;
	int64  serial;
	uint16 blk_high;
	uint16 blk_low;
	uint16 slave;

	InVectorSlave__ *Slave()          { return (InVectorSlave__ *)((byte *)this + slave); }
	void SetSlave(InVectorSlave__ *s) { slave = (uint16)((byte *)s - (byte *)this); }

	void SetCache(int blki, int offset) const;
	void ClearCache() const;
	int  FindBlock0(int& pos, int& off) const;
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void SetBlkPar();

	template <class L>
	int  FindUpperBound(const T& val, const L& less, int& off, int& pos) const;

	template <class L>
	int  FindLowerBound(const T& val, const L& less, int& off, int& pos) const;

	void Reindex();
	void Index(int q, int n);
	void Count(int n)                               { count += n; }
	void Join(int blki);

	bool JoinSmall(int blki);
	T   *Insert0(int ii, int blki, int pos, int off, const T *val);
	T   *Insert0(int ii, const T *val);
	
	void Reset();

	void SetIter(ConstIterator& it, int ii) const;
	void SetBegin(ConstIterator& it) const;
	void SetEnd(ConstIterator& it) const;

	void     Chk() const                            { ASSERT_(!IsPicked(), "Broken pick semantics"); }

#ifdef flagIVTEST
	void Check(int blki, int offset) const;
#endif

public:
	T&       Insert(int i)                          { return *Insert0(i, NULL); }
	T&       Insert(int i, const T& x)              { return *Insert0(i, &x); }
	void     InsertN(int i, int count);
	void     Remove(int i, int count = 1);

	const T& operator[](int i) const;
	T&       operator[](int i);

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	
	int      GetCount() const                       { Chk(); return count; }
	bool     IsEmpty() const                        { return GetCount() == 0; }

	void     Trim(int n)                            { Remove(n, GetCount() - n); }
	void     SetCount(int n);
	void     Clear();

	T&       At(int i)                              { if(i >= GetCount()) SetCount(i + 1); return (*this)[i]; }

	void     Shrink();

	void     Set(int i, const T& x, int count);
	T&       Set(int i, const T& x)                 { Set(i, x, 1); return (*this)(i); }

	void     Swap(int i1, int i2)                   { UPP::Swap((*this)[i1], (*this)[i2]); }

	void     Drop(int n = 1)                        { ASSERT(n <= GetCount()); Trim(GetCount() - n); }
	T&       Top()                                  { ASSERT(GetCount()); return (*this)[GetCount() - 1]; }
	const T& Top() const                            { ASSERT(GetCount()); return (*this)[GetCount() - 1]; }
	T        Pop()                                  { T h = Top(); Drop(); return h; }

	template <class L>
	int FindUpperBound(const T& val, const L& less) const { int off, pos; FindUpperBound(val, less, off, pos); return off + pos; }
	int FindUpperBound(const T& val) const                { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) const { int off, pos; FindLowerBound(val, less, off, pos); return off + pos; }
	int FindLowerBound(const T& val) const                { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val)                    { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less) const;
	int Find(const T& val) const                          { return Find(val, StdLess<T>()); }

	typedef T        ValueType;

	ConstIterator    Begin() const                  { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    End() const                    { ConstIterator it; SetEnd(it); return it; }
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }

	Iterator         Begin()                        { Iterator it; SetBegin(it); return it; }
	Iterator         End()                          { Iterator it; SetEnd(it); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }

	InVector();
	bool IsPicked() const                           { return data.IsPicked(); }

	InVector(const InVector& v, int);

	void Swap(InVector& b);

	friend void Swap(InVector& a, InVector& b)      { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

#ifdef _DEBUG
	void DumpIndex();
#endif
};

template <class T>
class InVector<T>::ConstIterator {
	const T        *ptr;
	const T        *begin;
	const T        *end;
	const InVector *v;
	int             offset;
	int             blki;

	friend class InVector<T>;
	friend class InVector<T>::Iterator;

	void NextBlk();
	void PrevBlk();		

public:
	force_inline int GetIndex() const              { return ptr - begin + offset; }

	force_inline ConstIterator& operator++()       { ASSERT(ptr); if(++ptr == end) NextBlk(); return *this; }
	force_inline ConstIterator& operator--()       { ASSERT(ptr); if(ptr == begin) PrevBlk(); --ptr; return *this; }
	force_inline ConstIterator  operator++(int)    { ConstIterator t = *this; ++*this; return t; }
	force_inline ConstIterator  operator--(int)    { ConstIterator t = *this; --*this; return t; }

	force_inline ConstIterator& operator+=(int d);
	ConstIterator& operator-=(int d)               { return operator+=(-d); }

	ConstIterator operator+(int d) const           { ConstIterator t = *this; t += d; return t; }
	ConstIterator operator-(int d) const           { return operator+(-d); }

	int  operator-(const ConstIterator& x) const   { return GetIndex() - x.GetIndex(); }

	bool operator==(const ConstIterator& b) const  { return ptr == b.ptr; }
	bool operator!=(const ConstIterator& b) const  { return ptr != b.ptr; }
	bool operator<(const ConstIterator& b) const   { return blki == b.blki ? ptr < b.ptr : blki < b.blki; }
	bool operator>(const ConstIterator& b) const   { return blki == b.blki ? ptr > b.ptr : blki > b.blki; }
	bool operator<=(const ConstIterator& b) const  { return blki == b.blki ? ptr <= b.ptr : blki <= b.blki; }
	bool operator>=(const ConstIterator& b) const  { return blki == b.blki ? ptr >= b.ptr : blki >= b.blki; }

	operator bool() const                          { return ptr; }

	const T& operator*() const                     { return *ptr; }
	const T *operator->() const                    { return ptr; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InVector<T>::Iterator : public InVector<T>::ConstIterator {
	typedef ConstIterator B;
public:
	Iterator& operator++()                         { ConstIterator::operator++(); return *this; }
	Iterator& operator--()                         { ConstIterator::operator--(); return *this; }
	Iterator  operator++(int)                      { Iterator t = *this; ++*this; return t; }
	Iterator  operator--(int)                      { Iterator t = *this; --*this; return t; }

	Iterator& operator+=(int d)                    { ConstIterator::operator+=(d); return *this; }
	Iterator& operator-=(int d)                    { return operator+=(-d); }

	Iterator operator+(int d) const                { Iterator t = *this; t += d; return t; }
	Iterator operator-(int d) const                { return operator+(-d); }

	int  operator-(const Iterator& x) const        { return B::GetIndex() - x.GetIndex(); }

	T& operator*()                                 { return *(T*)B::ptr; }
	T *operator->()                                { return (T*)B::ptr; }
	T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

	const T& operator*() const                     { return *B::ptr; }
	const T *operator->() const                    { return B::ptr; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
	
	typename InVector<T>::Iterator& operator=(const B& src) { *(B *)this = src; return *this; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InArray : public MoveableAndDeepCopyOption< InVector<T> > {
	template <class K, class TT, class Less, class Data> friend class SortedAMap;
	template <class TT> friend struct Slaved_InArray__;

public:
	class ConstIterator;
	class Iterator;

private:
#ifdef _DEBUG
	typedef T *PointerType;
#else
	typedef void *PointerType;
#endif
	typedef InVector<PointerType> IV;
	typedef typename InVector<PointerType>::Iterator IVIter;
	typedef typename InVector<PointerType>::Iterator IVConstIter;

	IV iv;

	T&       Get(int i) const       { return *(T *)iv[i]; }
	void     Delete(IVIter i, int count);
	void     Delete(int i, int count);
	void     Init(int i, int count);
	void     Free()                 { if(!IsPicked()) Delete(iv.Begin(), GetCount()); }

	void     SetIter(ConstIterator& it, int ii) const;
	void     SetBegin(ConstIterator& it) const;
	void     SetEnd(ConstIterator& it) const;

	template <class L>
	struct ALess {
		const L& less;
		
		bool operator()(const PointerType& a, const PointerType& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

	template <class K, class V, class L> friend class SortedVectorMap;

public:
	T&       Insert(int i, T *newt)                 { iv.Insert(i, newt); return *newt; }
	T&       Insert(int i)                          { return Insert(i, new T); }
	T&       Insert(int i, const T& x)              { return Insert(i, new T(x)); }
	template<class TT> TT& InsertCreate(int i)      { TT *q = new TT; Insert(i, q); return *q; }

	void     InsertN(int i, int count);
	void     Remove(int i, int count = 1);

	const T& operator[](int i) const                { return Get(i); }
	T&       operator[](int i)                      { return Get(i); }

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	T&       Add(T *newt)                           { Insert(GetCount(), newt); }
	template<class TT> TT& Create()                 { TT *q = new TT; Add(q); return *q; }
	
	int      GetCount() const                       { return iv.GetCount(); }
	bool     IsEmpty() const                        { return GetCount() == 0; }

	void     Trim(int n)                            { Remove(n, GetCount() - n); }

	void     SetCount(int n);
	void     Clear();

	T&       At(int i)                              { if(i >= GetCount()) SetCount(i + 1); return (*this)[i]; }

	void     Shrink()                               { iv.Shrink(); }

	void     Set(int i, const T& x, int count);
	T&       Set(int i, const T& x)                 { Set(i, x, 1); return Get(i); }

	void     Swap(int i1, int i2)                   { UPP::Swap((*this)[i1], (*this)[i2]); }

	void     Drop(int n = 1)                        { Trim(GetCount() - n); }
	T&       Top()                                  { return (*this)[GetCount() - 1]; }
	const T& Top() const                            { return (*this)[GetCount() - 1]; }
	T        Pop()                                  { T h = Top(); Drop(); return h; }

	template <class L>
	int FindUpperBound(const T& val, const L& less) const  { return iv.FindUpperBound((T*)&val, ALess<L>(less)); }
	int FindUpperBound(const T& val) const                 { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) const  { return iv.FindLowerBound((T*)&val, ALess<L>(less)); }
	int FindLowerBound(const T& val) const                 { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& lss)       { return iv.InsertUpperBound(new T(val), ALess<L>(lss)); }
	int InsertUpperBound(const T& val)                     { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less) const            { return iv.Find((T*)&val, ALess<L>(less)); }
	int Find(const T& val) const                           { return Find(val, StdLess<T>()); }

	typedef T        ValueType;

	ConstIterator    Begin() const                  { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    End() const                    { ConstIterator it; SetEnd(it); return it; }
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }

	Iterator         Begin()                        { Iterator it; SetBegin(it); return it; }
	Iterator         End()                          { Iterator it; SetEnd(it); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }

	bool IsPicked() const                           { return iv.IsPicked(); }

	InArray() {}
	InArray(const InArray& v, int);
	~InArray()                                      { Free(); }
	
	void Swap(InArray& b)                           { iv.Swap(b.iv); }
	
	friend void Swap(InArray& a, InArray& b)        { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InArray<T>)

#ifdef _DEBUG
	void DumpIndex();
#endif
};

template <class T>
class InArray<T>::ConstIterator {
	IVConstIter it;

	friend class InArray<T>;
	friend class InArray<T>::Iterator;

public:
	force_inline int GetIndex() const              { return it.GetIndex(); }

	force_inline ConstIterator& operator++()       { ++it; return *this; }
	force_inline ConstIterator& operator--()       { --it; return *this; }
	force_inline ConstIterator  operator++(int)    { ConstIterator t = *this; ++*this; return t; }
	force_inline ConstIterator  operator--(int)    { ConstIterator t = *this; --*this; return t; }

	force_inline ConstIterator& operator+=(int d)  { it += d; return *this; }
	ConstIterator& operator-=(int d)               { return operator+=(-d); }

	ConstIterator operator+(int d) const           { ConstIterator t = *this; t += d; return t; }
	ConstIterator operator-(int d) const           { return operator+(-d); }

	int  operator-(const ConstIterator& x) const   { return GetIndex() - x.GetIndex(); }

	bool operator==(const ConstIterator& b) const  { return it == b.it; }
	bool operator!=(const ConstIterator& b) const  { return it != b.it; }
	bool operator<(const ConstIterator& b) const   { return it < b.it; }
	bool operator>(const ConstIterator& b) const   { return it > b.it; }
	bool operator<=(const ConstIterator& b) const  { return it <= b.it; }
	bool operator>=(const ConstIterator& b) const  { return it >= b.it; }

	operator bool() const                          { return it; }

	const T& operator*() const                     { return *(T *)*it; }
	const T *operator->() const                    { return (T *)*it; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T>
class InArray<T>::Iterator : public InArray<T>::ConstIterator {
	typedef ConstIterator B;

public:
	Iterator& operator++()                         { ConstIterator::operator++(); return *this; }
	Iterator& operator--()                         { ConstIterator::operator--(); return *this; }
	Iterator  operator++(int)                      { Iterator t = *this; ++*this; return t; }
	Iterator  operator--(int)                      { Iterator t = *this; --*this; return t; }

	Iterator& operator+=(int d)                    { ConstIterator::operator+=(d); return *this; }
	Iterator& operator-=(int d)                    { return operator+=(-d); }

	Iterator operator+(int d) const                { Iterator t = *this; t += d; return t; }
	Iterator operator-(int d) const                { return operator+(-d); }

	int  operator-(const Iterator& x) const        { return B::GetIndex() - x.GetIndex(); }

	T& operator*()                                 { return *(T *)*B::it; }
	T *operator->()                                { return (T *)*B::it; }
	T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

	const T& operator*() const                     { return *(T *)*B::it; }
	const T *operator->() const                    { return (T *)*B::it; }
	const T& operator[](int i) const               { Iterator h = *this; h += i; return *h; }

	STL_ITERATOR_COMPATIBILITY
};

template <class T, class Less = StdLess<T> >
class SortedIndex : MoveableAndDeepCopyOption< SortedIndex<T, Less> > {
	InVector<T> iv;
	
	template <class K, class TT, class Lss, class Data> friend class SortedAMap;

public:
	int           Add(const T& x)                  { return iv.InsertUpperBound(x, Less()); }
	int           FindAdd(const T& key);
	SortedIndex&  operator<<(const T& x)           { Add(x); return *this; }
	
	int           FindLowerBound(const T& x) const { return iv.FindLowerBound(x, Less()); }
	int           FindUpperBound(const T& x) const { return iv.FindUpperBound(x, Less()); }

	int           Find(const T& x) const           { return iv.Find(x, Less()); }
	int           FindNext(int i) const;
	int           FindLast(const T& x) const;
	int           FindPrev(int i) const;

	void          Remove(int i)                    { iv.Remove(i); }
	void          Remove(int i, int count)         { iv.Remove(i, count); }
	int           RemoveKey(const T& x);

	const T&      operator[](int i) const          { return iv[i]; }
	int           GetCount() const                 { return iv.GetCount(); }
	bool          IsEmpty() const                  { return iv.IsEmpty(); }
	void          Clear()                          { iv.Clear(); }

	void          Trim(int n)                      { return iv.Trim(n); }
	void          Drop(int n = 1)                  { iv.Drop(n); }
	const T&      Top() const                      { return iv.Top(); }

	void          Shrink()                         { iv.Shrink(); }
	
	typedef typename InVector<T>::ConstIterator ConstIterator;
	
	typedef T        ValueType;

	ConstIterator    Begin() const                  { return iv.Begin(); }
	ConstIterator    End() const                    { return iv.End(); }
	ConstIterator    GetIter(int pos) const         { return iv.GetIter(pos); }
	
	const InVector<T>& GetKeys()                    { return iv; }


	SortedIndex()                                        {}
	SortedIndex(const SortedIndex& s, int) : iv(s.iv, 1) {}

	void Swap(SortedIndex& a)                       { iv.Swap(a.iv); }

	friend void Swap(SortedIndex& a, SortedIndex& b){ a.Swap(b); }

	STL_SINDEX_COMPATIBILITY(SortedIndex<T _cm_ Less>)
};

template <class K, class T, class Less, class Data>
class SortedAMap : MoveableAndDeepCopyOption< SortedAMap<K, T, Less, Data> > {
protected:
	SortedIndex<K, Less> key;
	Data                 value;

	void     SetSlave()      { key.iv.SetSlave(&value); }
	T&       At(int i) const { int blki = key.iv.FindBlock(i); return value.Get(blki, i); }

public:
	int      FindLowerBound(const K& k) const       { return key.FindLowerBound(k); }
	int      FindUpperBound(const K& k) const       { return key.FindUpperBound(k); }

	int      Find(const K& k) const                 { return key.Find(k); }
	int      FindNext(int i) const                  { return key.FindNext(i); }
	int      FindLast(const K& k) const             { return key.FindLast(k); }
	int      FindPrev(int i) const                  { return key.FindPrev(i); }

	T&       Get(const K& k)                        { return At(Find(k)); }
	const T& Get(const K& k) const                  { return At(Find(k)); }
	const T& Get(const K& k, const T& d) const      { int i = Find(k); return i >= 0 ? value[i] : d; }

	T       *FindPtr(const K& k)                    { int i = Find(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindPtr(const K& k) const              { int i = Find(k); return i >= 0 ? &value[i] : NULL; }

	const K& GetKey(int i) const                    { return key[i]; }
	const T& operator[](int i) const                { return At(i); }
	T&       operator[](int i)                      { return At(i); }
	int      GetCount() const                       { return key.GetCount(); }
	bool     IsEmpty() const                        { return key.IsEmpty(); }
	void     Clear()                                { key.Clear(); }
	void     Shrink()                               { key.Shrink(); value.Shrink(); }

	void     Remove(int i)                          { key.Remove(i); }
	void     Remove(int i, int count)               { key.Remove(i, count); }
	int      RemoveKey(const K& k)                  { return key.RemoveKey(k); }

	void     Drop(int n = 1)                        { key.Drop(n); }
	T&       Top()                                  { return value.Top(); }
	const T& Top() const                            { return value.Top(); }
	const K& TopKey() const                         { return key.Top(); }
	K        PopKey()                               { K h = TopKey(); Drop(); return h; }
	void     Trim(int n)                            { key.Trim(n); }

	void     Swap(SortedAMap& x)                    { Swap(value, x.value); Swap(key, x.key); }
	
	bool     IsPicked() const                       { return value.IsPicked() || key.IsPicked(); }
	
	const SortedIndex<K>& GetIndex() const          { return key; }
	const InVector<K>& GetKeys() const              { return key.GetKeys(); }
	const InVector<T>& GetValues() const            { return value; }

	SortedAMap()                                    { SetSlave(); }
	SortedAMap(const SortedAMap& s, int) : key(s.key, 0), value(s.value, 0) { SetSlave(); }

	typedef K        KeyType;

	typedef typename SortedIndex<K, Less>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.Begin(); }
	KeyConstIterator KeyEnd() const                               { return key.End(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }

	typedef T                                   ValueType;
	typedef typename Data::Type::ConstIterator  ConstIterator;
	typedef typename Data::Type::Iterator       Iterator;

	Iterator         Begin()                                      { return value.data.Begin(); }
	Iterator         End()                                        { return value.data.End(); }
	Iterator         GetIter(int pos)                             { return value.data.GetIter(pos); }
	ConstIterator    Begin() const                                { return value.data.Begin(); }
	ConstIterator    End() const                                  { return value.data.End(); }
	ConstIterator    GetIter(int pos) const                       { return value.data.GetIter(pos); }
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef InVector<T> Type;
	InVector<T> data;
	const T *ptr;
      
	virtual void Clear()                          { data.Clear(); }
	virtual void Count(int n)                     { data.count += n; }
	virtual void Insert(int blki, int pos);
	virtual void Split(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { data.data.Remove(blki, n); }
	virtual void Join(int blki)                   { data.Join(blki); }
	virtual void Remove(int blki, int pos, int n) { data.data[blki].Remove(pos, n); }
	virtual void Index(int blki, int n)           { data.Index(blki, n); }
	virtual void Reindex()                        { data.Reindex(); }

	T& Get(int blki, int i) const                 { return *(T*)&data.data[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedVectorMap : public MoveableAndDeepCopyOption<SortedVectorMap<K, T, Less> >,
                        public SortedAMap<K, T, Less, Slaved_InVector__<T> > {
    typedef SortedAMap<K, T, Less, Slaved_InVector__<T> > B;

public:
	T&       Add(const K& k, const T& x)            { B::value.ptr = &x; B::key.Add(k); return *(T*)B::value.ptr; }
	T&       Add(const K& k)                        { B::value.ptr = NULL; B::key.Add(k); return *(T*)B::value.ptr; }

	int      FindAdd(const K& k)                    { B::value.ptr = NULL; return B::key.FindAdd(k); }
	int      FindAdd(const K& k, const T& init)     { B::value.ptr = &init; return B::key.FindAdd(k); }

	T&       GetAdd(const K& k)                     { return B::At(FindAdd(k)); }
	T&       GetAdd(const K& k, const T& x)         { return B::At(FindAdd(k, x)); }

	T        Pop()                                  { T h = B::Top(); B::Drop(); return h; }

	SortedVectorMap(const SortedVectorMap& s, int) : B(s, 1) {}
	SortedVectorMap()                                                       {}

	friend void    Swap(SortedVectorMap& a, SortedVectorMap& b) { a.Swap(b); }

	typedef typename B::ConstIterator ConstIterator;
	typedef typename B::Iterator      Iterator;
	STL_MAP_COMPATIBILITY(SortedVectorMap<K _cm_ T _cm_ Less>)
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef InArray<T> Type;
	InArray<T> data;
	T         *ptr;
	bool       mk;
      
	virtual void Clear()                          { data.Clear(); }
	virtual void Count(int n)                     { data.iv.count += n; }
	virtual void Insert(int blki, int pos);
	virtual void Split(int blki, int nextsize);
	virtual void AddFirst();
	virtual void RemoveBlk(int blki, int n)       { data.iv.data.Remove(blki, n); }
	virtual void Join(int blki)                   { data.iv.Join(blki); }
	virtual void Remove(int blki, int pos, int n);
	virtual void Index(int blki, int n)           { data.iv.Index(blki, n); }
	virtual void Reindex()                        { data.iv.Reindex(); }

	T& Get(int blki, int i) const                 { return *(T*)data.iv.data[blki][i]; }
	T *Detach(int i)                              { T *x = data.iv[i]; data.iv[i] = NULL; return x; }
	
	Slaved_InArray__()                            { mk = false; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedArrayMap : public MoveableAndDeepCopyOption<SortedArrayMap<K, T, Less> >,
                        public SortedAMap<K, T, Less, Slaved_InArray__<T> > {
    typedef SortedAMap<K, T, Less, Slaved_InArray__<T> > B;

public:
	T&       Add(const K& k, const T& x)          { B::value.ptr = new T(x); B::key.Add(k); return *(T*)B::value.ptr; }
	T&       Add(const K& k)                      { B::value.ptr = new T; B::key.Add(k); return *(T*)B::value.ptr; }
	T&       Add(const K& k, T *newt)             { B::value.ptr = newt; B::key.Add(k); return *newt; }
	template <class TT> TT& Create(const K& k)    { TT *q = new TT; B::value.ptr = q; B::key.Add(k); return *q; }

	int      FindAdd(const K& k)                  { B::value.ptr = NULL; return B::key.FindAdd(k); }
	int      FindAdd(const K& k, const T& init)   { B::value.ptr = (T*)&init; B::value.mk = true; int x = B::key.FindAdd(k); B::value.mk = false; return x; }

	T&       GetAdd(const K& k)                   { return B::At(FindAdd(k)); }
	T&       GetAdd(const K& k, const T& x)       { return B::At(FindAdd(k, x)); }

	T       *Detach(int i)                        { T *x = B::value.Detach(i); B::Remove(i); return x; }
	T       *PopDetach()                          { return Detach(B::GetCount() - 1); }

	SortedArrayMap(const SortedArrayMap& s, int) : B(s, 1) {}
	SortedArrayMap() {}

	friend void    Swap(SortedArrayMap& a, SortedArrayMap& b) { a.Swap(b); }

	typedef typename B::ConstIterator ConstIterator;
	typedef typename B::Iterator      Iterator;
	STL_MAP_COMPATIBILITY(SortedArrayMap<K _cm_ T _cm_ HashFn>)
};

#define LLOG(x)   // DLOG(x)
#include "InVector.hpp"
#undef  LLOG
