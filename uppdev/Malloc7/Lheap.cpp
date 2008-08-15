#include "Malloc.h"

#define LTIMING(x) // RTIMING(x)

struct MLink;

struct MHeader {
	byte free;
	byte d1, d2, d3;
	word size;
	word prev;

	MLink       *Block()               { return (MLink *)(this + 1); }
	MHeader     *Next()                { return (MHeader *)((byte *)this + size) + 1; }
	MHeader     *Prev()                { return (MHeader *)((byte *)this - prev) - 1; }
};

struct MLink {
	MLink       *next;
	MLink       *prev;

	void         LinkSelf()            { Dbl_Self(this); }
	void         Unlink()              { Dbl_Unlink(this); }
	void         Link(MLink *lnk)      { Dbl_LinkAfter(this, lnk);  }

	MHeader     *Header()              { return (MHeader *)this - 1; }
};

#define LBINS    113
#define MAXBLOCK 65504

static word      sBinSz[LBINS];
static byte      s_SzBin[MAXBLOCK / 8 + 1];
static byte      s_BlBin[MAXBLOCK / 8 + 1];

static MLink s_freebin[LBINS];
static MLink s_mall;
static MLink s_ball;

inline static void sLInit()
{
	int p = 24;
	int bi = 0;
	while(p < MAXBLOCK) {
		sBinSz[bi++] = p;
		int add = minmax(6 * p / 100 / 32 * 32, 32, 2048);
		p += add;
	}
	ASSERT(bi == LBINS - 1);
	sBinSz[LBINS - 1] = MAXBLOCK;
	int k = 0;
	for(int i = 0; i < MAXBLOCK / 8; i++) {
		while(i * 8 + 7 > sBinSz[k])
			k++;
		s_SzBin[i] = k;
	}
	k = LBINS - 1;
	for(int i = MAXBLOCK / 8; i >= 0; i--) {
		while(i * 8 < sBinSz[k]) k--;
		s_BlBin[i] = k;
	}
	s_BlBin[0] = 0;
	for(int i = 0; i < LBINS; i++)
		s_freebin[i].LinkSelf();
	s_mall.LinkSelf();
	s_ball.LinkSelf();
}

inline static int sSzBin(int n)
{
	return s_SzBin[(n - 1) >> 3];
}

inline static int sBlBin(int n)
{
	return s_BlBin[n >> 3];
}

static void sLinkFree(MLink *b, int size)
{
	int q = sBlBin(size);
	b->Link(&s_freebin[q]);
}

static MLink *sAddChunk()
{
	ASSERT(sizeof(MHeader) == 8);
	ASSERT(sizeof(MLink) <= 16);
	MLink *ml = (MLink *)AllocRaw64KB();
	if(!ml) return NULL;
	ml->Link(&s_mall);
	MHeader *bh = (MHeader *)((byte *)ml + 16);
	bh->size = MAXBLOCK;
	bh->prev = 0;
	bh->free = true;
	MLink *b = bh->Block();
	sLinkFree(b, MAXBLOCK);
	bh = bh->Next();
	bh->prev = MAXBLOCK;
	bh->size = 0;
	bh->free = false;
	return b;
}

static void *sDivideBlock(MLink *b, int size, int ii)
{
	b->Unlink();
	MHeader *bh = b->Header();
	ASSERT(bh->size >= size);
	bh->free = false;
	int sz2 = bh->size - size - sizeof(MHeader);
	if(sz2 >= 32) {
		MHeader *bh2 = (MHeader *)((byte *)b + size);
		bh2->prev = size;
		bh2->free = true;
		sLinkFree(bh2->Block(), sz2);
		bh->Next()->prev = bh2->size = sz2;
		bh->size = size;
	}
	return b;
}

static StaticCriticalSection llock;

void *LAlloc(size_t& size) {
	PROFILEMT(llock);
	llock.Enter();
	static bool inited;
	if(!inited) {
		sLInit();
		inited = true;
	}
	if(size > MAXBLOCK) {
		LTIMING("Big alloc");
		sHeapStat(17);
		MLink *b = (MLink *)SysAllocRaw(size + 40);
		if(!b)
			Panic("Out of memory!");
		b->Link(&s_ball);
		size = ((size + 40 + 4095) & ~4095) - 40;
		*(size_t *)((byte *)b + 16) = size;
		MHeader *h = (MHeader *)((byte *)b + 32);
		h->size = 0;
		h->free = false;
		llock.Leave();
		return (byte *)b + 40;
	}
	LTIMING("Large alloc");
	sHeapStat(16);
	if(size < 256)
		size = 256;
	int ii = sSzBin((int)size);
	size = sBinSz[ii];

	while(ii < LBINS) {
		MLink *b = &s_freebin[ii];
		MLink *n = b->next;
		if(b != n) {
			void *ptr = sDivideBlock(n, (int)size, ii);
			llock.Leave();
			sDoPeakProfile();
			return ptr;
		}
		ii++;
	}

	MLink *n = sAddChunk();
	if(!n)
		Panic("Out of memory!");
	void *ptr = sDivideBlock(n, (int)size, LBINS - 1);
	llock.Leave();
	sDoPeakProfile();
	return ptr;
}

void LFree(void *ptr) {
	CriticalSection::Lock __(llock);
	MLink   *b = (MLink *)ptr;
	MHeader *bh = b->Header();
	if(bh->size == 0) {
		LTIMING("Big free");
		ASSERT(((dword)(uintptr_t)bh & 4095) == 32);
		b = (MLink *)((byte *)bh - 32);
		b->Unlink();
		SysFreeRaw(b, *(size_t *)((byte *)b + 16));
		return;
	}
	LTIMING("Large free");
	if(bh->prev) {
		MHeader *p = bh->Prev();
		if(p->free) {
			b = p->Block();
			b->Unlink();
			p->size += bh->size + sizeof(MHeader);
			p->Next()->prev = p->size;
			bh = p;
		}
	}
	MHeader *n = bh->Next();
	if(n->free) {
		n->Block()->Unlink();
		bh->size += n->size + sizeof(MHeader);
		n->Next()->prev = bh->size;
	}
	bh->free = true;
	sLinkFree(b, bh->size);
}

void LMemoryShrink()
{
#ifdef flagMEMSHRINK
	LTIMING("LMemoryShrink");
	CriticalSection::Lock __(llock);
	MLink *m = s_freebin[BINS - 1].next;
	while(m != &s_freebin[BINS - 1]) {
		MLink *cm = (MLink *)((byte *)m - 24);
		MLink *mm = m;
		m = m->next;
		mm->Unlink();
		cm->Unlink();
		SysFreeRaw(cm, 65536);
	}
#endif
}

void LMake(MemoryProfile& p)
{
	CriticalSection::Lock __(llock);
	int ii = 0;
	int fi = 0;
	MLink *m = s_ball.next;
	while(m != &s_ball) {
		int sz = (int)*(size_t *)((byte *)m + 16);
		p.large_count++;
		p.large_total += sz;
		if(ii < 4096)
			p.large_size[ii++] = sz;
		m = m->next;
	}
	m = s_mall.next;
	while(m != &s_mall) {
		MHeader *h = (MHeader *)((byte *)m + 16);
		int sz = h->size;
		if(h->free) {
			p.large_free_count++;
			p.large_free_total += sz;
			if(fi < 4096)
				p.large_free_size[fi++] = sz;
		}
		else {
			p.large_count++;
			p.large_total += sz;
			if(ii < 4096)
				p.large_size[ii++] = sz;
		}
		m = m->next;
	}
}
