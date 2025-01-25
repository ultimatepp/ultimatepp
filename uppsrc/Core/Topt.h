template <class I>
inline void IterSwap(I a, I b) { Swap(*a, *b); }

struct EmptyClass {};

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
{ memset8(t, x, size_t(lim - t)); }
inline void Copy(char *dst, const char *src, const char *lim)
{ memcpy8(dst, src, size_t((byte *)lim - (byte *)src)); }

inline void Fill(signed char *t, const signed char *lim, const signed char& x)
{ memset8(t, x, size_t(lim - t)); }
inline void Copy(signed char *dst, const signed char *src, const signed char *lim)
{ memcpy8(dst, src, size_t((byte *)lim - (byte *)src)); }

inline void Fill(unsigned char *t, const unsigned char *lim, const unsigned char& x)
{ memset8(t, x, size_t(lim - t)); }
inline void Copy(unsigned char *dst, const unsigned char *src, const unsigned char *lim)
{ memcpy8(dst, src, size_t((byte *)lim - (byte *)src)); }

template <class T>
inline void DeepCopyConstructFill(T *t, const T *end, const T& x) {
	while(t != end)
		new(t++) T(clone(x));
}

template <class T>
inline void Construct(T *t, const T *lim) {
	while(t < lim)
		new(t++) T;
}

template <class T>
inline void Destruct(T *t)
{
	t->~T();
}

template <class T>
inline void Destroy(T *t, const T *end)
{
	while(t != end)
		Destruct(t++);
}

template <class T>
struct TriviallyRelocatable {};

template <class T> // alternate name
struct Moveable : TriviallyRelocatable<T> {};

template <class T> // backward compatiblity
struct Moveable_ : TriviallyRelocatable<T> {};

template <class T>
inline constexpr bool is_trivially_relocatable = std::is_base_of_v<TriviallyRelocatable<T>, T> ||
                                                 std::is_trivially_copyable_v<T>;

template <class T>
inline constexpr bool is_upp_guest = false;

template <class T>
inline typename std::enable_if_t<is_trivially_relocatable<T>> Relocate(T *dst, T *src)
{
	memcpy(dst, src, sizeof(T));
}

template <class T>
inline typename std::enable_if_t<!is_trivially_relocatable<T>> Relocate(T *dst, T *src)
{
	static_assert(is_upp_guest<T>);
	new(dst) T(pick(*src));
	Destruct(src);
}

template <class T>
inline void InsertRelocate(T *dst, T *src, int n)
{
	if constexpr(is_trivially_relocatable<T>)
		memmove(dst, src, n * sizeof(T));
	else {
		static_assert(is_upp_guest<T>);
		if(n <= 0)
			return;
		dst += n - 1;
		T *s = src + n - 1;
		for(;;) {
			Relocate(dst, s);
			if(s == src) break;
			dst--;
			s--;
		}
	}
}

template <class T>
inline void RemoveRelocate(T *dst, T *src, int n)
{
	if constexpr(is_trivially_relocatable<T>)
		memmove(dst, src, n * sizeof(T));
	else {
		static_assert(is_upp_guest<T>);
		T *lim = src + n;
		while(src != lim)
			Relocate(dst++, src++);
	}
}

template <class T>
inline void Relocate(T *dst, T *src, int n)
{
	if constexpr(is_trivially_relocatable<T>)
		memcpy_t(dst, src, n);
	else {
		static_assert(is_upp_guest<T>);
		T *lim = src + n;
		while(src != lim)
			Relocate(dst++, src++);
	}
}

template <class T, class S>
inline void DeepCopyConstruct(T *t, const S *s, const S *end) {
	while(s != end)
		new (t++) T(clone(*s++));
}

template <class T, class B = EmptyClass>
class WithClone : public B {
public:
	friend T clone(const T& src) { T c(src, 1); return c; }
};

