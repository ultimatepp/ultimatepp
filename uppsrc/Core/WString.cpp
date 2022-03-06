#include "Core.h"

namespace Upp {

wchar *WString0::Alloc(int& count)
{
	if(count <= SMALL) {
		count = SMALL;
		wchar *p = (wchar *)MemoryAlloc((SMALL + 1) * sizeof(wchar));
		return p;
	}
	size_t sz = sizeof(Atomic) + ((size_t)count + 1) * sizeof(wchar);
	Atomic *rc = (Atomic *)MemoryAllocSz(sz);
	if(count != INT_MAX)
		count = int(((sz - sizeof(Atomic)) / sizeof(wchar)) - 1);
	*rc = 1;
	return (wchar *)(rc + 1);
}

void WString0::Free()
{
	if(alloc > 0) {
		if(IsRc()) {
			Atomic& rc = Rc();
			if(AtomicDec(rc) == 0)
				MemoryFree(&rc);
		}
		else
			MemoryFree(ptr);
	}
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
	if(newlen < length)
		Panic("WString is too big!");
	if(newlen < alloc && !IsShared() && (!s || s < ptr || s > ptr + length)) {
		if(pos < length)
			memmove(ptr + pos + count, ptr + pos, (length - pos) * sizeof(wchar));
		length = newlen;
		ptr[newlen] = 0;
		if(s)
			memcpy_t(ptr + pos, s, count);
		Dsyn();
		return ptr + pos;
	}
	int all = max(length >= int((int64)2 * INT_MAX / 3) ? INT_MAX : length + (length >> 1), newlen);
	wchar *p = Alloc(all);
	if(pos > 0)
		memcpy_t(p, ptr, pos);
	if(pos < length)
		memcpy_t(p + pos + count, ptr + pos, length - pos);
	if(s)
		memcpy_t(p + pos, s, count);
	ASSERT(pos + count <= all);
	p[newlen] = 0;
	Free();
	ptr = p;
	length = newlen;
	alloc = all;
	Dsyn();
	return ptr + pos;
}

void WString0::Set0(const WString0& src)
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
		ptr = (wchar *)MemoryAlloc((SMALL + 1) * sizeof(wchar));
		memcpy(ptr, src.ptr, sizeof(wchar) * (SMALL + 1));
	}
	Dsyn();
}

void WString0::Cat(const wchar *s, int l)
{
	if(length + l >= alloc || IsShared())
		Insert(length, l, s);
	else {
		memcpy_t(ptr + length, s, l);
		ptr[length += l] = 0;
	}
	Dsyn();
}

void WString0::Set(const wchar *s, int length)
{
	Free();
	Set0(s, length);
}

void WString0::LCat(int c)
{
	*Insert(length, 1, NULL) = c;
}

void WString0::Set0(const wchar *s, int l)
{
	alloc = length = l;
	memcpy_t(ptr = Alloc(alloc), s, l);
	ptr[l] = 0;
	Dsyn();
}

void WString0::UnShare()
{
	if(!IsShared()) return;
	int al = length;
	wchar *p = Alloc(al);
	memcpy_t(p, ptr, length + 1);
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
	WString0::Set0(s, strlen__(s));
	return *this;
}

WString::WString(WStringBuffer& b)
{
	length = b.GetLength();
	ptr = b.pbegin;
	ptr[length] = 0;
	alloc = b.GetAlloc();
	if(GetAlloc() > 4 * GetLength() / 3)
		Shrink();
	b.Zero();
	Dsyn();
}

WString::WString(const char *s) {
	Zero();
	*this = ToUnicode(s, s ? (int)strlen(s) : 0, CHARSET_DEFAULT);
}

