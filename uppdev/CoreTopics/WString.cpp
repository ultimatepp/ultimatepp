#include "Core.h"

NAMESPACE_UPP

wchar *WString0::Alloc(int& count)
{
	if(count <= SMALL) {
		count = SMALL;
		wchar *p = (wchar *)MemoryAlloc48();
		return p;
	}
	size_t sz = sizeof(Atomic) + (count + 1) * sizeof(wchar);
	Atomic *rc = (Atomic *)MemoryAlloc(sz);
	count = ((int)(sz - sizeof(Atomic)) >> 1) - 1;
	*rc = 1;
	return (wchar *)(rc + 1);
}

void WString0::Free()
{
	if(alloc > 0)
		if(IsRc()) {
			Atomic& rc = Rc();
			if(AtomicDec(rc) == 0)
				MemoryFree(&rc);
		}
		else
			MemoryFree48(ptr);
}

#ifdef _DEBUG
void WString0::Dsyn()
{
	WString *d_str = static_cast<WString *>(this);
	d_str->s = Begin();
	d_str->len = GetCount();
}
#endif

wchar *WString0::Insert(int pos, int count, const wchar *s)
{
	ASSERT(pos >= 0 && count >= 0 && pos <= GetCount());
	int newlen = length + count;
	if(newlen < alloc && !IsShared() && (!s || s < ptr || s > ptr + length)) {
		if(pos < length)
			memmove(ptr + pos + count, ptr + pos, (length - pos) * sizeof(wchar));
		length = newlen;
		ptr[newlen] = 0;
		if(s)
			memcpy(ptr + pos, s, count * sizeof(wchar));
		Dsyn();
		return ptr + pos;
	}
	int all = max(2 * length, newlen);
	wchar *p = Alloc(all);
	if(pos > 0)
		memcpy(p, ptr, pos * sizeof(wchar));
	if(pos < length)
		memcpy(p + pos + count, ptr + pos, (length - pos) * sizeof(wchar));
	if(s)
		memcpy(p + pos, s, count * sizeof(wchar));
	ASSERT(pos + count <= all);
	p[newlen] = 0;
	Free();
	ptr = p;
	length = newlen;
	alloc = all;
	Dsyn();
	return ptr + pos;
}

void WString0::Set(const WString0& src)
{
	if(src.alloc <= 0) {
		static wchar h[2];
		ptr = h;
		length = 0;
		alloc = src.alloc;
		Dsyn();
		return;
	}
	if(src.length == 0) {
		Zero();
		return;
	}
	length = src.length;
	alloc = src.alloc;
	if(src.IsRc()) {
		ptr = src.ptr;
		AtomicInc(Rc());
	}
	else {
		ptr = (wchar *)MemoryAlloc48();
		qword *t = (qword *)ptr;
		qword *s = (qword *)src.ptr;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
	}
	Dsyn();
}

void WString0::Cat(const wchar *s, int l)
{
	if(length + l >= alloc || IsShared())
		Insert(length, l, s);
	else {
		memcpy(ptr + length, s, l * sizeof(wchar));
		ptr[length += l] = 0;
	}
	Dsyn();
}

void WString0::LCat(int c)
{
	*Insert(length, 1, NULL) = c;
}

void WString0::Set(const wchar *s, int l)
{
	alloc = length = l;
	memcpy(ptr = Alloc(alloc), s, l * sizeof(wchar));
	ptr[l] = 0;
	Dsyn();
}

void WString0::UnShare()
{
	if(!IsShared()) return;
	int al = length;
	wchar *p = Alloc(al);
	memcpy(p, ptr, (length + 1) * sizeof(wchar));
	Free();
	ptr = p;
	alloc = al;
}

void WString0::Set(int pos, int ch)
{
	ASSERT(pos >= 0 && pos < GetCount());
	UnShare();
	ptr[pos] = ch;
}

void WString0::Trim(int pos)
{
	ASSERT(pos >= 0 && pos <= GetCount());
	UnShare();
	length = pos;
	ptr[pos] = 0;
	Dsyn();
}

void WString0::Remove(int pos, int count)
{
	ASSERT(pos >= 0 && count >= 0 && pos + count <= GetCount());
	UnShare();
	memmove(ptr + pos, ptr + pos + count, (GetCount() - pos - count + 1) * sizeof(wchar));
	length -= count;
	Dsyn();
}

void WString0::Insert(int pos, const wchar *s, int count)
{
	Insert(pos, count, s);
	Dsyn();
}

int WString0::Compare(const WString0& s) const
{
	const wchar *a = Begin();
	const wchar *ae = a + GetLength();
	const wchar *b = s.Begin();
	const wchar *be = b + s.GetLength();
	for(;;) {
		if(a >= ae)
			return b >= be ? 0 : -1;
		if(b >= be)
			return 1;
		int q = cmpval__(*a++) - cmpval__(*b++);
		if(q)
			return q;
	}
}

