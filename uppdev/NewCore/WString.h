class WString;

class WString0 {
	enum { SMALL = 23 };

	wchar *ptr;
	int    length;
	int    alloc;

	static Atomic voidptr[2];

	bool    IsRc() const  { return alloc > SMALL; }
	Atomic& Rc()          { return *((Atomic *)ptr - 1); }
	bool    IsShared()    { return IsRc() && Rc() > 1; }

	wchar  *Alloc(int& count);
	void    LCat(int c);
	void    UnShare();

	friend class WStringBuffer;
	friend class WString;

protected:
	typedef wchar         tchar;
	typedef int16         bchar;
	typedef WStringBuffer Buffer;
	typedef WString       String;

	void    Zero()                       { static wchar e[2]; length = alloc = 0; ptr = e; ASSERT(*ptr == 0); }
	void    Set(const wchar *s, int length);
	void    Set(const WString0& s);
	void    Free();
	void    Swap(WString0& b)            { Upp::Swap(ptr, b.ptr); Upp::Swap(length, b.length); Upp::Swap(alloc, b.alloc); }
	wchar  *Insert(int pos, int count, const wchar *data);

public:
	const wchar *Begin() const           { return ptr; }
	int   operator[](int i) const        { return ptr[i]; }

	operator const wchar *() const       { return Begin(); }
	const wchar *operator~() const       { return Begin(); }

	void Cat(int c)                      { if(!IsRc() && length < alloc) { ptr[length++] = c; ptr[length] = 0; } else LCat(c); }
	void Cat(const wchar *s, int length);

	int  GetCount() const                { return length; }
	int  GetLength() const               { return length; }
	int  GetAlloc() const                { return alloc; }

	unsigned GetHashValue() const             { return memhash(ptr, length * sizeof(wchar)); }
	bool     IsEqual(const WString0& s) const { return memcmp(ptr, s.ptr, length * sizeof(wchar)) == 0; }

	void Remove(int pos, int count);
	void Insert(int pos, const wchar *s, int count);
	void Clear()                         { Free(); Zero(); }

	void Set(int pos, int ch);
	void Trim(int pos);

	WString0()                           { Zero(); }
	~WString0()                          { Free(); }
};

class WString : public Moveable<WString, AString<WString0> > {
	void Swap(WString& b)                                   { WString0::Swap(b); }

public:
	const WString& operator+=(wchar c)                      { Cat(c); return *this; }
	const WString& operator+=(const wchar *s)               { Cat(s); return *this; }
	const WString& operator+=(const WString& s)             { Cat(s); return *this; }

	WString& operator=(const wchar *s)                      { WString0::Free(); WString0::Set(s, strlen__(s)); return *this; }
	WString& operator=(const WString& s)                    { WString0::Free(); WString0::Set(s); return *this; }
	WString& operator=(WStringBuffer& b)                    { *this = WString(b); return *this; }
	WString& operator<<=(const WString& s)                  { WString0::Free(); WString0::Set(s, s.GetCount()); return *this; }

	WString()                                               {}
	WString(const Nuller&)                                  {}
	WString(const WString& s)                               { WString0::Set(s); }
	WString(const wchar *s)                                 { WString0::Set(s, strlen__(s)); }
	WString(const WString& s, int n)                        { ASSERT(n >= 0 && n <= s.GetLength()); WString0::Set(~s, n); }
	WString(const wchar *s, int n)                          { WString0::Set(s, n); }
	WString(const wchar *s, const wchar *lim)               { WString0::Set(s, lim - s); }
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
};

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

	void  SetLength(int l);
	int   GetLength() const          { return end - begin; }
	void  Strlen()                   { SetLength(wstrlen(begin)); }

	void  Cat(int c)                 { if(end >= limit) Expand(); *end++ = c; }
	void  Cat(const wchar *s, int l);
	void  Cat(const wchar *s)        { Cat(s, wstrlen(s)); }

	int   GetAlloc() const           { return limit - begin; }

	void operator=(WString& s)       { Free(); Set(s); }

	WStringBuffer()                  { Zero(); }
	WStringBuffer(WString& s)        { Set(s); }
	~WStringBuffer()                 { Free(); }
};
