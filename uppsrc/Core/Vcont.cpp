#include "Core.h"

namespace Upp {

struct Vector_ {
	void *vector;
	int   items;
	int   alloc;
};

#ifdef _DEBUG

void *break_when_picked;

void BreakWhenPicked(void *ptr)
{
	if(ptr == break_when_picked)
		__BREAK__;
}

void BREAK_WHEN_PICKED__(void *ptr)
{
	break_when_picked = ptr;
}

#endif

void VectorReAlloc_(void *v_, int newalloc, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	ASSERT(newalloc >= v->items);
	ASSERT(v->items >= 0); // Pick semantics broken
	size_t sz0 = (size_t)newalloc * sizeofT;
	size_t sz = sz0;
	void *newvector = newalloc ? MemoryAllocSz(sz) : NULL;
	v->alloc = newalloc == INT_MAX ? INT_MAX // maximum alloc reached
	           : (int)((sz - sz0) / sizeofT + newalloc); // adjust alloc to real memory size
	if(v->vector)
		memcpy(newvector, v->vector, (size_t)v->items * sizeofT);
	v->vector = newvector;
}

void VectorReAllocF_(void *v_, int newalloc, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	void *prev = v->vector;
	VectorReAlloc_(v, newalloc, sizeofT);
	MemoryFree(prev);
}

void VectorGrow_(void *v_, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	if(v->alloc == INT_MAX)
		Panic("Too many items in container!");
#ifdef _DEBUG
	VectorReAlloc_(v, max(v->alloc + 1, v->alloc >= INT_MAX / 2 ? INT_MAX : 2 * v->alloc), sizeofT);
#else
	VectorReAlloc_(v, max(v->alloc + 1, v->alloc >= int((int64)2 * INT_MAX / 3) ? INT_MAX : v->alloc + (v->alloc >> 1)), sizeofT);
#endif
}

void VectorGrowF_(void *v_, int sizeofT)
{
	Vector_ *v = (Vector_*)v_;
	void *prev = v->vector;
	VectorGrow_(v, sizeofT);
	MemoryFree(prev);
}


int64 NewInVectorSerial()
{
	static int64 x;
	INTERLOCKED {
		++x;
	}
	return x;
}

thread_local int64 invector_cache_serial_;
thread_local int   invector_cache_blki_;
thread_local int   invector_cache_offset_;
thread_local int   invector_cache_end_;

void Bits::Clear()
{
	if(bp)
		MemoryFree(bp);
	alloc = 0;
	bp = NULL;
}

void Bits::Realloc(int nalloc)
{
	size_t sz = sizeof(dword) * nalloc;
	dword *nbp = (dword *)MemoryAllocSz(sz);
	nalloc = int(sz / sizeof(dword));
	if(bp) {
		Copy(nbp, bp, bp + min(alloc, nalloc));
		MemoryFree(bp);
	}
	if(nalloc > alloc)
		Fill(nbp + alloc, nbp + nalloc, (dword)0);
	bp = nbp;
	alloc = nalloc;
}

void Bits::Expand(int q)
{
	Realloc(3 * q / 2 + 1);
}

void Bits::Reserve(int nbits)
{
	int n = (nbits + 31) >> 5;
	if(n > alloc)
		Realloc(n);
}

int Bits::GetLast() const
{
	int lasti = alloc - 1;
	while(lasti > 0 && bp[lasti] == 0)
		lasti--;
	return lasti;
}

void Bits::Shrink()
{
	int nalloc = GetLast() + 1;
	if(nalloc != alloc)
		Realloc(nalloc);
}

dword *Bits::CreateRaw(int n_dwords)
{
	
	Clear();
	Realloc(n_dwords);
	return bp;
}

String Bits::ToString() const
{
	StringBuffer ss;
	for(int i = GetLast(); i >= 0; i--)
		ss << FormatIntHex(bp[i]);
	return ss;
}

void Bits::Serialize(Stream& s)
{
	int dwords;
	if(s.IsStoring())
		dwords = GetLast() + 1;
	s % dwords;
	if(s.IsLoading())
		CreateRaw(dwords);
	s.SerializeRaw(bp, dwords);
}

}
