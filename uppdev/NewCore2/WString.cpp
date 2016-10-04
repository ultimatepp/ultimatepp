#include "Core.h"

#include "HeapImp.h"

static inline void *MAlloc_WS()
{
	MCache& m = mcache[2];
	FreeLink *l = m.list;
	if(l) {
		m.list = l->next;
		m.count--;
		return l;
	}
	else {
		void *ptr = MAlloc_Get(m, 2);
		return ptr;
	}
}

static inline void MFree_WS(void *ptr)
{
	MCache& m = mcache[2];
	((FreeLink *)ptr)->next = m.list;
	m.list = (FreeLink *)ptr;
	if(++m.count > CACHEMAX)
		MFree_Reduce(m, 2);
}

/*
void *MAlloc_WS()         { void *ptr = new byte[24 * sizeof(wchar)]; LOG("Alloc " << ptr); return ptr; }
void  MFree_WS(void *ptr) { LOG("Free " << ptr); delete[] (byte *)ptr; }
*/

wchar *WString::Alloc(int& count)
{
	if(count <= SMALL) {
		count = SMALL;
		wchar *p = (wchar *)MAlloc_WS();
		return p;
	}
	size_t sz = sizeof(Atomic) + (count + 1) * sizeof(wchar);
	Atomic *rc = (Atomic *)MAlloc(sz);
	count = ((sz - sizeof(Atomic)) >> 1) - 1;
	*rc = 1;
	return (wchar *)(rc + 1);
}

void WString::Free()
{
	if(alloc)
		if(IsRc()) {
			Atomic& rc = Rc();
			if(AtomicDec(rc) == 0)
				MFree(&rc);
		}
		else
			MFree_WS(ptr);
}

wchar *WString::Insert(int pos, int count, const wchar *s)
{
	ASSERT(pos >= 0 && count >= 0 && pos <= GetCount());
	int newlen = length + count;
	if(newlen < alloc && !IsShared()/* && !(s >= ptr && s <= ptr + length)*/) { //Optimize!
		if(s >= ptr + pos && s <= ptr + length)
			s += count;
		if(pos < length)
			memmove(ptr + pos + count, ptr + pos, (length - pos) * sizeof(wchar));
		length = newlen;
		ptr[newlen] = 0;
		if(s)
			memcpy(ptr + pos, s, count * sizeof(wchar));
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
	return ptr + pos;
}

void WString::Set(const WString& src)
{
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
		ptr = (wchar *)MAlloc_WS();
		qword *t = (qword *)ptr;
		qword *s = (qword *)src.ptr;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
	}
}

void WString::Cat(const wchar *s, int l)
{
	if(length + l >= alloc || IsShared())
		Insert(length, l, s);
	else {
		memcpy(ptr + length, s, l * sizeof(wchar));
		ptr[length += l] = 0;
	}
}

void WString::LCat(int c)
{
	*Insert(length, 1, NULL) = c;
}

void WString::Set(const wchar *s, int l)
{
	alloc = length = l;
	memcpy(ptr = Alloc(alloc), s, l * sizeof(wchar));
	ptr[l] = 0;
}

void WString::UnShare()
{
	if(!IsShared()) return;
	int al = length;
	wchar *p = Alloc(al);
	memcpy(p, ptr, (length + 1) * sizeof(wchar));
	Free();
	ptr = p;
	alloc = al;
}

void WString::Set(int pos, int ch)
{
	ASSERT(pos >= 0 && pos < GetCount());
	UnShare();
	ptr[pos] = ch;
}

void WString::Trim(int pos)
{
	ASSERT(pos >= 0 && pos <= GetCount());
	UnShare();
	length = pos;
	ptr[pos] = 0;
}

void WString::Remove(int pos, int count)
{
	ASSERT(pos >= 0 && count >= 0 && pos + count <= GetCount());
	UnShare();
	memmove(ptr + pos, ptr + pos + count, (GetCount() - pos - count + 1) * sizeof(wchar));
	length -= count;
}

void WString::Insert(int pos, const wchar *s, int count)
{
	Insert(pos, count, s);
}

WString::WString(WStringBuffer& b)
{
	length = b.GetLength();
	ptr = b.begin;
	ptr[length] = 0;
	alloc = b.GetAlloc();
	b.Zero();
}

void WStringBuffer::Zero()
{
	static wchar h[2];
	begin = end = limit = h;
}

wchar *WStringBuffer::Alloc(int count, int& alloc)
{
	if(count <= 23) {
		wchar *s = (wchar *)MAlloc_WS();
		alloc = WString::SMALL;
		return s;
	}
	else {
		size_t sz = sizeof(Atomic) + (count + 1) * sizeof(wchar);
		Atomic *rc = (Atomic *)MAlloc(sz);
		alloc = ((sz - sizeof(Atomic)) >> 1) - 1;
		*rc = 1;
		return (wchar *)(rc + 1);
	}
}

void WStringBuffer::Free()
{
	int all = limit - begin;
	if(all == WString::SMALL)
		MFree_WS(begin);
	if(all > WString::SMALL)
		MFree((Atomic *)begin - 1);
}

void WStringBuffer::Expand(int n, const wchar *cat, int l)
{
	int al;
	int ep = end - begin;
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

WStringBuffer::WStringBuffer(WString& s)
{
	s.UnShare();
	int l = s.GetLength();
	begin = s.ptr;
	end = begin + l;
	limit = begin + s.GetAlloc();
	s.Zero();
}
