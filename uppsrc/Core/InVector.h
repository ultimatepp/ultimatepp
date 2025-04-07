template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;
template <class K, class TT, class Lss, class Data> class SortedAMap;
template <class K, class TT, class Less> class SortedVectorMap;

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
	virtual void Shrink() = 0;
};

template <class T>
class InVector : public MoveableAndDeepCopyOption< InVector<T> > {
public:
	class ConstIterator;
	class Iterator;

	template <class K, class TT, class Lss, class Data> friend class SortedAMap;
	template <class K, class TT, class Less> friend class SortedVectorMap;
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

	void Pick(InVector&& b);

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

	void     Init();

	template <class Range>
	void     Insert_(int ii, const Range& r, bool def);

#ifdef flagIVTEST // .activate additional diagnostics code for InVector
	void Check(int blki, int offset) const;
#endif
public:
	T&       Insert(int i)                          { return *Insert0(i, NULL); }
	T&       Insert(int i, const T& x)              { return *Insert0(i, &x); }
	template <class Range>
	void     InsertRange(int i, const Range& r)     { Insert_(i, r, false); }
	void     InsertN(int i, int count)              { Insert_(i, ConstRange<T>(count), true); }
	void     Remove(int i, int count = 1);
	template <class Range>
	void     AppendRange(const Range& r)            { InsertRange(GetCount(), r); }

	const T& operator[](int i) const;
	T&       operator[](int i);

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	
	int      GetCount() const                       { return count; }
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
	T        Pop()                                  { T h = pick(Top()); Drop(); return h; }

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
	int Find(const T& val) const                         { return Find(val, StdLess<T>()); }