template <class T, class B = EmptyClass>
class DeepCopyOption : public B {
public:
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& src)
	{ if(&dest != &src) { (&dest)->~T(); ::new(&dest) T(src, 1); } return dest; }
#endif
	friend T  clone(const T& src) { T c(src, 1); return c; }
};

template <class T>
class MoveableAndDeepCopyOption : public TriviallyRelocatable<T> {
#ifdef DEPRECATED
	friend T& operator<<=(T& dest, const T& src)
	{ if(&dest != &src) { (&dest)->~T(); ::new(&dest) T(src, 1); } return dest; }
#endif
	friend T  clone(const T& src) { T c(src, 1); return c; }
};

template <class T>
class WithDeepCopy : public T {
public:
	WithDeepCopy(const T& a) : T(a, 1)                 {}
	WithDeepCopy(const T& a, int) : T(a, 1)            {}

	WithDeepCopy(const WithDeepCopy& a) : T(a, 1)      {}
	WithDeepCopy& operator=(const WithDeepCopy& a)     { (T&)*this = pick(T(a, 1)); return *this; }

	WithDeepCopy(T&& a) : T(pick(a))                   {}
	WithDeepCopy& operator=(T&& a)                     { (T&)*this = pick(a); return *this; }

	WithDeepCopy(WithDeepCopy&& a) : T(pick(a))        {}
	WithDeepCopy& operator=(WithDeepCopy&& a)          { (T&)*this = pick(a); return *this; }

	WithDeepCopy()                                     {}

#ifdef CPP_20
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
#endif
};

// compatibility hacks

#define STL_ITERATOR_COMPATIBILITY \
	typedef ptrdiff_t                        difference_type; \
    typedef std::random_access_iterator_tag  iterator_category; \
    typedef T                                value_type; \
    typedef T                                pointer; \
    typedef T                                reference; \

#define STL_BI_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        Begin() const          { return begin(); } \
	const_iterator        End() const            { return end(); } \
	void                  clear()                { Clear(); } \
	size_type             size() const           { return GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return IsEmpty(); } \
	iterator              Begin()                { return begin(); } \
	iterator              End()                  { return end(); } \

#define STL_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        Begin() const          { return B::begin(); } \
	const_iterator        End() const            { return B::end(); } \
	void                  clear()                { B::Clear(); } \
	size_type             size() const           { return B::GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return B::IsEmpty(); } \
	iterator              Begin()                { return B::begin(); } \
	iterator              End()                  { return B::end(); } \

