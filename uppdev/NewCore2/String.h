#ifndef _NewString_NewString_h
#define _NewString_NewString_h

using namespace Upp;

class StringBuffer;

class String {
	enum { SMALL, MEDIUM, REF };
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
	bool   IsRef() const         { return chr[KIND] == REF; }
	Rc    *Ref() const           { return (Rc *)ptr - 1; }
	bool   IsShared() const      { return Ref()->refcount != 1; }
	bool   IsSharedRef() const   { return IsRef() && IsShared(); }
	int    LAlloc() const        { return IsMedium() ? 31 : Ref()->alloc; }
	dword  LEqual(const String& s) const;

	void LSet(const String& s);
	void LFree();
	void LCat(int c);
	unsigned LHashValue() const;

	char *Insert(int pos, int count, const char *str);

	void UnShare();
	void SetSLen(int l);

	char *Ptr() { return IsSmall() ? chr : ptr; }
	char *Alloc(int count, char& kind);

	static Rc SVoid;
	static char *VoidPtr() { return (char *)(&SVoid + 1); }

	friend class StringBuffer;

protected:
	void Zero() { q[0] = q[1] = 0; }
	void Free()                  { if(IsLarge()) LFree(); }
	void Set(const String& s) {
		if(s.IsSmall()) { q[0] = s.q[0]; q[1] = s.q[1]; }
		else LSet(s);
	}
	void Set(const char *s, int len);

public:
	bool IsEqual(const String& s) const {
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
	void Insert(int pos, const char *s, int len);
	void Clear()                { Free(); Zero(); }

	int GetCount() const        { return IsSmall() ? chr[SLEN] : w[LLEN]; }
	int GetLength() const       { return GetCount(); }
	int GetAlloc() const        { return IsSmall() ? 14 : LAlloc(); }

	String& operator=(const String& s) { Free(); Set(s); return *this; }

	String()                       { Zero(); }
	String(const String& s)        { Set(s); }
	String(const char *s, int len) { Set(s, len); }
	String(const char *s)          { Set(s, strlen(s)); }
	String(StringBuffer& b);
	~String()                      { Free(); }

	static String GetVoid();
	bool   IsVoid() const          { return IsRef() && Ref()->alloc == 0; }
};

class StringBuffer {
	char   *begin;
	char   *end;
	char   *limit;

	friend class String;

	typedef String::Rc Rc;

	char *Alloc(int len, int& alloc);
	void  Expand(int n, const char *cat = NULL, int l = 0);
	void  Expand();
	void  Zero()                    { begin = end = limit = ""; }
	void  Free();

public:
	char *Begin()                   { *end = '\0'; return begin; }
	char *End()                     { *end = '\0'; return end; }

	operator char*()                { return Begin(); }

	void SetLength(int l);
	int  GetLength() const          { return end - begin; }
	void Strlen()                   { SetLength(strlen(begin)); }

	void  Cat(int c)                { if(end >= limit) Expand(); *end++ = c; }
	void  Cat(const char *s, int l);

	int  GetAlloc() const           { return limit - begin; }

	StringBuffer()                  { Zero(); }
	StringBuffer(String& s);
	~StringBuffer()                 { Free(); }
};

#endif
