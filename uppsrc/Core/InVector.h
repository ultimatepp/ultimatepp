template <class T>
class InVector  : public MoveableAndDeepCopyOption< InVector<T> > {
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
	void     AddN(int n)                            { Insert(GetCount(), n); }
	
	int      GetCount() const                       { Chk(); return count; }
	bool     IsEmpty() const                        { return GetCount() == 0; }

	void     Trim(int n)                            { Remove(GetCount() - n); }
	void     SetCount(int n);
	void     Clear();

	T&       At(int i)                              { if(i >= items) SetCount(i + 1); return (*this)[i]; }

	void     Shrink();

	void     Set(int i, const T& x, int count);
	T&       Set(int i, const T& x)                 { Set(i, x, 1); return Get(i); }

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

	template <class L> int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val) { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L> int Find(const T& val, const L& less);
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

	friend class InVector;

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

#define LLOG(x)   // DLOG(x)
#include "InVector.hpp"
#undef  LLOG
