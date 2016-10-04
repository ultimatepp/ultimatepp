#ifndef _NewCore_AString_h_
#define _NewCore_AString_h_

template <class B>
class AString : public B {
	typedef typename B::tchar  tchar;
	typedef typename B::bchar  bchar;
	typedef typename B::Buffer buffer;
	typedef typename B::String String;

public:
	void Clear()                                              { B::Free(); B::Zero(); }
	int  GetLength() const                                    { return GetCount(); }

	const tchar *End() const                                  { return B::Begin() + B::GetLength(); }

	int operator[](int i) const                               { ASSERT(i >= 0 && i <= GetCount()); return Begin()[i]; }

	operator const tchar *() const                            { return B::Begin(); }
	const tchar *operator~() const                            { return B::Begin(); }
	operator const bchar *() const                            { return (bchar *)B::Begin(); }
	operator const void *() const                             { return B::Begin(); }

	void Insert(int pos, int c)                               { *B::Insert(pos, 1, NULL) = c; }
	void Insert(int pos, const tchar *s, int count)           { B::Insert(pos, count, s); }
	void Insert(int pos, const String& s)                     { Insert(pos, s, s.GetCount()); }
	void Insert(int pos, const char *s)                       { Insert(pos, s, strlen(s)); }

	void  Cat(int c)                                          { B::Cat(c); }
	void  Cat(const tchar *s, int len)                        { B::Cat(s, len); }
	void  Cat(const tchar *s)                                 { Cat(s, strlen__(s)); }
	void  Cat(const String& s)                                { Cat(~s, s.GetLength()); }
	void  Cat(int c, int count);
	void  Cat(const tchar *s, const tchar *lim)               { ASSERT(s <= lim); Cat(s, lim - s); }
	void  Cat(const String& s, int len)                       { B::Cat(~s, len); }
	void  Cat(const bchar *s, int len)                        { Cat((const tchar *) s, len); }

	String& Cat()                                             { return *this; }

	int    Compare(const String& s) const;
	int    Compare(const char *s) const;

	bool   IsEqual(const String& s) const                     { return B::IsEqual(s); }
	bool   IsEqual(const char *s) const                       { return Compare(s) == 0; }

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

	void   Shrink()                                           { return *this = String(Begin(), GetLength()); }

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

#endif
