class WString {
	enum { SMALL = 23 };

	wchar *ptr;
	int    length;
	int    alloc;

	bool    IsRc() const  { return alloc > SMALL; }
	Atomic& Rc()          { return *((Atomic *)ptr - 1); }
	bool    IsShared()    { return IsRc() && Rc() > 1; }

	void    Zero()        { static wchar e[2]; length = alloc = 0; ptr = e; ASSERT(*ptr == 0); }
	wchar  *Alloc(int& count);
	void    Free();
	wchar  *Insert(int pos, int count, const wchar *data);
	void    Set(const wchar *s, int length);
	void    Set(const WString& s);
	void    LCat(int c);
	void    UnShare();

	friend class WStringBuffer;

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

	unsigned GetHashValue() const        { return memhash(ptr, length * sizeof(wchar)); }
	bool     IsEqual(const WString& s)   { return memcmp(ptr, s.ptr, length * sizeof(wchar)) == 0; }

	void Remove(int pos, int count);
	void Insert(int pos, const wchar *s, int count);
	void Clear()                         { Free(); Zero(); }

	void Set(int pos, int ch);
	void Trim(int pos);

	WString& operator=(const WString& s) { Free(); Set(s); return *this; }

	WString()                            { Zero(); }
	WString(const WString& b)            { Set(b); }
	WString(const wchar *s, int l)       { Set(s, l); }
	WString(const wchar *s)              { Set(s, wstrlen(s)); }
	WString(WStringBuffer& b);
	~WString()                           { Free(); }
};

class WStringBuffer {
	wchar   *begin;
	wchar   *end;
	wchar   *limit;

	friend class WString;

	wchar *Alloc(int len, int& alloc);
	void   Expand(int n, const wchar *cat = NULL, int l = 0);
	void   Expand();
	void   Zero();
	void   Free();

public:
	wchar *Begin()                   { *end = '\0'; return begin; }
	wchar *End()                     { *end = '\0'; return end; }

	operator wchar*()                { return Begin(); }

	void  SetLength(int l);
	int   GetLength() const          { return end - begin; }
	void  Strlen()                   { SetLength(wstrlen(begin)); }

	void  Cat(int c)                { if(end >= limit) Expand(); *end++ = c; }
	void  Cat(const wchar *s, int l);

	int   GetAlloc() const           { return limit - begin; }

	WStringBuffer()                  { Zero(); }
	WStringBuffer(WString& s);
	~WStringBuffer()                 { Free(); }
};
