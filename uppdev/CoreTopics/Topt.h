class NoCopy {
private:
	NoCopy(const NoCopy&);
	void operator=(const NoCopy&);
public:
	NoCopy() {}
};

template <class I>
inline void IterSwap(I a, I b) { if(a != b) Swap(*a, *b); }

class EmptyClass
{
};

template <class T, class B = EmptyClass>
class RelOps : public B
{
public:
	friend bool operator >  (const T& a, const T& b)  { return b < a; }
	friend bool operator != (const T& a, const T& b)  { return !(a == b); }
	friend bool operator <= (const T& a, const T& b)  { return !(b < a); }
	friend bool operator >= (const T& a, const T& b)  { return !(a < b); }
};

template <class U, class V, class B = EmptyClass>
class AddOps : public B
{
public:
	friend U& operator -= (U& a, const V& b)          { a += -b; return a; }
	friend U  operator +  (const U& a, const V& b)    { U x(a); x += b; return x; }
	friend U  operator -  (const U& a, const V& b)    { U x(a); x += -b; return x; }
};

template <class T, class B = EmptyClass>
class PostfixOps : public B
{
public:
	friend T operator ++ (T& i, int)                  { T x = i; ++i; return x; }
	friend T operator -- (T& i, int)                  { T x = i; --i; return x; }
};

template <class T, int (*compare)(T a, T b), class B = EmptyClass>
class CompareRelOps : public B
{
public:
	friend bool operator <  (T a, T b) { return (*compare)(a, b) <  0; }
	friend bool operator >  (T a, T b) { return (*compare)(a, b) >  0; }
	friend bool operator == (T a, T b) { return (*compare)(a, b) == 0; }
	friend bool operator != (T a, T b) { return (*compare)(a, b) != 0; }
	friend bool operator <= (T a, T b) { return (*compare)(a, b) <= 0; }
	friend bool operator >= (T a, T b) { return (*compare)(a, b) >= 0; }
};

template <class T, class B = EmptyClass>
struct Comparable : public B
{
public:
	friend bool operator <  (const T& a, const T& b) { return a.Compare(b) < 0; }
	friend bool operator >  (const T& a, const T& b) { return a.Compare(b) > 0; }
	friend bool operator == (const T& a, const T& b) { return a.Compare(b) == 0; }
	friend bool operator != (const T& a, const T& b) { return a.Compare(b) != 0; }
	friend bool operator <= (const T& a, const T& b) { return a.Compare(b) <= 0; }
	friend bool operator >= (const T& a, const T& b) { return a.Compare(b) >= 0; }
	friend int  SgnCompare(const T& a, const T& b)   { return a.Compare(b); }
};

template <class T>
int NumberCompare__(const T& a, const T& b)
{
	if(a < b) return -1;
	if(a > b) return 1;
	return 0;
}

inline int SgnCompare(const char& a, const char& b)                     { return NumberCompare__(a, b); }
inline int SgnCompare(const signed char& a, const signed char& b)       { return NumberCompare__(a, b); }
inline int SgnCompare(const unsigned char& a, const unsigned char& b)   { return NumberCompare__(a, b); }
inline int SgnCompare(const short& a, const short& b)                   { return NumberCompare__(a, b); }
inline int SgnCompare(const unsigned short& a, const unsigned short& b) { return NumberCompare__(a, b); }
inline int SgnCompare(const int& a, const int& b)                       { return NumberCompare__(a, b); }
inline int SgnCompare(const unsigned int& a, const unsigned int& b)     { return NumberCompare__(a, b); }
inline int SgnCompare(const long& a, const long& b)                     { return NumberCompare__(a, b); }
inline int SgnCompare(const unsigned long& a, const unsigned long& b)   { return NumberCompare__(a, b); }
inline int SgnCompare(const bool& a, const bool& b)                     { return NumberCompare__(a, b); }
inline int SgnCompare(const int64& a, const int64& b)                   { return NumberCompare__(a, b); }
inline int SgnCompare(const uint64& a, const uint64& b)                 { return NumberCompare__(a, b); }
inline int SgnCompare(const float& a, const float& b)                   { return NumberCompare__(a, b); }
inline int SgnCompare(const double& a, const double& b)                 { return NumberCompare__(a, b); }

template<class T>
inline int SgnCompare(const T& a, const T& b)
{
	return a.Compare(b);
}

struct CombineCompare {
	int result;

	template <class T>
	CombineCompare& operator()(const T& a, const T& b) { if(!result) result = SgnCompare(a, b); return *this; }

	operator int() const                               { return result; }

	CombineCompare()                                   { result = 0; }