#define STL_SORTED_MAP_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        Begin() const          { return begin(); } \
	const_iterator        End() const            { return end(); } \
	void                  clear()                { B::Clear(); } \
	size_type             size() const           { return B::GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	bool                  empty() const          { return B::GetCount() == 0; } \
	iterator              Begin()                { return begin(); } \
	iterator              End()                  { return end(); } \

#define STL_VECTOR_COMPATIBILITY(C) \
	typedef T             value_type; \
	typedef ConstIterator const_iterator; \
	typedef const T&      const_reference; \
	typedef int           size_type; \
	typedef int           difference_type; \
	const_iterator        Begin() const          { return begin(); } \
	const_iterator        End() const            { return end(); } \
	void                  clear()                { Clear(); } \
	size_type             size() const           { return GetCount(); } \
	typedef Iterator      iterator; \
	typedef T&            reference; \
	iterator              Begin()                { return begin(); } \
	iterator              End()                  { return end(); } \
	reference             front()                { return (*this)[0]; } \
	const_reference       front() const          { return (*this)[0]; } \
	reference             back()                 { return Top(); } \
	const_reference       back() const           { return Top(); } \
	bool                  empty() const          { return IsEmpty(); } \
	void                  push_back(const T& x)  { Add(x); } \
	void                  pop_back()             { Drop(); } \


template <class T>
T *DeclPtr__();

template <class Range>
using ValueTypeOfArray = typename std::remove_reference<decltype((*DeclPtr__<Range>())[0])>::type;

template <class V>
class ConstIIterator {
	typedef ValueTypeOfArray<V> T;

	const V       *cont;
	int            ii;
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

	ConstIIterator  operator+(int d) const { return ConstIIterator(*cont, ii + d); }
	ConstIIterator  operator-(int d) const { return ConstIIterator(*cont, ii - d); }

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

	STL_ITERATOR_COMPATIBILITY
};

template <class V>
class IIterator {
	typedef ValueTypeOfArray<V> T;

	V             *cont;
	int            ii;
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

	STL_ITERATOR_COMPATIBILITY
};

hash_t memhash(const void *ptr, size_t size);

template <class T>
inline hash_t GetHashValue(const T& x)                         { return x.GetHashValue(); }

template<> inline hash_t GetHashValue(const char& a)           { return (hash_t)a; }
template<> inline hash_t GetHashValue(const signed char& a)    { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const unsigned char& a)  { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const short& a)          { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const unsigned short& a) { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const int& a)            { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const unsigned int& a)   { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const long& a)           { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const unsigned long& a)  { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const bool& a)           { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const wchar_t& a)        { return (const hash_t)a; }

#ifdef HASH64
template<> inline hash_t GetHashValue(const int64& a)          { return (const hash_t)a; }
template<> inline hash_t GetHashValue(const uint64& a)         { return (const hash_t)a; }
#else
template<> inline hash_t GetHashValue(const int64& a)          { return HASH_CONST2 * (hash_t)a + (hash_t)(a >> 32); }
template<> inline hash_t GetHashValue(const uint64& a)         { return GetHashValue((int64)a); }
#endif

template<> inline hash_t GetHashValue(const double& a)         { return memhash(&a, sizeof(a)); }
template<> inline hash_t GetHashValue(const float& a)          { return memhash(&a, sizeof(a)); }

inline hash_t GetPtrHashValue(const void *a)                   { return (hash_t)(uintptr_t)a; }

template <class T>
inline hash_t GetHashValue(T *ptr)                             { return GetPtrHashValue(reinterpret_cast<const void *>(ptr)); }

struct CombineHash {
	hash_t hash;

	template <class T> CombineHash& Do(const T& x)                  { Put(GetHashValue(x)); return *this; }

public:
	CombineHash& Put(hash_t h)                                      { hash = HASH_CONST2 * hash + h; return *this; }

	operator hash_t() const                                         { return hash; }

	CombineHash()                                                   { hash = HASH_CONST1; }
	template <class T>
	CombineHash(const T& h1)                                        { hash = HASH_CONST1; Do(h1); }
	template <class T, class U>
	CombineHash(const T& h1, const U& h2)                           { hash = HASH_CONST1; Do(h1); Do(h2); }
	template <class T, class U, class V>
	CombineHash(const T& h1, const U& h2, const V& h3)              { hash = HASH_CONST1; Do(h1); Do(h2); Do(h3); }
	template <class T, class U, class V, class W>
	CombineHash(const T& h1, const U& h2, const V& h3, const W& h4)	{ hash = HASH_CONST1; Do(h1); Do(h2); Do(h3); Do(h4); }

	template <class T> CombineHash& operator<<(const T& x)          { Do(x); return *this; }
};

template <class C>
bool IsEqualMap(const C& a, const C& b)
{
	if(a.GetCount() != b.GetCount())
		return false;
	for(int i = 0; i < a.GetCount(); i++)
		if(a.GetKey(i) != b.GetKey(i) || a[i] != b[i])
			return false;
	return true;
}

template <class C>
int CompareMap(const C& a, const C& b)
{
	int n = min(a.GetCount(), b.GetCount());
	for(int i = 0; i < n; i++) {
		int q = SgnCompare(a.GetKey(i), b.GetKey(i));
		if(q)
			return q;
		q = SgnCompare(a[i], b[i]);
		if(q)
			return q;
	}
	return SgnCompare(a.GetCount(), b.GetCount());
}
