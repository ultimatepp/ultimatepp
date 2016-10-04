template <class B>
void AString<B>::Cat(int c, int count)
{
	tchar *s = B::Insert(GetLength(), count, NULL);
	while(count--)
		*s++ = c;
}

template <class B>
int AString<B>::Compare(const String& s) const
{
	const tchar *a = Begin();
	const tchar *ae = End();
	const tchar *b = s.Begin();
	const tchar *be = s.End();
	for(;;) {
		if(a >= ae)
			return b >= be ? 0 : 1;
		if(b >= be)
			return -1;
		int q = cmpval__(*a++) - cmpval__(*b++);
		if(q)
			return q;
	}
}

template <class B>
int AString<B>::Compare(const char *b) const
{
	const tchar *a = Begin();
	const tchar *ae = End();
	for(;;) {
		if(a >= ae)
			return *b == 0 ? 0 : 1;
		if(*b == 0)
			return -1;
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
	return String(Begin() + from, count);
}

template <class B>
int AString<B>::Find(int chr, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const tchar *e = End();
	const tchar *ptr = Begin();
	for(const tchar *s = ptr + from; s < e; s++)
		if(*s == chr)
			return s - ptr;
	return -1;
}

template <class B>
int AString<B>::ReverseFind(int chr, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	if(from < GetLength()) {
		const tchar *ptr = Begin();
		for(const tchar *s = ptr + from; s >= ptr; s--)
			if(*s == chr)
				return s - ptr;
	}
	return -1;
}

template <class B>
int AString<B>::ReverseFind(int chr) const
{
	return GetCount() ? ReverseFind(chr, GetCount() - 1) : -1;
}

template <class B>
int AString<B>::Find(int len, const tchar *s, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const tchar *ptr = Begin();
	const tchar *p = ptr + from;
	int l = GetLength() - len - from;
	if(l < 0)
		return -1;
	const tchar *e = p + l;
	len *= sizeof(tchar);
	while(p <= e) {
		if(memcmp(s, p, len) == 0)
			return p - ptr;
		p++;
	}
	return -1;
}

template <class B>
bool AString<B>::StartsWith(const tchar *s, int len) const
{
	if(len > GetLength()) return false;
	return memcmp(s, Begin(), len * sizeof(tchar)) == 0;
}

template <class B>
bool AString<B>::EndsWith(const tchar *s, int len) const
{
	int l = GetLength();
	if(len > l) return false;
	return memcmp(s, Begin() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
int AString<B>::Find(const tchar *s, int from) const
{
	return Find(strlen__(s), s, from);
}