WString& WString::operator=(const wchar *s)
{
	int  len = GetCount();
	wchar *str = (wchar *)Begin();
	if(s >= str && s <= str + len)
		return *this = WString(s, strlen__(s));
	WString0::Free();
	WString0::Set(s, strlen__(s));
	return *this;
}

WString::WString(WStringBuffer& b)
{
	length = b.GetLength();
	ptr = b.begin;
	ptr[length] = 0;
	alloc = b.GetAlloc();
	b.Zero();
	Dsyn();
}

WString::WString(const char *s) {
	Zero();
	*this = ToUnicode(s, s ? (int)strlen(s) : 0, CHARSET_DEFAULT);
}

WString::WString(const char *s, int n) {
	Zero();
	*this = ToUnicode(s, n, CHARSET_DEFAULT);
}

WString::WString(const char *s, const char *lim) {
	Zero();
	*this = ToUnicode(s, s ? (int)(lim - s) : 0, CHARSET_DEFAULT);
}

String WString::ToString() const
{
	return FromUnicode(*this, CHARSET_DEFAULT);
}

Atomic WString0::voidptr[2] = { 0, 0 };

WString WString::GetVoid()
{
	WString b;
	b.alloc = -1;
	return b;
}

WString::WString(const std::wstring& s)
{
	if(sizeof(std::wstring::value_type) == sizeof(wchar)) {
		WString0::Set((wchar *)s.c_str(), (int)s.length());
	}
	else {
		WString0::Zero();
		std::wstring::const_iterator i = s.begin();
		while(i < s.end())
			Cat(*i++);
	}
}

WString::operator std::wstring() const
{
	if(sizeof(std::wstring::value_type) == sizeof(wchar))
		return std::wstring((std::wstring::value_type *)Begin(),
		                    (std::wstring::value_type *)End());
	else {
		std::wstring r;
		const wchar *s = Begin();
		while(s < End())
			r += *s++;
		return r;
	}
}

void WStringBuffer::Zero()
{
	static wchar h[2];
	begin = end = limit = h;
}

wchar *WStringBuffer::Alloc(int count, int& alloc)
{
	if(count <= 23) {
		wchar *s = (wchar *)MemoryAlloc48();
		alloc = WString0::SMALL;
		return s;
	}
	else {
		size_t sz = sizeof(Atomic) + (count + 1) * sizeof(wchar);
		Atomic *rc = (Atomic *)MemoryAlloc(sz);
		alloc = ((int)(sz - sizeof(Atomic)) >> 1) - 1;
		*rc = 1;
		return (wchar *)(rc + 1);
	}
}

void WStringBuffer::Free()
{
	int all = (int)(limit - begin);
	if(all == WString0::SMALL)
		MemoryFree48(begin);
	if(all > WString0::SMALL)
		MemoryFree((Atomic *)begin - 1);
}

void WStringBuffer::Expand(int n, const wchar *cat, int l)
{
	int al;
	int ep = (int)(end - begin);
	wchar *p = Alloc(n, al);
	memcpy(p, begin, GetLength() * sizeof(wchar));
	if(cat) {
		memcpy(p + ep, cat, l * sizeof(wchar));
		ep += l;
	}
	Free();
	begin = p;
	end = begin + ep;
	limit = begin + al;
}

void WStringBuffer::Expand()
{
	Expand(GetLength() * 2);
}

void WStringBuffer::SetLength(int l)
{
	if(l > (limit - begin))
		Expand(l);
	end = begin + l;
}

void WStringBuffer::Cat(const wchar *s, int l)
{
	if(end + l > limit)
		Expand(max(GetLength(), l) + GetLength(), s, l);
	else {
		memcpy(end, s, l * sizeof(wchar));
		end += l;
	}
}

void WStringBuffer::Cat(int c, int l)
{
	if(end + l > limit)
		Expand(max(GetLength(), l) + GetLength(), NULL, l);
	memsetw(end, c, l);
	end += l;
}

void WStringBuffer::Set(WString& s)
{
	s.UnShare();
	int l = s.GetLength();
	begin = s.ptr;
	end = begin + l;
	limit = begin + s.GetAlloc();
	s.Zero();
}

WString TrimLeft(const WString& str)
{
	const wchar *s = str;
	if(!IsSpace(*s))
		return s;
	while(IsSpace(*s)) s++;
	return WString(s, str.End());
}

WString TrimRight(const WString& str)
{
	if(str.IsEmpty())
		return str;
	const wchar *s = str.Last();
	if(!IsSpace(*s))
		return str;
	while(s >= ~str && IsSpace(*s)) s--;
	return WString(~str, s + 1);
}

struct WStringICompare__
{
	int operator()(wchar a, wchar b) const { return ToUpper(a) - ToUpper(b); }
};

int CompareNoCase(const WString& a, const WString& b)
{
	return IterCompare(a.Begin(), a.End(), b.Begin(), b.End(), WStringICompare__());
}

int CompareNoCase(const WString& a, const wchar *b)
{
	return IterCompare(a.Begin(), a.End(), b, b + wstrlen(b), WStringICompare__());
}

END_UPP_NAMESPACE