	template <class T>
	CombineCompare(const T& a, const T& b)             { result = 0; operator()(a, b); }
};

template <class T>
inline void Fill(T *dst, const T *lim, const T& x) {
	while(dst < lim)
		*dst++ = x;
}

template <class T>
inline void Copy(T *dst, const T *src, const T *lim) {
	while(src < lim)
		*dst++ = *src++;
}

template <class T>
inline void Copy(T *dst, const T *src, int n) {
	Copy(dst, src, src + n);
}

inline void Fill(char *t, const char *lim, const char& x)
{ memset(t, x, size_t(lim - t)); }
inline void Copy(char *dst, const char *src, const char *lim)
{ memcpy(dst, src, size_t((byte *)lim - (byte *)src)); }

inline void Fill(signed char *t, const signed char *lim, const signed char& x)
{ memset(t, x, size_t(lim - t)); }
inline void Copy(signed char *dst, const signed char *src, const signed char *lim)
{ memcpy(dst, src, size_t((byte *)lim - (byte *)src)); }

inline void Fill(unsigned char *t, const unsigned char *lim, const unsigned char& x)
{ memset(t, x, size_t(lim - t)); }
inline void Copy(unsigned char *dst, const unsigned char *src, const unsigned char *lim)
{ memcpy(dst, src, size_t((byte *)lim - (byte *)src)); }

template <class T>
inline void DeepCopyConstruct(void *p, const T& x) {
	::new(p) T(x);
}

template <class T>
inline T *DeepCopyNew(const T& x) {
	return new T(x);
}

template <class T>
inline void ConstructArray(T *t, const T *lim) {
	while(t < lim)
		::new(t++) T;
}

template <class T>
inline void DestroyArray(T *t, const T *lim) {
	while(t < lim) {
		t->T::~T();
		t++;
	}
}

template <class T>
inline void DeepCopyConstructArray(T *t, const T *s, const T *lim) {
	while(s < lim)
		DeepCopyConstruct(t++, *s++);
}

template <class T>
inline void DeepCopyConstructFill(T *t, const T *lim, const T& x) {
	while(t < lim)
		DeepCopyConstruct(t++, x);
}

#ifdef NO_MOVEABLE_CHECK

template <class T>
inline void AssertMoveable() {}

#define MoveableTemplate(T)

template <class T, class B = EmptyClass>
class Moveable : public B
{
public:
//	void operator=(const Moveable& b) { B::operator = (b); } // MSC 6.0 empty base class bug fix
};

#define NTL_MOVEABLE(T)

#else

template <class T>
inline void AssertMoveablePtr(T, T) {}

template <class T>
inline void AssertMoveable0(T *t) { AssertMoveablePtr(&**t, *t); }
// COMPILATION ERROR HERE MEANS TYPE T WAS NOT MARKED AS Moveable

template <class T, class B = EmptyClass>
struct Moveable : public B {
	friend void AssertMoveable0(T *) {}
//	void operator=(const Moveable& b) { B::operator = (b); } // MSC 6.0 empty base class bug fix
};

template <class T>
inline void AssertMoveable(T *t = 0) { if(t) AssertMoveable0(t); }

#if defined(COMPILER_MSC) || defined(COMPILER_GCC) && (__GNUC__ < 4 || __GNUC_MINOR__ < 1)
	#define NTL_MOVEABLE(T) inline void AssertMoveable0(T *) {}
#else
	#define NTL_MOVEABLE(T) template<> inline void AssertMoveable<T>(T *) {}
#endif

#endif

NTL_MOVEABLE(bool);
NTL_MOVEABLE(char);
NTL_MOVEABLE(signed char);
NTL_MOVEABLE(unsigned char);
NTL_MOVEABLE(short);
NTL_MOVEABLE(unsigned short);
NTL_MOVEABLE(int);
NTL_MOVEABLE(unsigned int);
NTL_MOVEABLE(long);
NTL_MOVEABLE(unsigned long);
NTL_MOVEABLE(int64);
NTL_MOVEABLE(uint64);
NTL_MOVEABLE(float);
NTL_MOVEABLE(double);
NTL_MOVEABLE(void *);
NTL_MOVEABLE(const void *);

#if defined(_NATIVE_WCHAR_T_DEFINED) || defined(COMPILER_GCC)
NTL_MOVEABLE(wchar_t);
#endif

template <class T, class B = EmptyClass>
class DeepCopyOption : public B {
public:
	friend T& operator<<=(T& dest, const T& src)
	{ if(&dest != &src) { (&dest)->T::~T(); ::new(&dest) T(src, 1); } return dest; }
	friend void DeepCopyConstruct(void *dest, const T& src)
	{ ::new (dest) T(src, 0); }
	friend T *DeepCopyNew(const T& src)
	{ return ::new T(src, 0); }
//	void operator=(const DeepCopyOption& b) { B::operator = (b); } // MSC 6.0 empty base class bug fix
};

