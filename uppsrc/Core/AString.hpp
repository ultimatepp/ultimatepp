template <class B>
force_inline
void AString<B>::Insert(int pos, const char *s)
{
	Insert(pos, s, strlen__(s));
}

template <class B>
void AString<B>::Cat(int c, int count)
{
	tchar *s = B::Insert(GetLength(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
force_inline
void AString<B>::Cat(const tchar *s)
{
	Cat(s, strlen__(s));
}

template <class B>
int AString<B>::Compare(const tchar *b) const
{
	const tchar *a = B::Begin();
	const tchar *ae = End();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : -1;
		if(*b == 0)
			return 1;
		int q = cmpval__(*a++) - cmpval__(*b++);
		if(q)
			return q;
	}
}

template <class B>
typename AString<B>::String AString<B>::Mid(int from, int count) const
{
	int l = GetLength();
	if(from > l) from = l;
	if(from < 0) from = 0;
	if(count < 0)
		count = 0;
	if(from + count > l)
		count = l - from;
	return String(B::Begin() + from, count);
}

template <class B>
int AString<B>::Find(int chr, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const tchar *e = End();
	const tchar *ptr = B::Begin();
	for(const tchar *s = ptr + from; s < e; s++)
		if(*s == chr)
			return (int)(s - ptr);
	return -1;
}

template <class B>
int AString<B>::ReverseFind(int chr, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	if(from < GetLength()) {
		const tchar *ptr = B::Begin();
		for(const tchar *s = ptr + from; s >= ptr; s--)
			if(*s == chr)
				return (int)(s - ptr);
	}
	return -1;
}

template <class B>
int AString<B>::ReverseFind(int len, const tchar *s, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	if(from < GetLength()) {
		const tchar *ptr = B::Begin();
		const tchar *p = ptr + from - len + 1;
		len *= sizeof(tchar);
		while(p >= ptr) {
			if(*s == *p && memcmp(s, p, len) == 0)
				return (int)(p - ptr);
			p--;
		}
	}
	return -1;
}

template <class B>
int AString<B>::ReverseFindAfter(int len, const tchar *s, int from) const
{
	int q = ReverseFind(len, s, from);
	return q >= 0 ? q + len : -1;
}

template <class B>
void AString<B>::Replace(const tchar *find, int findlen, const tchar *replace, int replacelen)
{
	if(findlen == 0)
		return;
	String r;
	int i = 0;
	const tchar *p = B::Begin();
	for(;;) {
		int j = Find(findlen, find, i);
		if(j < 0)
			break;
		r.Cat(p + i, j - i);
		r.Cat(replace, replacelen);
		i = j + findlen;
	}
	r.Cat(p + i, B::GetCount() - i);
	B::Free();
	B::Set0(r);
}

template <class B>
int AString<B>::ReverseFind(const tchar *s, int from) const
{
	return ReverseFind(strlen__(s), s, from);
}

template <class B>
int AString<B>::ReverseFindAfter(const tchar *s, int from) const
{
	return ReverseFindAfter(strlen__(s), s, from);
}

template <class B>
int AString<B>::ReverseFind(int chr) const
{
	return B::GetCount() ? ReverseFind(chr, B::GetCount() - 1) : -1;
}

template <class B>
void AString<B>::Replace(const String& find, const String& replace)
{
	Replace(~find, find.GetCount(), ~replace, replace.GetCount());
}

template <class B>
force_inline
void AString<B>::Replace(const tchar *find, const tchar *replace)
{
	Replace(find, (int)strlen__(find), replace, (int)strlen__(replace));
}

template <class B>
force_inline
void AString<B>::Replace(const String& find, const tchar *replace)
{
	Replace(~find, find.GetCount(), replace, (int)strlen__(replace));
}

template <class B>
force_inline
void AString<B>::Replace(const tchar *find, const String& replace)
{
	Replace(find, (int)strlen__(find), ~replace, replace.GetCount());
}

template <class B>
bool AString<B>::StartsWith(const tchar *s, int len) const
{
	if(len > GetLength()) return false;
	return memcmp(s, B::Begin(), len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
bool AString<B>::StartsWith(const tchar *s) const
{
	return StartsWith(s, strlen__(s));
}

template <class B>
bool AString<B>::EndsWith(const tchar *s, int len) const
{
	int l = GetLength();
	if(len > l) return false;
	return memcmp(s, B::Begin() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
force_inline
bool AString<B>::EndsWith(const tchar *s) const
{
	return EndsWith(s, strlen__(s));
}

template <class B>
int AString<B>::FindFirstOf(int len, const tchar *s, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const tchar *ptr = B::Begin();
	const tchar *e = B::End();
	const tchar *se = s + len;
	if(len == 1) {
		tchar c1 = s[0];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			if(*bs == c1)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 2) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 3) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3)
				return (int)(bs - ptr);
		}
		return -1;
	}
	if(len == 4) {
		tchar c1 = s[0];
		tchar c2 = s[1];
		tchar c3 = s[2];
		tchar c4 = s[3];
		for(const tchar *bs = ptr + from; bs < e; bs++) {
			tchar ch = *bs;
			if(ch == c1 || ch == c2 || ch == c3 || ch == c4)
				return (int)(bs - ptr);
		}
		return -1;
	}
	for(const tchar *bs = ptr + from; bs < e; bs++)
		for(const tchar *ss = s; ss < se; ss++)
			if(*bs == *ss)
				return (int)(bs - ptr);
	return -1;
}

force_inline
void String0::Set(const char *s, int len)
{
	Clear();
	if(len < 14) {
		memcpy8(chr, s, len);
		SLen() = len;
		Dsyn();
		return;
	}
	SetL(s, len);
	Dsyn();
}

inline
bool String0::IsEqual(const char *s) const
{ // This optimized for comparison with string literals...
	size_t len = strlen(s);
	if(len != GetCount()) return false;
	const char *t = begin();
	return memcmp(begin(), s, len) == 0; // compiler is happy to optimize memcmp out...
}

force_inline
String& String::operator=(const char *s)
{
	AssignLen(s, strlen__(s));
	return *this;
}

force_inline
String::String(const char *s)
{
	String0::Set0(s, strlen__(s));
}

force_inline
void StringBuffer::Strlen()
{
	SetLength((int)strlen__(pbegin));
}

force_inline
void StringBuffer::Cat(const char *s)
{
	Cat(s, (int)strlen__(s));
}
