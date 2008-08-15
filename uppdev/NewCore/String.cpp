#include "Core.h"

#include "HeapImp.h"

static inline void *MAlloc_S()
{
	MCache& m = mcache[1];
	FreeLink *l = m.list;
	if(l) {
		m.list = l->next;
		m.count--;
		return l;
	}
	else
		return MAlloc_Get(m, 1);
}

static inline void MFree_S(void *ptr)
{
	MCache& m = mcache[1];
	((FreeLink *)ptr)->next = m.list;
	m.list = (FreeLink *)ptr;
	if(++m.count > CACHEMAX)
		MFree_Reduce(m, 1);
}

String0::Rc String0::voidptr[2] = { { 2, 0 }, { 0, 0 } };

void String0::LSet(const String0& s)
{
	w[2] = s.w[2];
	w[3] = s.w[3];
	if(s.IsRef()) {
		ptr = s.ptr;
		if(ptr != (char *)(voidptr + 1))
			AtomicInc(s.Ref()->refcount);
	}
	else {
		ptr = (char *)MAlloc_S();
		qptr[0] = s.qptr[0];
		qptr[1] = s.qptr[1];
		qptr[2] = s.qptr[2];
		qptr[3] = s.qptr[3];
	}
}

void String0::LFree()
{
	if(IsRef()) {
		if(ptr != (char *)(voidptr + 1)) {
			Rc *rc = Ref();
			ASSERT(rc->refcount > 0);
			if(AtomicDec(rc->refcount) == 0) MFree(rc);
		}
	}
	else
		MFree_S(ptr);
}

dword String0::LEqual(const String0& s) const
{
#ifdef CPU_64
	int l = GetCount();
	if(s.GetCount() != l) return 1;
	const qword *qa = (const qword *)Begin();
	const qword *qb = (const qword *)s.Begin();
	while(l >= 8) {
		if(*qa++ != *qb++) return 1;
		l -= 8;
	}
	const dword *da = (const dword *)qa;
	const dword *db = (const dword *)qb;
	if((l & 4) && *da++ != *db++) return 1;
#else
	int l = GetCount();
	if(s.GetCount() != l) return 1;
	const dword *da = (const dword *)Begin();
	const dword *db = (const dword *)s.Begin();
	while(l >= 4) {
		if(*da++ != *db++) return 1;
		l -= 4;
	}
#endif
	const word *wa = (const word *)da;
	const word *wb = (const word *)db;
	if((l & 2) && *wa++ != *wb++) return 1;
	return (l & 1) ? *(const char *)wa != *(const char *)wb : 0;
}

unsigned String0::LHashValue() const
{
	int l = LLen();
	if(l < 15) {
		dword w[4];
		w[0] = w[1] = w[2] = w[3] = 0;
		memcpy(w, ptr, l);
		((byte *)w)[SLEN] = l;
		return CombineHash(w[0], w[1], w[2], w[3]);
	}
	return memhash(ptr, l);
}

char *String0::Alloc(int count, char& kind)
{
	if(count < 32) {
		kind = MEDIUM;
		return (char *)MAlloc_S();
	}
	size_t sz = sizeof(Rc) + count + 1;
	Rc *rc = (Rc *)MAlloc(sz);
	rc->alloc = sz - sizeof(Rc) - 1;
	rc->refcount = 1;
	kind = min(rc->alloc, 255);
	return (char *)(rc + 1);
}

char *String0::Insert(int pos, int count, const char *s)
{
	ASSERT(pos >= 0 && count >= 0 && pos <= GetCount());
	int len = GetCount();
	int newlen = len + count;
	char *str = (char *)Begin();
	if(newlen < GetAlloc() && !IsSharedRef()/* && !(s >= str && s <= str + len)*/) {
		if(s >= str + pos && s <= str + len)
			s += count;
		if(pos < len)
			memmove(str + pos + count, str + pos, len - pos);
		if(IsSmall())
			SLen() = newlen;
		else
			LLen() = newlen;
		str[newlen] = 0;
		if(s)
			memcpy(str + pos, s, count);
		return str + pos;
	}
	char kind;
	char *p = Alloc(max(2 * len, newlen), kind);
	if(pos > 0)
		memcpy(p, str, pos);
	if(pos < len)
		memcpy(p + pos + count, str + pos, len - pos);
	if(s)
		memcpy(p + pos, s, count);
	p[newlen] = 0;
	Free();
	ptr = p;
	LLen() = newlen;
	SLen() = 15;
	chr[KIND] = kind;
	return ptr + pos;
}

void String0::UnShare()
{
	if(IsSharedRef()) {
		int len = LLen();
		char kind;
		char *p = Alloc(len, kind);
		memcpy(p, ptr, len + 1);
		Free();
		chr[KIND] = kind;
		ptr = p;
	}
}

void String0::SetSLen(int l)
{
	SLen() = l;
	memset(chr + l, 0, 15 - l);
}

void String0::Remove(int pos, int count)
{
	ASSERT(pos >= 0 && count >= 0 && pos + count <= GetCount());
	UnShare();
	char *s = (char *)Begin();
	memmove(s + pos, s + pos + count, GetCount() - pos - count + 1);
	if(IsSmall())
		SetSLen(SLen() - count);
	else
		LLen() -= count;
}

