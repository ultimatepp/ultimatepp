// These are String methods which are best inlined in heap allocator

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
		ptr = (char *)MemoryAlloc32_i();
		memcpy(qptr, s.qptr, 32); // optimizes to movups
	}
}

void String0::LFree()
{
	if(IsRef()) {
		if(ptr != (char *)(voidptr + 1)) {
			Rc *rc = Ref();
			ASSERT(rc->refcount > 0);
			if(AtomicDec(rc->refcount) == 0) MemoryFree(rc);
		}
	}
	else
		MemoryFree32_i(ptr);
}

force_inline
char *String0::Alloc_(int count, char& kind)
{
	if(count < 32) {
		kind = MEDIUM;
		return (char *)MemoryAlloc32_i();
	}
	size_t sz = sizeof(Rc) + count + 1;
	Rc *rc = (Rc *)MemoryAllocSz(sz);
	rc->alloc = count == INT_MAX ? INT_MAX : (int)sz - sizeof(Rc) - 1;
	rc->refcount = 1;
	kind = min(rc->alloc, 255);
	return rc->GetPtr();
}

char *String0::Alloc(int count, char& kind)
{
	return Alloc_(count, kind);
}

void String0::SetL(const char *s, int len)
{
	ptr = Alloc_(len, chr[KIND]);
	memcpy8(ptr, s, len);
	ptr[len] = 0;
	LLen() = len;
	SLen() = 15;
}

void String0::LCat(int c)
{
	if(IsSmall()) {
		qword *x = (qword *)MemoryAlloc32_i();
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

char *StringBuffer::Alloc(int count, int& alloc)
{
	if(count <= 31) {
		char *s = (char *)MemoryAlloc32_i();
		alloc = 31;
		return s;
	}
	else {
		size_t sz = sizeof(Rc) + count + 1;
		Rc *rc = (Rc *)MemoryAlloc(sz);
		alloc = rc->alloc = (int)min((size_t)INT_MAX, sz - sizeof(Rc) - 1);
		rc->refcount = 1;
		return (char *)(rc + 1);
	}
}

void StringBuffer::Set(String& s)
{
	s.UnShare();
	int l = s.GetLength();
	if(s.GetAlloc() == 14) {
		pbegin = (char *)MemoryAlloc32_i();
		limit = pbegin + 31;
		memcpy8(pbegin, s.Begin(), l);
		pend = pbegin + l;
	}
	else {
		pbegin = s.ptr;
		pend = pbegin + l;
		limit = pbegin + s.GetAlloc();
	}
	s.Zero();
}

void StringBuffer::Free()
{
	if(pbegin == buffer)
		return;
	int all = (int)(limit - pbegin);
	if(all == 31)
		MemoryFree32_i(pbegin);
	if(all > 31)
		MemoryFree((Rc *)pbegin - 1);
}
