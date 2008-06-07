template <class B>
void AString<B>::Cat(int c, int count)
{
	tchar *s = B::Insert(GetLength(), count, NULL);
	while(count--)
		*s++ = c;
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
int AString<B>::ReverseFind(int chr) const
{
	return B::GetCount() ? ReverseFind(chr, B::GetCount() - 1) : -1;
}

template <class B>
int AString<B>::Find(int len, const tchar *s, int from) const
{
	ASSERT(from >= 0 && from <= GetLength());
	const tchar *ptr = B::Begin();
	const tchar *p = ptr + from;
	int l = GetLength() - len - from;
	if(l < 0)
		return -1;
	const tchar *e = p + l;
	len *= sizeof(tchar);
	while(p <= e) {
		if(memcmp(s, p, len) == 0)
			return (int)(p - ptr);
		p++;
	}
	return -1;
}

template <class B>
bool AString<B>::StartsWith(const tchar *s, int len) const
{
	if(len > GetLength()) return false;
	return memcmp(s, B::Begin(), len * sizeof(tchar)) == 0;
}

template <class B>
bool AString<B>::EndsWith(const tchar *s, int len) const
{
	int l = GetLength();
	if(len > l) return false;
	return memcmp(s, B::Begin() + l - len, len * sizeof(tchar)) == 0;
}

template <class B>
int AString<B>::Find(const tchar *s, int from) const
{
	return Find(strlen__(s), s, from);
}

inline int String0::Compare(const String0& s) const
{
#ifdef FAST_STRING_COMPARE
	if((chr[KIND] | s.chr[KIND]) == 0) {
	#ifdef CPU_64
		uint64 a64 = SwapEndian64(q[0]);
		uint64 b64 = SwapEndian64(s.q[0]);
		if(a64 != b64)
			return a64 < b64 ? -1 : 1;
		uint32 a32 = SwapEndian32(w[2]);
		uint32 b32 = SwapEndian32(s.w[2]);
		if(a32 != b32)
			return a32 < b32 ? -1 : 1;
	#else
		uint32 a32 = SwapEndian32(w[0]);
		uint32 b32 = SwapEndian32(s.w[0]);
		if(a32 != b32)
			return a32 < b32 ? -1 : 1;
		a32 = SwapEndian32(w[1]);
		b32 = SwapEndian32(s.w[1]);
		if(a32 != b32)
			return a32 < b32 ? -1 : 1;
		a32 = SwapEndian32(w[2]);
		b32 = SwapEndian32(s.w[2]);
		if(a32 != b32)
			return a32 < b32 ? -1 : 1;
	#endif
		uint16 a16 = SwapEndian16(v[6]);
		uint16 b16 = SwapEndian16(s.v[6]);
		if(a16 != b16)
			return a16 < b16 ? -1 : 1;
		return 0;
	}
#endif
/*	int l1 = GetLength();
	int l2 = s.GetLength();
	int q = memcmp(Begin(), s.Begin(), min(l1, l2));
	int q = MemCmp_aligned__(Begin(), s.Begin(), min(l1, l2));
	return q ? q : l1 - l2;*/
	return LCompare(s);
}