void String0::Set(int pos, int chr)
{
	ASSERT(pos >= 0 && pos < GetCount());
	UnShare();
	Ptr()[pos] = chr;
}

void String0::Trim(int pos)
{
	ASSERT(pos >= 0 && pos <= GetCount());
	if(IsSmall()) {
		chr[pos] = 0;
		SetSLen(pos);
	}
	else {
		UnShare();
		ptr[pos] = 0;
		LLen() = pos;
	}
}

void String0::LCat(int c)
{
	if(IsSmall()) {
		qword *x = (qword *)MAlloc_S();
		x[0] = q[0];
		x[1] = q[1];
		LLen() = SLen();
		SLen() = 15;
		chr[KIND] = MEDIUM;
		qptr = x;
	}
	int l = LLen();
	if(IsRef() ? !IsShared() && l < (int)Ref()->alloc : l < 31) {
		ptr[l] = c;
		ptr[LLen() = l + 1] = 0;
	}
	else {
		char *s = Insert(l, 1, NULL);
		s[0] = c;
		s[1] = 0;
	}
}

void String0::Cat(const char *s, int len)
{
	if(IsSmall()) {
		if(SLen() + len < 14) {
			memcpy(chr + SLen(), s, len);
			SLen() += len;
			chr[SLen()] = 0;
			return;
		}
	}
	else
		if((int)LLen() + len < LAlloc() && !IsSharedRef()) {
			memcpy(ptr + LLen(), s, len);
			LLen() += len;
			ptr[LLen()] = 0;
			return;
		}
	Insert(GetCount(), len, s);
}

void String0::Set(const char *s, int len)
{
	w[0] = w[1] = w[2] = w[3] = 0;
	switch(len) {
	#define MOV(x) case x: chr[x - 1] = s[x - 1];
		MOV(14) MOV(13) MOV(12) MOV(11) MOV(10) MOV(9) MOV(8)
		MOV(7) MOV(6) MOV(5) MOV(4) MOV(3) MOV(2) MOV(1)
	case 0:
		SLen() = len;
		break;
	default:
		char *p = Alloc(len, chr[KIND]);
		memcpy(p, s, len);
		p[len] = 0;
		ptr = p;
		LLen() = len;
		SLen() = 15;
	};
}

String String::GetVoid()
{
	String s;
	s.ptr = (char *)(voidptr + 1);
	s.LLen() = 0;
	s.SLen() = 15;
	s.chr[KIND] = 50;
	return s;
}

bool String::IsVoid() const
{
	return IsRef() && ptr == (char *)(voidptr + 1);
}

String::String(StringBuffer& b)
{
	int l = b.GetLength();
	if(l <= 14) {
		Zero();
		memcpy(chr, b.begin, l);
		SLen() = l;
		b.Free();
	}
	else {
		ptr = b.begin;
		ptr[l] = 0;
		SLen() = 15;
		LLen() = l;
		chr[KIND] = min(b.GetAlloc(), 255);
	}
	b.Zero();
}

WString String::ToWString() const
{
	return WString(Begin(), GetCount());
}

char *StringBuffer::Alloc(int count, int& alloc)
{
	if(count <= 31) {
		char *s = (char *)MAlloc_S();
		alloc = 31;
		return s;
	}
	else {
		size_t sz = sizeof(Rc) + count + 1;
		Rc *rc = (Rc *)MAlloc(sz);
		alloc = rc->alloc = sz - sizeof(Rc) - 1;
		rc->refcount = 1;
		return (char *)(rc + 1);
	}
}

void StringBuffer::Free()
{
	int all = limit - begin;
	if(all == 31)
		MFree_S(begin);
	if(all > 31)
		MFree((Rc *)begin - 1);
}

void StringBuffer::Expand(int n, const char *cat, int l)
{
	int al;
	int ep = end - begin;
	char *p = Alloc(n, al);
	memcpy(p, begin, GetLength());
	if(cat) {
		memcpy(p + ep, cat, l);
		ep += l;
	}
	Free();
	begin = p;
	end = begin + ep;
	limit = begin + al;
}

void StringBuffer::Expand()
{
	Expand(GetLength() * 2);
}

void StringBuffer::SetLength(int l)
{
	if(l > (limit - begin))
		Expand(l);
	end = begin + l;
}

void StringBuffer::Cat(const char *s, int l)
{
	if(end + l > limit)
		Expand(max(GetLength(), l) + GetLength(), s, l);
	else {
		memcpy(end, s, l);
		end += l;
	}
}

void StringBuffer::Set(String& s)
{
	s.UnShare();
	int l = s.GetLength();
	if(s.GetAlloc() == 14) {
		begin = (char *)MAlloc_S();
		limit = begin + 31;
		memcpy(begin, s.Begin(), l);
		end = begin + l;
	}
	else {
		begin = s.ptr;
		end = begin + l;
		limit = begin + s.GetAlloc();
	}
	s.Zero();
}
