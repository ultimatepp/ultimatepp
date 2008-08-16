class Nuller;

#if defined(CPU_X86) && (defined(COMPILER_MSC) || defined(COMPILER_GCC))
#define FAST_STRING_COMPARE
#endif

int wstrlen(const wchar *s);

#ifdef PLATFORM_POSIX
inline int stricmp(const char *a, const char *b)         { return strcasecmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return strncasecmp(a, b, n); }
#endif

#ifdef PLATFORM_WINCE
inline int stricmp(const char *a, const char *b)         { return _stricmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return _strnicmp(a, b, n); }
#endif

inline int strlen__(const char *s)  { return s ? (int)strlen(s) : 0; }
inline int strlen__(const wchar *s) { return s ? (int)wstrlen(s) : 0; }

inline int cmpval__(char x)         { return (byte)x; }
inline int cmpval__(wchar x)        { return (word)x; }

class String;
class WString;
class StringBuffer;
class WStringBuffer;

template <class B>
class AString : public B {
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Buffer buffer;
	typedef typename B::String String;

public:
	void Clear()                                              { B::Free(); B::Zero(); }
	int  GetLength() const                                    { return B::GetCount(); }
	bool IsEmpty() const                                      { return B::GetCount() == 0; }

	const tchar *End() const                                  { return B::Begin() + GetLength(); }
	const tchar *Last() const                                 { return End() - !!B::GetCount(); }
	const tchar *GetIter(int i) const                         { ASSERT(i >= 0 && i <= B::GetCount()); return B::Begin() + i; }

	int operator*() const                                     { return *B::Begin(); }
	int operator[](int i) const                               { ASSERT(i >= 0 && i <= B::GetCount()); return B::Begin()[i]; }

	operator const tchar *() const                            { return B::Begin(); }
	const tchar *operator~() const                            { return B::Begin(); }
	operator const bchar *() const                            { return (bchar *)B::Begin(); }
	operator const void *() const                             { return B::Begin(); }

	void Insert(int pos, int c)                               { *B::Insert(pos, 1, NULL) = c; }
	void Insert(int pos, const tchar *s, int count)           { B::Insert(pos, count, s); }
	void Insert(int pos, const String& s)                     { Insert(pos, s, s.GetCount()); }
	void Insert(int pos, const char *s)                       { Insert(pos, s, strlen__(s)); }

	void  Cat(int c)                                          { B::Cat(c); }
	void  Cat(const tchar *s, int len)                        { B::Cat(s, len); }
	void  Cat(const tchar *s)                                 { Cat(s, strlen__(s)); }
	void  Cat(const String& s)                                { Cat(~s, s.GetLength()); }
	void  Cat(int c, int count);
	void  Cat(const tchar *s, const tchar *lim)               { ASSERT(s <= lim); Cat(s, int(lim - s)); }
	void  Cat(const String& s, int len)                       { B::Cat(~s, len); }
	void  Cat(const bchar *s, int len)                        { Cat((const tchar *) s, len); }

	String& Cat()                                             { return *(String *)this; }

	int    Compare(const String& s) const                     { return B::Compare(s); }
	int    Compare(const tchar *s) const;

	bool   IsEqual(const String& s) const                     { return B::IsEqual(s); }
	bool   IsEqual(const tchar *s) const                      { return Compare(s) == 0; }

	String Mid(int pos, int length) const;
	String Mid(int pos) const                                 { return Mid(pos, GetLength() - pos); }
	String Right(int count) const                             { return Mid(GetLength() - count); }
	String Left(int count) const                              { return Mid(0, count); }

	int    Find(int chr, int from = 0) const;
	int    ReverseFind(int chr, int from) const;
	int    ReverseFind(int chr) const;

	int    Find(int len, const tchar *s, int from) const;
	int    Find(const tchar *s, int from = 0) const;
	int    Find(const String& s, int from = 0) const          { return Find(s.GetCount(), ~s, from); }

	bool   StartsWith(const tchar *s, int len) const;
	bool   StartsWith(const tchar *s) const                   { return StartsWith(s, strlen__(s)); }
	bool   StartsWith(const String& s) const                  { return StartsWith(~s, s.GetLength()); }

	bool   EndsWith(const tchar *s, int len) const;
	bool   EndsWith(const tchar *s) const                     { return EndsWith(s, strlen__(s)); }
	bool   EndsWith(const String& s) const                    { return EndsWith(~s, s.GetLength()); }

