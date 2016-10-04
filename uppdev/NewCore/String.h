#ifndef _NewString_NewString_h
#define _NewString_NewString_h

using namespace Upp;

class StringBuffer;
class String;

class String0 : Moveable<String0> {
	enum { SMALL, MEDIUM = 31 };
	enum { KIND = 14, SLEN = 15, LLEN = 2 };

	struct Rc {
		Atomic refcount;
		int    alloc;

		char *GetPtr() const  { return (char*)(this + 1); }
		void  Release();
		void  Retain();
	};

	union { // Optimize for CPU_64 too!!!
		char   chr[16];
		char  *ptr;
		dword *wptr;
		qword *qptr;
		dword  w[4];
		qword  q[2];
	};

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

	void LSet(const String0& s);
	void LFree();
	void LCat(int c);
	unsigned LHashValue() const;

	void UnShare();
	void SetSLen(int l);

	char *Ptr() { return IsSmall() ? chr : ptr; }
	char *Alloc(int count, char& kind);

	static String0::Rc String0::voidptr[2];

	void Swap(String0& b)                           { ::Swap(q[0], b.q[0]); ::Swap(q[1], b.q[1]); }

	friend class String;
	friend class StringBuffer;

protected:
	void Zero() { q[0] = q[1] = 0; }
	void Free()                  { if(IsLarge()) LFree(); }
	void Set(const String0& s) {
		if(s.IsSmall()) { q[0] = s.q[0]; q[1] = s.q[1]; }
		else LSet(s);
	}
	void Set(const char *s, int len);
	char *Insert(int pos, int count, const char *str);


	typedef char         tchar;
	typedef byte         bchar;
	typedef StringBuffer Buffer;
	typedef String       String;

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

	unsigned GetHashValue() const {
		return chr[KIND] ? LHashValue() : (unsigned)CombineHash(w[0], w[1], w[2], w[3]);
	}

	void Cat(int c) {
		if(SLen() < 14)
			chr[SLen()++] = c;
		else
			LCat(c);
	}

	void Cat(const char *s, int len);

	void Set(int i, int chr);
	void Trim(int pos);

	const char *Begin() const   { return IsSmall() ? chr : ptr; }

	int operator[](int i) const { ASSERT(i >= 0 && i <= GetCount()); return Begin()[i]; }

	operator const char *() const   { return Begin(); }
	const char *operator~() const   { return Begin(); }

	void Remove(int pos, int count);
	void Clear()                { Free(); Zero(); }

	int GetCount() const        { return IsSmall() ? chr[SLEN] : w[LLEN]; }
	int GetLength() const       { return GetCount(); }
	int GetAlloc() const        { return IsSmall() ? 14 : LAlloc(); }

	String0& operator=(const String0& s) { Free(); Set(s); return *this; }

	String0()                       { Zero(); }
	~String0()                      { Free(); }
};

class String : public Moveable<String, AString<String0> > {
	void Swap(String& b)                                   { String0::Swap(b); }

public:
	const String& operator+=(char c)                       { Cat(c); return *this; }
	const String& operator+=(const char *s)                { Cat(s); return *this; }
	const String& operator+=(const String& s)              { Cat(s); return *this; }

	String& operator=(const char *s)                       { String0::Free(); String0::Set(s, strlen__(s)); return *this; }
	String& operator=(const String& s)                     { String0::Free(); String0::Set(s); return *this; }
	String& operator=(StringBuffer& b)                     { *this = String(b); return *this; }
	String& operator<<=(const String& s)                   { String0::Free(); String0::Set(s, s.GetCount()); return *this; }

	String()                                               {}
	String(const Nuller&)                                  {}
	String(const String& s)                                { String0::Set(s); }
	String(const char *s)                                  { String0::Set(s, strlen__(s)); }
	String(const String& s, int n)                         { ASSERT(n >= 0 && n <= s.GetLength()); String0::Set(~s, n); }
	String(const char *s, int n)                           { String0::Set(s, n); }
	String(const byte *s, int n)                           { String0::Set((const char *)s, n); }
	String(const char *s, const char *lim)                 { String0::Set(s, lim - s); }
	String(int chr, int count)                             { String0::Zero(); Cat(chr, count); }
	String(StringBuffer& b);

	WString ToWString() const;

	static String GetVoid();
	bool   IsVoid() const;

	friend void Swap(String& a, String& b)                 { a.Swap(b); }
};

class StringBuffer : NoCopy {
	char   *begin;
	char   *end;
	char   *limit;

	friend class String;

	typedef String0::Rc Rc;

	char *Alloc(int len, int& alloc);
	void  Expand(int n, const char *cat = NULL, int l = 0);
	void  Expand();
	void  Zero()                    { begin = end = limit = ""; }
	void  Free();
	void  Set(String& s);

public:
	char *Begin()                   { *end = '\0'; return begin; }
	char *End()                     { *end = '\0'; return end; }

	operator char*()                { return Begin(); }

	void SetLength(int l);
	int  GetLength() const          { return end - begin; }
	void Strlen()                   { SetLength(strlen(begin)); }

	void  Cat(int c)                { if(end >= limit) Expand(); *end++ = c; }
	void  Cat(const char *s, int l);
	void  Cat(const char *s)        { Cat(s, strlen(s)); }

	int  GetAlloc() const           { return limit - begin; }

	void operator=(String& s)       { Free(); Set(s); }

	StringBuffer()                  { Zero(); }
	StringBuffer(String& s)         { Set(s); }
	~StringBuffer()                 { Free(); }
};

#endif