template <class T, class B = EmptyClass>
class MoveableAndDeepCopyOption : public Moveable< T, DeepCopyOption<T, B> > {
public:
//	void operator=(const MoveableAndDeepCopyOption& b) { B::operator = (b); } // MSC 6.0 empty base class bug fix
};

template <class T, class B = EmptyClass>
class PolyDeepCopyNew : public B
{
public:
	friend T *DeepCopyNew(const T& t)              { return t.Copy(); }
//	void operator=(const PolyDeepCopyNew& b)       { B::operator = (b); } // MSC 6.0 empty base class bug fix
};

template <class T>
class WithDeepCopy : public T {
public:
	WithDeepCopy(const T& a) : T(a, 1)             {}
	WithDeepCopy(const WithDeepCopy& a) : T(a, 1)  {}
	WithDeepCopy& operator=(const WithDeepCopy& a) { (T&)*this <<= a; return *this; }
	WithDeepCopy(int, pick_ T& a) : T(a)           {}
	WithDeepCopy& operator^=(pick_ T& a)           { (T&)*this = a; return *this; }
	WithDeepCopy()                                 {}
};

template <class V>
class ConstIIterator {
protected:
	const V       *cont;
	int            ii;
	typedef        typename V::ValueType T;
	struct NP { int dummy; };

public:
	const T&       operator*() const       { return (*cont)[ii]; }
	const T       *operator->() const      { return &(*cont)[ii]; }
	const T&       operator[](int i) const { return (*cont)[ii + i]; }

	ConstIIterator& operator++()           { ++ii; return *this; }
	ConstIIterator& operator--()           { --ii; return *this; }
	ConstIIterator  operator++(int)        { ConstIIterator t = *this; ++ii; return t; }
	ConstIIterator  operator--(int)        { ConstIIterator t = *this; --ii; return t; }

	ConstIIterator& operator+=(int d)      { ii += d; return *this; }
	ConstIIterator& operator-=(int d)      { ii -= d; return *this; }

	ConstIIterator  operator+(int d) const { return ConstIIterator(cont, ii + d); }
	ConstIIterator  operator-(int d) const { return ConstIIterator(cont, ii - d); }

	int  operator-(const ConstIIterator& b) const   { return ii - b.ii; }

	bool operator==(const ConstIIterator& b) const  { return ii == b.ii; }
	bool operator!=(const ConstIIterator& b) const  { return ii != b.ii; }
	bool operator<(const ConstIIterator& b) const   { return ii < b.ii; }
	bool operator>(const ConstIIterator& b) const   { return ii > b.ii; }
	bool operator<=(const ConstIIterator& b) const  { return ii <= b.ii; }
	bool operator>=(const ConstIIterator& b) const  { return ii >= b.ii; }

	operator bool() const     { return ii < 0; }