	int    FindFirstOf(int len, const tchar *s, int from) const;
	int    FindFirstOf(const tchar *s, int from = 0) const;
	int    FindFirstOf(const String& s, int from = 0) const   { return FindFirstOf(s.GetCount(), ~s, from); }

	friend bool operator<(const String& a, const String& b)   { return a.Compare(b) < 0; }
	friend bool operator<(const String& a, const tchar *b)    { return a.Compare(b) < 0; }
	friend bool operator<(const tchar *a, const String& b)    { return b.Compare(a) > 0; }

	friend bool operator<=(const String& a, const String& b)  { return a.Compare(b) <= 0; }
	friend bool operator<=(const String& a, const tchar *b)   { return a.Compare(b) <= 0; }
	friend bool operator<=(const tchar *a, const String& b)   { return b.Compare(a) >= 0; }

	friend bool operator>(const String& a, const String& b)   { return a.Compare(b) > 0; }
	friend bool operator>(const String& a, const tchar *b)    { return a.Compare(b) > 0; }
	friend bool operator>(const tchar *a, const String& b)    { return b.Compare(a) < 0; }

	friend bool operator>=(const String& a, const String& b)   { return a.Compare(b) >= 0; }
	friend bool operator>=(const String& a, const tchar *b)    { return a.Compare(b) >= 0; }
	friend bool operator>=(const tchar *a, const String& b)    { return b.Compare(a) <= 0; }

	friend bool operator==(const String& a, const String& b)   { return a.IsEqual(b); }
	friend bool operator!=(const String& a, const String& b)   { return !a.IsEqual(b); }
	friend bool operator==(const String& a, const tchar *b)    { return a.Compare(b) == 0; }
	friend bool operator==(const tchar *a, const String& b)    { return b.Compare(a) == 0; }
	friend bool operator!=(const String& a, const tchar *b)    { return a.Compare(b) != 0; }
	friend bool operator!=(const tchar *a, const String& b)    { return b.Compare(a) != 0; }

	friend String operator+(const String& a, const String& b)  { String c(a); c += b; return c; }
	friend String operator+(const String& a, const tchar *b)   { String c(a); c += b; return c; }
	friend String operator+(const tchar *a, const String& b)   { String c(a); c += b; return c; }
	friend String operator+(const String& a, int b)            { String c(a); c += b; return c; }
	friend String operator+(int a, const String& b)            { String c(a, 1); c += b; return c; }
	friend String operator+(const String& a, tchar b)          { String c(a); c += b; return c; }
	friend String operator+(tchar a, const String& b)          { String c(a, 1); c += b; return c; }
};

class String0 : Moveable<String0> {
	enum { SMALL, MEDIUM = 31 };
	enum { KIND = 14, SLEN = 15, LLEN = 2 };

#if defined(_DEBUG) && defined(COMPILER_GCC)
	int          len;
	const char  *s;
#endif

	struct Rc {
		Atomic refcount;
		int    alloc;

		char *GetPtr() const  { return (char*)(this + 1); }
		void  Release();
		void  Retain();
	};

	union {
		char   chr[16];
		char  *ptr;
		dword *wptr;
		qword *qptr;
		word   v[8];
		dword  w[4];
		qword  q[2];
	};

#ifdef _DEBUG
	void Dsyn();
#else
	void Dsyn() {}
#endif

	char&  SLen()                { return chr[SLEN]; }
	char   SLen() const          { return chr[SLEN]; }
	dword& LLen()                { return w[LLEN]; }
	dword  LLen() const          { return w[LLEN]; }
	bool   IsSmall() const       { return chr[KIND] == SMALL; }
	bool   IsLarge() const       { return chr[KIND] != SMALL; }
	bool   IsMedium() const      { return chr[KIND] == MEDIUM; }
	bool   IsRef() const         { return (byte)chr[KIND] > MEDIUM; }
	Rc    *Ref() const           { return (Rc *)ptr - 1; }
	bool   IsShared() const      { return Ref()->refcount != 1; }
	bool   IsSharedRef() const   { return IsRef() && IsShared(); }
	int    LAlloc() const        { int b = (byte)chr[KIND]; return b == 255 ? Ref()->alloc : b; }
	dword  LEqual(const String0& s) const;
	int    LCompare(const String0& s) const;

	void LSet(const String0& s);
	void LFree();
	void LCat(int c);
	unsigned LHashValue() const;

	void UnShare();
	void SetSLen(int l);

