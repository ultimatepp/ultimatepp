template <class T>
class InVector : public MoveableAndDeepCopyOption< InVector<T> > {
public:
	class ConstIterator;
	class Iterator;

private:	
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;
	int   count;
	int   hcount;
	int64 serial;
	int   blk_high;
	int   blk_low;

	void SetCache(int blki, int offset) const;
	void ClearCache() const;
	int  FindBlock0(int& pos, int& off) const;
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void Reindex();
	void SetBlkPar();

	template <class L>
	int  FindUpperBound(const T& val, const L& less, int& off, int& pos);

	template <class L>
	int  FindLowerBound(const T& val, const L& less, int& off, int& pos);

	bool JoinSmall(int blki);
	T   *Insert0(int ii, int blki, int pos, int off, const T *val);
	T   *Insert0(int ii, const T *val);
	
	void Reset();

	void SetIter(ConstIterator& it, int ii);
	void SetBegin(ConstIterator& it);
	void SetEnd(ConstIterator& it);

	void     Chk() const                            { ASSERT_(!IsPicked(), "Broken pick semantics"); }

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
	int FindUpperBound(const T& val, const L& less) { int off, pos; FindUpperBound(val, less, off, pos); return off + pos; }
	int FindUpperBound(const T& val)                { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) { int off, pos; FindLowerBound(val, less, off, pos); return off + pos; }
	int FindLowerBound(const T& val)                { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val)              { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less);
	int Find(const T& val)                          { return Find(val, StdLess<T>()); }

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
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

#ifdef _DEBUG
	void DumpIndex();
#endif
};

template <class T>
class InVector<T>::ConstIterator {
	T        *ptr;
	T        *begin;
	T        *end;
	InVector *v;
	int       offset;
	int       blki;

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

	T& operator*()                                 { return *B::ptr; }
	T *operator->()                                { return B::ptr; }
	T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

	const T& operator*() const                     { return *B::ptr; }
	const T *operator->() const                    { return B::ptr; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
};

template <class T>
class InArray : public MoveableAndDeepCopyOption< InVector<T> > {
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

	void     SetIter(ConstIterator& it, int ii);
	void     SetBegin(ConstIterator& it);
	void     SetEnd(ConstIterator& it);

	template <class L>
	struct ALess {
		const L& less;
		
		bool operator()(const PointerType& a, const PointerType& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

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

	void     Trim(int n)                            { Delete(n, GetCount() - n); }

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
	int FindUpperBound(const T& val, const L& less) { return iv.FindUpperBound((T*)&val, ALess<L>(less)); }
	int FindUpperBound(const T& val)                { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) { return iv.FindLowerBound((T*)&val, ALess<L>(less)); }
	int FindLowerBound(const T& val)                { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& lss){ return iv.InsertUpperBound(new T(val), ALess<L>(lss)); }
	int InsertUpperBound(const T& val)              { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less)           { return iv.Find((T*)&val, ALess<L>(less)); }
	int Find(const T& val)                          { return Find(val, StdLess<T>()); }

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
};

#define LLOG(x)   // DLOG(x)
#include "InVector.hpp"
#undef  LLOG