	ConstIterator    begin() const                       { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    end() const                         { ConstIterator it; SetEnd(it); return it; }

	Iterator         begin()                             { Iterator it; SetBegin(it); return it; }
	Iterator         end()                               { Iterator it; SetEnd(it); return it; }

	InVector();
	InVector(InVector&& v)                               { Pick(pick(v)); }
	void operator=(InVector&& v)                         { Pick(pick(v)); }
	InVector(const InVector& v, int);
	InVector(std::initializer_list<T> init)              { Init(); for(const auto& i : init) Add(i); }

	void Swap(InVector& b);

	void     Serialize(Stream& s)                        { StreamContainer(s, *this); }
	void     Xmlize(XmlIO& xio, const char *itemtag = "item");
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	hash_t   GetHashValue() const                        { return HashBySerialize(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
#ifndef CPP_20
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
#endif
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	friend void Swap(InVector& a, InVector& b)      { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

	void DumpIndex() const;
	
	void Reserve(int) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)
#ifdef DEPRECATED
	typedef T        ValueType;
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }
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
	force_inline int GetIndex() const              { return int(ptr - begin) + offset; }

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
class InArray : public MoveableAndDeepCopyOption< InArray<T> > {
//	template <class K, class TT, class Less, class Data> friend class SortedAMap;
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
	void     Free()                 { Delete(iv.begin(), GetCount()); }

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
	template<class TT, class... Args>
	TT&      InsertCreate(int i, Args&&... args)    { TT *q = new TT(std::forward<Args>(args)...); Insert(i, q); return *q; }

	void     InsertN(int i, int count);
	template <class Range>
	void     InsertRange(int i, const Range& r);
	void     Remove(int i, int count = 1);
	
	template <class Range>
	void     AppendRange(const Range& r)            { InsertRange(GetCount(), r); }

	const T& operator[](int i) const                { return Get(i); }
	T&       operator[](int i)                      { return Get(i); }

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	T&       Add(T *newt)                           { Insert(GetCount(), newt); return *newt; }
	template<class TT, class... Args>
	TT&      Create(Args&&... args)                 { TT *q = new TT(std::forward<Args>(args)...); Add(q); return *q; }
	
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
	T        Pop()                                  { T h = pick(Top()); Drop(); return h; }

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

	ConstIterator    begin() const                  { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    end() const                    { ConstIterator it; SetEnd(it); return it; }

	Iterator         begin()                        { Iterator it; SetBegin(it); return it; }
	Iterator         end()                          { Iterator it; SetEnd(it); return it; }

	InArray() {}
	InArray(InArray&& v) : iv(pick(v.iv))           {}
	InArray& operator=(InArray&& v)                 { if(this != &v) { Free(); iv.operator=(pick(v.iv)); } return *this; }
	InArray(const InArray& v, int);

	~InArray()                                      { Free(); }

	InArray(std::initializer_list<T> init)          { for(const auto& i : init) Add(i); }

	void Swap(InArray& b)                           { iv.Swap(b.iv); }
	
	void     Serialize(Stream& s)                   { StreamContainer(s, *this); }
	void     Xmlize(XmlIO& xio, const char *itemtag = "item");
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	hash_t   GetHashValue() const                        { return HashBySerialize(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
#ifndef CPP_20
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
#endif
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	friend void Swap(InArray& a, InArray& b)        { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InArray<T>)

	void Reserve(int) {} // Does nothing, but needed for unified interface (e.g. StreamContainer)

#ifdef _DEBUG
	void DumpIndex();
#endif

#ifdef DEPRECATED
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }
	typedef T        ValueType;
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

template <class T, class Less = std::less<T> >
class SortedIndex : MoveableAndDeepCopyOption< SortedIndex<T, Less> > {
	InVector<T> iv;
	
	template <class K, class TT, class Lss> friend class SortedVectorMap;
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

	ConstIterator    begin() const                  { return iv.begin(); }
	ConstIterator    end() const                    { return iv.end(); }
	
	const InVector<T>& GetKeys()  const             { return iv; }

	SortedIndex()                                        {}
	SortedIndex(const SortedIndex& s, int) : iv(s.iv, 1) {}
	
	void Swap(SortedIndex& a)                        { iv.Swap(a.iv); }

	void     Serialize(Stream& s)                               { iv.Serialize(s); }
	void     Xmlize(XmlIO& xio, const char *itemtag = "key")    { iv.Xmlize(xio, itemtag); }
	void     Jsonize(JsonIO& jio)                               { iv.Jsonize(jio); }
	String   ToString() const;
	hash_t   GetHashValue() const                        { return HashBySerialize(*this); }
	
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
#ifndef CPP_20
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
#endif
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	friend void Swap(SortedIndex& a, SortedIndex& b){ a.Swap(b); }

#ifdef DEPRECATED
	ConstIterator    GetIter(int pos) const         { return iv.GetIter(pos); }
	
	typedef T        ValueType;

	typedef T             value_type;
	typedef ConstIterator const_iterator;
	typedef const T&      const_reference;
	typedef int           size_type;
	typedef int           difference_type;
	const_iterator        Begin() const          { return begin(); }
	const_iterator        End() const            { return end(); }
	void                  clear()                { Clear(); }
	size_type             size()                 { return GetCount(); }
	bool                  empty() const          { return IsEmpty(); }
#endif
};

template <class K, class T, class Less, class Data>
class SortedAMap {
protected:
	SortedIndex<K, Less> key;
	Data value;

	void     SetSlave()                             { key.iv.SetSlave(&value); }
	T&       At(int i) const                        { return (T&)value.data[i]; }

public:
	int      FindLowerBound(const K& k) const       { return key.FindLowerBound(k); }
	int      FindUpperBound(const K& k) const       { return key.FindUpperBound(k); }

	int      Find(const K& k) const                 { return key.Find(k); }
	int      FindNext(int i) const                  { return key.FindNext(i); }
	int      FindLast(const K& k) const             { return key.FindLast(k); }
	int      FindPrev(int i) const                  { return key.FindPrev(i); }

	T&       Get(const K& k)                        { return At(Find(k)); }
	const T& Get(const K& k) const                  { return At(Find(k)); }
	const T& Get(const K& k, const T& d) const      { int i = Find(k); return i >= 0 ? At(i) : d; }

	T       *FindPtr(const K& k)                    { int i = Find(k); return i >= 0 ? &At(i) : NULL; }
	const T *FindPtr(const K& k) const              { int i = Find(k); return i >= 0 ? &At(i) : NULL; }

	const T& operator[](int i) const                { return At(i); }
	T&       operator[](int i)                      { return At(i); }

	const K& GetKey(int i) const                    { return key[i]; }

	int      GetCount() const                       { return key.GetCount(); }
	bool     IsEmpty() const                        { return key.IsEmpty(); }
	void     Clear()                                { key.Clear(); }
	void     Shrink()                               { key.Shrink(); value.Shrink(); }

	void     Remove(int i)                          { key.Remove(i); }
	void     Remove(int i, int count)               { key.Remove(i, count); }
	int      RemoveKey(const K& k)                  { return key.RemoveKey(k); }

	const SortedIndex<K>& GetIndex() const          { return key; }
	const InVector<K>& GetKeys() const              { return key.GetKeys(); }

	String   ToString() const;
	bool     operator==(const SortedAMap& b) const  { return IsEqualMap(*this, b); }
	bool     operator!=(const SortedAMap& b) const  { return !operator==(b); }
	int      Compare(const SortedAMap& b) const     { return CompareMap(*this, b); }
	bool     operator<=(const SortedAMap& x) const  { return Compare(x) <= 0; }
	bool     operator>=(const SortedAMap& x) const  { return Compare(x) >= 0; }
	bool     operator<(const SortedAMap& x) const   { return Compare(x) < 0; }
	bool     operator>(const SortedAMap& x) const   { return Compare(x) > 0; }

#ifdef DEPRECATED
	typedef K        KeyType;

	typedef typename SortedIndex<K, Less>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.begin(); }
	KeyConstIterator KeyEnd() const                               { return key.end(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }
#endif
};

template <class T>
struct Slaved_InVector__ : InVectorSlave__ {
	typedef InVector<T> Type;
	InVector<T> data;
	T *res;
      
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
//	virtual void Serialize(Stream& s)             { data.Serialize(s); }
	virtual void Shrink()                         { data.Shrink(); }

	T& Get(int blki, int i) const                 { return *(T*)&data.data[blki][i]; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedVectorMap : public SortedAMap<K, T, Less, Slaved_InVector__<T> >,
                        public MoveableAndDeepCopyOption<SortedVectorMap<K, T, Less> > {
	typedef Slaved_InVector__<T> Data;
    typedef SortedAMap<K, T, Less, Data>  B;
    
public:
	T&       Add(const K& k)                        { B::key.Add(k); return *B::value.res; }
	T&       Add(const K& k, const T& x)            { B::key.Add(k); *B::value.res = clone(x); return *B::value.res; }
	T&       Add(const K& k, T&& x)                 { B::key.Add(k); *B::value.res = pick(x); return *B::value.res; }

	int      FindAdd(const K& k)                    { return B::key.FindAdd(k); }
	int      FindAdd(const K& k, const T& init);

	T&       GetAdd(const K& k)                     { return B::At(FindAdd(k)); }
	T&       GetAdd(const K& k, const T& x)         { return B::At(FindAdd(k, x)); }

	SortedVectorMap& operator()(const K& k, const T& v) { Add(k, v); return *this; }

	SortedVectorMap()                               { B::SetSlave(); }
	SortedVectorMap(SortedVectorMap&&);
	SortedVectorMap& operator=(SortedVectorMap&&);
	SortedVectorMap(const SortedVectorMap& s, int);
	
	SortedVectorMap(std::initializer_list<std::pair<K, T>> init) : SortedVectorMap() { for(const auto& i : init) Add(i.first, i.second); }

	void     Swap(SortedVectorMap& x);

#ifdef UPP
	void     Serialize(Stream& s);
	void     Xmlize(XmlIO& xio);
	void     Jsonize(JsonIO& jio);
	hash_t   GetHashValue() const                   { return HashBySerialize(*this); }
#endif

	const InVector<T>& GetValues() const            { return B::value.data; }

	friend void    Swap(SortedVectorMap& a, SortedVectorMap& b) { a.Swap(b); }

	typedef typename Data::Type::ConstIterator  ConstIterator;
	typedef typename Data::Type::Iterator       Iterator;

	Iterator         begin()                        { return B::value.data.begin(); }
	Iterator         end()                          { return B::value.data.end(); }
	ConstIterator    begin() const                  { return B::value.data.begin(); }
	ConstIterator    end() const                    { return B::value.data.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedVectorMap<K _cm_ T _cm_ Less>)

#ifdef DEPRECATED
	typedef T                                       ValueType;
	Iterator         GetIter(int pos)               { return B::value.data.GetIter(pos); }
	ConstIterator    GetIter(int pos) const         { return B::value.data.GetIter(pos); }
#endif
};

template <class T>
struct Slaved_InArray__ : InVectorSlave__ {
	typedef InArray<T> Type;
	InArray<T> data;
	T         *res;
      
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
//	virtual void Serialize(Stream& s)             { data.iv.Serialize(s); }
	virtual void Shrink()                         { data.iv.Shrink(); }

	T& Get(int blki, int i) const                 { return *(T*)data.iv.data[blki][i]; }
	T *Detach(int i)                              { T *x = data.iv[i]; data.iv[i] = NULL; return x; }
};

template <class K, class T, class Less = StdLess<K> >
class SortedArrayMap : public MoveableAndDeepCopyOption<SortedArrayMap<K, T, Less> >,
                        public SortedAMap<K, T, Less, Slaved_InArray__<T> > {
	typedef Slaved_InArray__<T> Data;
    typedef SortedAMap<K, T, Less, Data> B;

public:
	T&       Add(const K& k, const T& x)          { B::value.res = new T(clone(x)); B::key.Add(k); return *(T*)B::value.res; }
	T&       Add(const K& k, T&& x)               { B::value.res = new T(pick(x)); B::key.Add(k); return *(T*)B::value.res; }
	T&       Add(const K& k)                      { B::value.res = NULL; B::key.Add(k); return *(T*)B::value.res; }
	T&       Add(const K& k, T *newt)             { B::value.res = newt; B::key.Add(k); return *newt; }
	template <class TT, class... Args>
	TT&      Create(const K& k, Args&&... args)   { TT *q = new TT(std::forward<Args>(args)...); B::value.res = q; B::key.Add(k); return *q; }

	int      FindAdd(const K& k)                  { B::value.res = NULL; return B::key.FindAdd(k); }
	int      FindAdd(const K& k, const T& init);

	T&       GetAdd(const K& k)                   { return B::At(FindAdd(k)); }
	T&       GetAdd(const K& k, const T& x)       { return B::At(FindAdd(k, x)); }

	T       *Detach(int i)                        { T *x = B::value.Detach(i); B::Remove(i); return x; }
	T       *PopDetach()                          { return Detach(B::GetCount() - 1); }

	const InArray<T>& GetValues() const           { return B::value.data; }

	SortedArrayMap& operator()(const K& k, const T& v) { Add(k, v); return *this; }

	SortedArrayMap()                              { B::SetSlave(); }
	SortedArrayMap(SortedArrayMap&&);
	SortedArrayMap& operator=(SortedArrayMap&&);
	SortedArrayMap(const SortedArrayMap& s, int);
	SortedArrayMap(std::initializer_list<std::pair<K, T>> init) : SortedArrayMap() { for(const auto& i : init) Add(i.first, i.second); }

#ifdef UPP
	void     Serialize(Stream& s);
	void     Xmlize(XmlIO& xio);
	void     Jsonize(JsonIO& jio);
	hash_t   GetHashValue() const                   { return HashBySerialize(*this); }
#endif

	void     Swap(SortedArrayMap& x);

	friend void    Swap(SortedArrayMap& a, SortedArrayMap& b) { a.Swap(b); }

	typedef typename Data::Type::ConstIterator  ConstIterator;
	typedef typename Data::Type::Iterator       Iterator;

	Iterator         begin()                        { return B::value.data.begin(); }
	Iterator         end()                          { return B::value.data.end(); }
	ConstIterator    begin() const                  { return B::value.data.begin(); }
	ConstIterator    end() const                    { return B::value.data.end(); }

	STL_SORTED_MAP_COMPATIBILITY(SortedArrayMap<K _cm_ T _cm_ HashFn>)

#ifdef DEPRECATED
	typedef T                                   ValueType;

	Iterator         GetIter(int pos)               { return B::value.data.GetIter(pos); }
	ConstIterator    GetIter(int pos) const         { return B::value.data.GetIter(pos); }
#endif
};