	char *Ptr() { return IsSmall() ? chr : ptr; }
	char *Alloc(int count, char& kind);

	static String0::Rc voidptr[2];

	void Swap(String0& b)                           { UPP::Swap(q[0], b.q[0]); UPP::Swap(q[1], b.q[1]); Dsyn(); b.Dsyn(); }

	friend class String;
	friend class StringBuffer;

protected:
	void Zero()                  { q[0] = q[1] = 0; Dsyn(); }
	void Free()                  { if(IsLarge()) LFree(); }
	void Set(const String0& s) {
		if(s.IsSmall()) { q[0] = s.q[0]; q[1] = s.q[1]; }
		else LSet(s);
		Dsyn();
	}
	void Assign(const String0& s) {
		if(s.IsSmall()) {
			if(IsLarge()) LFree();
			q[0] = s.q[0]; q[1] = s.q[1];
		}
		else
			if(this != &s) {
				Free();
				LSet(s);
			}
		Dsyn();
	}
	void  Set(const char *s, int len);
	char *Insert(int pos, int count, const char *str);

public: // should be protected, bug in gcc 3.4
	typedef char         tchar;
	typedef byte         bchar;
	typedef StringBuffer Buffer;
	typedef Upp::String  String;

public:
	bool IsEqual(const String0& s) const {
		return (chr[KIND] | s.chr[KIND] ? LEqual(s) :
		#ifdef CPU_64
		        ((q[0] ^ s.q[0]) | (q[1] ^ s.q[1]))
		#else
		        ((w[0] ^ s.w[0]) | (w[1] ^ s.w[1]) | (w[2] ^ s.w[2]) | (w[3] ^ s.w[3]))
		#endif
		       ) == 0;
	}

	int    Compare(const String0& s) const;

	unsigned GetHashValue() const {
		return chr[KIND] ? LHashValue() : (unsigned)CombineHash(w[0], w[1], w[2], w[3]);
	}

	void Cat(int c) {
		if(SLen() < 14)
			chr[int(SLen()++)] = c;
		else
			LCat(c);
		Dsyn();
	}

	void Cat(const char *s, int len);

	void Set(int i, int chr);
	void Trim(int pos);

	const char *Begin() const   { return IsSmall() ? chr : ptr; }

	int operator[](int i) const { ASSERT(i >= 0 && i <= GetCount()); return Begin()[i]; }

	operator const char *() const   { return Begin(); }
	const char *operator~() const   { return Begin(); }

	void Remove(int pos, int count = 1);
	void Clear()                { Free(); Zero(); }

	int GetCount() const        { return IsSmall() ? chr[SLEN] : w[LLEN]; }
	int GetLength() const       { return GetCount(); }
	int GetAlloc() const        { return IsSmall() ? 14 : LAlloc(); }

	void Reserve(int r);

	String0& operator=(const String0& s) { Free(); Set(s); return *this; }

	String0()                       { Zero(); }
	~String0()                      { Free(); }
};

class String : public Moveable<String, AString<String0> > {
	void Swap(String& b)                                   { String0::Swap(b); }

#ifdef _DEBUG
#ifndef COMPILER_GCC
	int          len;
	const char  *s;
#endif
	friend class String0;
#endif

public:
	const String& operator+=(char c)                       { Cat(c); return *this; }
	const String& operator+=(const char *s)                { Cat(s); return *this; }
	const String& operator+=(const String& s)              { Cat(s); return *this; }

	String& operator=(const char *s);
	String& operator=(const String& s)                     { String0::Assign(s); return *this; }
	String& operator=(StringBuffer& b)                     { *this = String(b); return *this; }
	String& operator<<=(const String& s)                   { if(this != &s) { String0::Free(); String0::Set(s, s.GetCount()); } return *this; }

	void   Shrink()                                        { *this = String(Begin(), GetLength()); }

	String()                                               {}
	String(const Nuller&)                                  {}
	String(const String& s)                                { String0::Set(s); }
	String(const char *s)                                  { String0::Set(s, strlen__(s)); }
	String(const String& s, int n)                         { ASSERT(n >= 0 && n <= s.GetLength()); String0::Set(~s, n); }
	String(const char *s, int n)                           { String0::Set(s, n); }
	String(const byte *s, int n)                           { String0::Set((const char *)s, n); }
	String(const char *s, const char *lim)                 { String0::Set(s, (int)(lim - s)); }
	String(int chr, int count)                             { String0::Zero(); Cat(chr, count); }
	String(StringBuffer& b);