	ConstIIterator()          {}
	ConstIIterator(NP *null)  { ASSERT(null == NULL); ii = -1; }
	ConstIIterator(const V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template <class V>
class IIterator {
protected:
	V             *cont;
	int            ii;
	typedef        typename V::ValueType T;
	struct NP { int dummy; };

public:
	T&       operator*()              { return (*cont)[ii]; }
	T       *operator->()             { return &(*cont)[ii]; }
	T&       operator[](int i)        { return (*cont)[ii + i]; }

	const T& operator*() const        { return (*cont)[ii]; }
	const T *operator->() const       { return &(*cont)[ii]; }
	const T& operator[](int i) const  { return (*cont)[ii + i]; }

	IIterator& operator++()           { ++ii; return *this; }
	IIterator& operator--()           { --ii; return *this; }
	IIterator  operator++(int)        { IIterator t = *this; ++ii; return t; }
	IIterator  operator--(int)        { IIterator t = *this; --ii; return t; }

	IIterator& operator+=(int d)      { ii += d; return *this; }
	IIterator& operator-=(int d)      { ii -= d; return *this; }

	IIterator  operator+(int d) const { return IIterator(*cont, ii + d); }
	IIterator  operator-(int d) const { return IIterator(*cont, ii - d); }

	int  operator-(const IIterator& b) const   { return ii - b.ii; }

	bool operator==(const IIterator& b) const  { return ii == b.ii; }
	bool operator!=(const IIterator& b) const  { return ii != b.ii; }
	bool operator<(const IIterator& b) const   { return ii < b.ii; }
	bool operator>(const IIterator& b) const   { return ii > b.ii; }
	bool operator<=(const IIterator& b) const  { return ii <= b.ii; }
	bool operator>=(const IIterator& b) const  { return ii >= b.ii; }

	operator bool() const                      { return ii < 0; }

	IIterator()          {}
	IIterator(NP *null)  { ASSERT(null == NULL); ii = -1; }
	IIterator(V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

unsigned Pow2Bound(unsigned i);
unsigned PrimeBound(unsigned i);

unsigned memhash(const void *ptr, size_t size);

template <class T>
inline unsigned GetHashValue(const T& x)                            { return x.GetHashValue(); }

struct CombineHash {
	unsigned hash;

	enum { INIT = 1234567890 };

	template <class T> CombineHash& Do(const T& x)                  { Put(GetHashValue(x)); return *this; }

public:
	CombineHash& Put(unsigned h) { hash = ((hash << 4) + hash) ^ h; return *this; }

	operator unsigned() const                                       { return hash; }

	CombineHash()                                                   { hash = INIT; }
	template <class T>
	CombineHash(const T& h1)                                        { hash = INIT; Do(h1); }
	template <class T, class U>
	CombineHash(const T& h1, const U& h2)                           { hash = INIT; Do(h1); Do(h2); }
	template <class T, class U, class V>
	CombineHash(const T& h1, const U& h2, const V& h3)              { hash = INIT; Do(h1); Do(h2); Do(h3); }
	template <class T, class U, class V, class W>
	CombineHash(const T& h1, const U& h2, const V& h3, const W& h4)	{ hash = INIT; Do(h1); Do(h2); Do(h3); Do(h4); }

	template <class T> CombineHash& operator<<(const T& x)          { Do(x); return *this; }
};

template<> inline unsigned GetHashValue(const char& a)           { return (unsigned)a; }
template<> inline unsigned GetHashValue(const signed char& a)    { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const unsigned char& a)  { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const short& a)          { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const unsigned short& a) { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const int& a)            { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const unsigned int& a)   { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const long& a)           { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const unsigned long& a)  { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const bool& a)           { return (const unsigned)a; }
template<> inline unsigned GetHashValue(const wchar_t& a)        { return (const unsigned)a; }

template<> inline unsigned GetHashValue(const int64& a)          { return CombineHash((unsigned)a, (unsigned)(a >> 32)); }
template<> inline unsigned GetHashValue(const uint64& a)         { return GetHashValue((int64)a); }

unsigned GetHashValue0(const double& a);

template<> inline unsigned GetHashValue(const double& a)         { return GetHashValue0(a); }
template<> inline unsigned GetHashValue(const float& a)          { double x = a; return GetHashValue0(x); }

#ifdef CPU_32
inline unsigned GetPtrHashValue(const void *a)                   { return (int)a; }
#else
inline unsigned GetPtrHashValue(const void *a)                   { return CombineHash((unsigned)(uintptr_t)a); }
#endif

/* Is it time to activate this?
template <class T>
inline unsigned GetHashValue(T *ptr)                             { return GetPtrHashValue(ptr); }
*/

// workaround for broken standard libraries...

template <class T> inline const T& ntl_max(const T& a, const T& b) { return a > b ? a : b; }

// STL compatibility hacks

#define STL_INDEX_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        begin() const          { return B::Begin(); } \
	const_iterator        end() const            { return B::End(); } \
	void                  clear()                { B::Clear(); } \
	size_type             size()                 { return B::GetCount(); } \

#define STL_BI_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        begin() const          { return Begin(); } \
	const_iterator        end() const            { return End(); } \
	void                  clear()                { Clear(); } \
	size_type             size()                 { return GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	iterator              begin()                { return Begin(); } \
	iterator              end()                  { return End(); } \

#define STL_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        begin() const          { return B::Begin(); } \
	const_iterator        end() const            { return B::End(); } \
	void                  clear()                { B::Clear(); } \
	size_type             size()                 { return B::GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	iterator              begin()                { return B::Begin(); } \
	iterator              end()                  { return B::End(); } \

#define STL_VECTOR_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        begin() const          { return Begin(); } \
	const_iterator        end() const            { return End(); } \
	void                  clear()                { Clear(); } \
	size_type             size()                 { return GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	iterator              begin()                { return Begin(); } \
	iterator              end()                  { return End(); } \
	reference             front()                { return (*this)[0]; } \
	const_reference       front() const          { return (*this)[0]; } \
	reference             back()                 { return Top(); } \
	const_reference       back() const           { return Top(); } \
	void                  push_back(const T& x)  { Add(x); } \
	void                  pop_back()             { Drop(); }