WString::WString(const char16 *s) {
	Zero();
	*this = ToUtf32(s);
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

Atomic WString0::voidptr[2];

WString WString::GetVoid()
{
	WString b;
	b.alloc = -1;
	return b;
}

#ifndef _HAVE_NO_STDWSTRING
static_assert(sizeof(wchar_t) == 4 || sizeof(wchar_t) == 2, "Invalid wchar_t size");

WString::WString(const std::wstring& s)
{
	WString0::Zero();
	if(sizeof(wchar_t) == 4)
		*this = WString((const wchar *)s.c_str(), (int)s.size());
	if(sizeof(wchar_t) == 2)
		*this = ToUtf32((char16 *)s.c_str(), (int)s.size());
}

std::wstring WString::ToStd() const
{
	if(sizeof(wchar_t) == 4)
		return std::wstring((const wchar_t *)begin(), GetCount());
	if(sizeof(wchar_t) == 2) {
		Vector<char16> h = ToUtf16(*this);
		return std::wstring((const wchar_t *)h.begin(), h.GetCount());
	}
}
#endif

void WStringBuffer::Zero()
{
	static wchar h[2];
	pbegin = pend = limit = h;
}

wchar *WStringBuffer::Alloc(int count, int& alloc)
{
	if(count <= 23) {
		wchar *s = (wchar *)MemoryAlloc(24 * sizeof(wchar));
		alloc = WString0::SMALL;
		return s;
	}
	else {
		size_t sz = sizeof(Atomic) + ((size_t)count + 1) * sizeof(wchar);
		Atomic *rc = (Atomic *)MemoryAlloc(sz);
		alloc = (int)min((size_t)INT_MAX, ((sz - sizeof(Atomic)) / sizeof(wchar)) - 1);
		ASSERT(alloc >= 0);
		*rc = 1;
		return (wchar *)(rc + 1);
	}
}

void WStringBuffer::Free()
{
	int all = (int)(limit - pbegin);
	if(all == WString0::SMALL)
		MemoryFree(pbegin);
	if(all > WString0::SMALL)
		MemoryFree((Atomic *)pbegin - 1);
}

void WStringBuffer::Expand(dword n, const wchar *cat, int l)
{
	int al;
	size_t ep = pend - pbegin;
	if(n > INT_MAX)
		n = INT_MAX;
	wchar *p = Alloc(n, al);
	memcpy_t(p, pbegin, GetLength());
	if(cat) {
		if(ep + l > INT_MAX)
			Panic("WStringBuffer is too big!");
		memcpy_t(p + ep, cat, l);
		ep += l;
	}
	Free();
	pbegin = p;
	pend = pbegin + ep;
	limit = pbegin + al;
}

void WStringBuffer::Expand()
{
	Expand(GetLength() * 2);
	if(pend == limit)
		Panic("WStringBuffer is too big!");
}

void WStringBuffer::SetLength(int l)
{
	if(l > (limit - pbegin))
		Expand(l);
	pend = pbegin + l;
}

void WStringBuffer::Cat(const wchar *s, int l)
{
	if(pend + l > limit)
		Expand(max(GetLength(), l) + GetLength(), s, l);
	else {
		memcpy_t(pend, s, l);
		pend += l;
	}
}

void WStringBuffer::Cat(int c, int l)
{
	if(pend + l > limit)
		Expand(max(GetLength(), l) + GetLength(), NULL, l);
	memset32(pend, c, l);
	pend += l;
}

void WStringBuffer::Set(WString& s)
{
	s.UnShare();
	int l = s.GetLength();
	pbegin = s.ptr;
	pend = pbegin + l;
	limit = pbegin + s.GetAlloc();
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
#ifdef DEPRECATED
	return IterCompare(a.Begin(), a.End(), b.Begin(), b.End(), WStringICompare__());
#else
	return CompareRanges(a, b, WStringICompare__());
#endif
}

int CompareNoCase(const WString& a, const wchar *b)
{
#ifdef DEPRECATED
	return IterCompare(a.Begin(), a.End(), b, b + strlen__(b), WStringICompare__());
#else
	return CompareRanges(a, SubRange(b, b + strlen__(b)), WStringICompare__());
#endif
}

}