	WString ToWString() const;
	const String& ToString() const                         { return *this; }

	static String GetVoid();
	bool   IsVoid() const;

	friend void Swap(String& a, String& b)                 { a.Swap(b); }

	String(const std::string& s)                           { String0::Set(s.c_str(), (int)s.length()); }
	operator std::string() const                           { return std::string(Begin(), End()); }
};

inline std::string to_string(const String& s)              { return std::string(s.Begin(), s.End()); }

class StringBuffer : NoCopy {
	char   *begin;
	char   *end;
	char   *limit;
	char    buffer[256];

	friend class String;

	typedef String0::Rc Rc;

	char *Alloc(int len, int& alloc);
	void  Realloc(int n, const char *cat = NULL, int l = 0);
	void  Expand();
	void  Zero()                    { begin = end = buffer; limit = begin + 255; }
	void  Free();
	void  Set(String& s);

public:
	char *Begin()                   { *end = '\0'; return begin; }
	char *End()                     { *end = '\0'; return end; }

	operator char*()                { return Begin(); }
	char *operator~()               { return Begin(); }

	void SetLength(int l);
	void SetCount(int l)            { SetLength(l); }
	int  GetLength() const          { return (int)(end - begin); }
	int  GetCount() const           { return GetLength(); }
	void Strlen()                   { SetLength((int)strlen(begin)); }
	void Clear()                    { Free(); Zero(); }
	void Reserve(int r)             { int l = GetLength(); SetLength(l + r); SetLength(l); }

	void Cat(int c)                 { if(end >= limit) Expand(); *end++ = c; }
	void Cat(int c, int count);
	void Cat(const char *s, int l);
	void Cat(const char *s)         { Cat(s, (int)strlen(s)); }
	void Cat(const String& s)       { Cat(s, s.GetLength()); }

	int  GetAlloc() const           { return (int)(limit - begin); }

	void operator=(String& s)       { Free(); Set(s); }

	StringBuffer()                  { Zero(); }
	StringBuffer(String& s)         { Set(s); }
	StringBuffer(int len)           { Zero(); SetLength(len); }
	~StringBuffer()                 { if(begin != buffer) Free(); }
};

inline bool  IsEmpty(const String& s)      { return s.IsEmpty(); }

template <class T> bool IsNull(const T& x) { return x.IsNullInstance(); }

String FormatPtr(const void *p);

template <class T>
inline String AsString(const T& x)
{
	return x.ToString();
}

/*
template <class T>
inline String AsString(T *x)
{
	return FormatPtr(x);
}
*/

#ifdef PLATFORM_MSC
inline String AsString(const void *x)
{
	return FormatPtr(x);
}
#endif

inline String& operator<<(String& s, const char *x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, char *x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, const String &x)
{
	s.Cat(x);
	return s;
}

inline String& operator<<(String& s, char x)
{
	s.Cat((int) x);
	return s;
}

inline String& operator<<(String& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline String& operator<<(String& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline String& operator<<(String& s, const T& x)
{
	s.Cat(AsString(x));
	return s;
}

template<>
inline String& operator<<(String& s, const char * const &x)
{
	s.Cat(x);
	return s;
}

template<>
inline String& operator<<(String& s, const String &x)
{
	s.Cat(x);
	return s;
}

template<>
inline String& operator<<(String& s, const char& x)
{
	s.Cat(x);
	return s;
}

template<>
inline bool  IsNull(const String& s)       { return s.IsEmpty(); }

template<>
inline String AsString(const String& s)     { return s; }

template<>
inline unsigned GetHashValue(const String& s) { return memhash(~s, s.GetLength()); }

int CompareNoCase(const String& a, const String& b, byte encoding = 0);
int CompareNoCase(const String& a, const char *b, byte encoding = 0);

inline
int CompareNoCase(const char *a, const String& b, byte encoding = 0) {
	return -CompareNoCase(b, a, encoding);
}

String TrimLeft(const String& s);
String TrimRight(const String& s);

inline StringBuffer& operator<<(StringBuffer& s, const char *x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, char *x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, const String &x)
{
	s.Cat(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, char x)
{
	s.Cat((int) x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline StringBuffer& operator<<(StringBuffer& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline StringBuffer& operator<<(StringBuffer& s, const T& x)
{
	s.Cat(AsString(x));
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const char * const &x)
{
	s.Cat(x);
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const String &x)
{
	s.Cat(x);
	return s;
}

template<>
inline StringBuffer& operator<<(StringBuffer& s, const char& x)
{
	s.Cat(x);
	return s;
}

template <class T>
void RawCat(String& s, const T& x)
{
	s.Cat((const char *)&x, sizeof(x));
}

template <class T>
void RawCat(StringBuffer& s, const T& x)
{
	s.Cat((const char *)&x, sizeof(x));
}

class WString0 {
	enum { SMALL = 23 };

	wchar *ptr;
	int    length;
	int    alloc;

#ifdef _DEBUG
	void Dsyn();
#else
	void Dsyn() {}
#endif

	static Atomic voidptr[2];

	bool    IsRc() const  { return alloc > SMALL; }
	Atomic& Rc()          { return *((Atomic *)ptr - 1); }
	bool    IsShared()    { return IsRc() && Rc() > 1; }

	wchar  *Alloc(int& count);
	void    LCat(int c);
	void    UnShare();

	friend class WStringBuffer;
	friend class WString;

public: // should be protected, bug in GCC 3.4
	typedef wchar         tchar;
	typedef int16         bchar;
	typedef WStringBuffer Buffer;
	typedef WString       String;

protected:
	void    Zero()                       { static wchar e[2]; length = alloc = 0; ptr = e; Dsyn(); ASSERT(*ptr == 0); }
	void    Set(const wchar *s, int length);
	void    Set(const WString0& s);
	void    Free();
	void    Swap(WString0& b)            { Upp::Swap(ptr, b.ptr); Upp::Swap(length, b.length); Upp::Swap(alloc, b.alloc); Dsyn(); b.Dsyn(); }
	wchar  *Insert(int pos, int count, const wchar *data);

public:
	const wchar *Begin() const           { return ptr; }
	int   operator[](int i) const        { return ptr[i]; }

	operator const wchar *() const       { return Begin(); }
	const wchar *operator~() const       { return Begin(); }

	void Cat(int c)                      { if(!IsRc() && length < alloc) { ptr[length++] = c; ptr[length] = 0; } else LCat(c); Dsyn(); }
	void Cat(const wchar *s, int length);

	int  GetCount() const                { return length; }
	int  GetLength() const               { return length; }
	int  GetAlloc() const                { return alloc; }

	unsigned GetHashValue() const             { return memhash(ptr, length * sizeof(wchar)); }
	bool     IsEqual(const WString0& s) const { return s.length == length && memcmp(ptr, s.ptr, length * sizeof(wchar)) == 0; }
	int      Compare(const WString0& s) const;

	void Remove(int pos, int count = 1);
	void Insert(int pos, const wchar *s, int count);
	void Clear()                         { Free(); Zero(); }

	void Set(int pos, int ch);
	void Trim(int pos);

	WString0()                           { Zero(); }
	~WString0()                          { Free(); }
};

class WString : public Moveable<WString, AString<WString0> >
{
	void Swap(WString& b)                                   { WString0::Swap(b); }

#ifdef _DEBUG
	int          len;
	const wchar *s;
	friend class WString0;
#endif

public:
	UPP::String ToString() const;

	const WString& operator+=(wchar c)                      { Cat(c); return *this; }
	const WString& operator+=(const wchar *s)               { Cat(s); return *this; }
	const WString& operator+=(const WString& s)             { Cat(s); return *this; }

	WString& operator<<(wchar c)                            { Cat(c); return *this; }
	WString& operator<<(const WString& s)                   { Cat(s); return *this; }
	WString& operator<<(const wchar *s)                     { Cat(s); return *this; }

	WString& operator=(const wchar *s);
	WString& operator=(const WString& s)                    { if(this != &s) { WString0::Free(); WString0::Set(s); } return *this; }
	WString& operator=(WStringBuffer& b)                    { *this = WString(b); return *this; }
	WString& operator<<=(const WString& s)                  { if(this != &s) { WString0::Free(); WString0::Set(s, s.GetCount()); } return *this; }

	void   Shrink()                                         { *this = WString(Begin(), GetLength()); }

	WString()                                               {}
	WString(const Nuller&)                                  {}
	WString(const WString& s)                               { WString0::Set(s); }
	WString(const wchar *s)                                 { WString0::Set(s, strlen__(s)); }
	WString(const WString& s, int n)                        { ASSERT(n >= 0 && n <= s.GetLength()); WString0::Set(~s, n); }
	WString(const wchar *s, int n)                          { WString0::Set(s, n); }
	WString(const wchar *s, const wchar *lim)               { WString0::Set(s, (int)(lim - s)); }
	WString(int chr, int count)                             { WString0::Zero(); Cat(chr, count); }
	WString(WStringBuffer& b);

	WString(const char *s);
	WString(const char *s, int n);
	WString(const char *s, const char *lim);

	static WString GetVoid();
	bool   IsVoid() const                                   { return alloc < 0; }

	friend void Swap(WString& a, WString& b)                { a.Swap(b); }
	friend WString operator+(const WString& a, char b)      { WString c(a); c += b; return c; }
	friend WString operator+(char a, const WString& b)      { WString c(a, 1); c += b; return c; }

	WString(const std::wstring& s);
	operator std::wstring() const;
};

inline std::wstring to_string(const WString& s)             { return std::wstring(s.Begin(), s.End()); }

class WStringBuffer : NoCopy {
	wchar   *begin;
	wchar   *end;
	wchar   *limit;

	friend class WString;

	wchar *Alloc(int len, int& alloc);
	void   Expand(int n, const wchar *cat = NULL, int l = 0);
	void   Expand();
	void   Zero();
	void   Free();
	void   Set(WString& s);

public:
	wchar *Begin()                   { *end = '\0'; return begin; }
	wchar *End()                     { *end = '\0'; return end; }

	operator wchar*()                { return Begin(); }
	wchar *operator~()               { return Begin(); }

	void  SetLength(int l);
	void  SetCount(int l)            { SetLength(l); }
	int   GetLength() const          { return (int)(end - begin); }
	int   GetCount() const           { return GetLength(); }
	void  Strlen()                   { SetLength(wstrlen(begin)); }
	void  Clear()                    { Free(); Zero(); }
	void  Reserve(int r)             { int l = GetLength(); SetLength(l + r); SetLength(l); }

	void  Cat(int c)                 { if(end >= limit) Expand(); *end++ = c; }
	void  Cat(int c, int count);
	void  Cat(const wchar *s, int l);
	void  Cat(const wchar *s)        { Cat(s, wstrlen(s)); }
	void  Cat(const WString& s)      { Cat(s, s.GetLength()); }
	void  Cat(const char *s)         { Cat(WString(s)); }

	int   GetAlloc() const           { return (int)(limit - begin); }

	void operator=(WString& s)       { Free(); Set(s); }

	WStringBuffer()                  { Zero(); }
	WStringBuffer(WString& s)        { Set(s); }
	WStringBuffer(int len)           { Zero(); SetLength(len); }
	~WStringBuffer()                 { Free(); }
};

inline bool  IsEmpty(const WString& s)      { return s.IsEmpty(); }

template<>
inline bool  IsNull(const WString& s)       { return s.IsEmpty(); }

//template<>
//inline String AsString(const WString& s)     { return s; }

template<>
inline unsigned GetHashValue(const WString& s) { return memhash(~s, 2 * s.GetLength()); }

WString TrimLeft(const WString& str);
WString TrimRight(const WString& s);

int CompareNoCase(const WString& a, const WString& b);
int CompareNoCase(const WString& a, const wchar *b);

inline
int CompareNoCase(const wchar *a, const WString& b) {
	return -CompareNoCase(b, a);
}

template<> inline String AsString(const char * const &s)    { return s; }
template<> inline String AsString(char * const &s)          { return s; }
template<> inline String AsString(const char& a)            { return String(a, 1); }
template<> inline String AsString(const signed char& a)     { return String(a, 1); }
template<> inline String AsString(const unsigned char& a)   { return String(a, 1); }
template<> inline String AsString(const bool& a)            { return a ? "true" : "false"; }

unsigned ctoi(int c);

typedef int (*CharFilter)(int);

int CharFilterAscii(int c);
int CharFilterAscii128(int c);
int CharFilterUnicode(int c);
int CharFilterDigit(int c);
int CharFilterWhitespace(int c);
int CharFilterAlpha(int c);
int CharFilterAlphaToUpper(int c);
int CharFilterAlphaToLower(int c);
int CharFilterInt(int c);
int CharFilterDouble(int c);
int CharFilterDefaultToUpperAscii(int c);
int CharFilterCrLf(int c);

String Filter(const char *s, int (*filter)(int));
String FilterWhile(const char *s, int (*filter)(int));

#include "AString.hpp"
