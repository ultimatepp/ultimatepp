#include "Core.h"
#include "HeapImp.h"

namespace Upp {
extern byte *Alloc4KBRaw();
};

#define LTIMING(x)  // RTIMING(x)

static int klass[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
                       16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
                       20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20 };
static int klasz[] = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 288, 352, 448, 576, 672, 800, 992, 1344 };
static int klasl[] = { 4080, 4064, 4016, 4000, 3952, 3968, 3952, 3872, 3920, 3872, 3904, 3872, 3840, 3776, 3552, 3616, 3488, 3392, 3232, 3008, 2720 };
static int klasn[] = { 254, 127, 84, 63, 50, 42, 36, 31, 28, 25, 23, 21, 18, 14, 11, 9, 7, 6, 5, 4, 3 };

#define MPAGE(a, x)     { 0, 0, 0, 0, 0, NULL, a[x], a[x] },

inline
void MPage::Format(int k)
{
	klass = k;
	count = klasn[k];
	freecount = 0;
	free = klasl[k];
	freelist = NULL;
}

#ifdef flagMT
#ifdef COMPILER_MSC
__declspec(thread)
#else
__thread
#endif
#endif
MCache mcache[10];

static MPage sFull[NKLASS][1] = {
#define SFULL(x)   MPAGE(sFull, x)
	SFULL(0)  SFULL(1)  SFULL(2)  SFULL(3)  SFULL(4)  SFULL(5)  SFULL(6)  SFULL(7)
	SFULL(8)  SFULL(9)  SFULL(10) SFULL(11) SFULL(12) SFULL(13) SFULL(14) SFULL(15)
	SFULL(16) SFULL(17) SFULL(18) SFULL(19) SFULL(20)
};

static MPage sWork[NKLASS][1] = {
#define SWORK(x)   MPAGE(sWork, x)
	SWORK(0)  SWORK(1)  SWORK(2)  SWORK(3)  SWORK(4)  SWORK(5)  SWORK(6)  SWORK(7)
	SWORK(8)  SWORK(9)  SWORK(10) SWORK(11) SWORK(12) SWORK(13) SWORK(14) SWORK(15)
	SWORK(16) SWORK(17) SWORK(18) SWORK(19) SWORK(20)
};

static MPage sFree[1] = { MPAGE(&sFree, 0) };

static StaticCriticalSection sHeapLock;


void MCheck()
{
	INTERLOCKED_(sHeapLock) {
		for(int i = 0; i < NKLASS; i++) {
			MPage *p = sWork[i]->next;
			while(p != sWork[i]) {
				ASSERT(p->free <= 4096);
				p = p->next;
			}
		}
	}
}

static inline void *sMAllocK(int k)
{
	MPage *p = sWork[k]->next;
	for(;;) {
		if(p->free >= 32) {
			void *r = (byte *)p + p->free;
			if(p->free > 4096)
				LOGHEXDUMP((byte *)p - 32, 100);
			p->free -= klasz[k];
			return r;
		}
		FreeLink *b = p->freelist;
		if(b) {
			p->freelist = b->next;
			--p->freecount;
			return b;
		}
		if(p->next == p) {
			p = sFree->next;
			if(p->next == p)
				p = (MPage *)Alloc4KBRaw();
			else
				p->Unlink();
			p->Format(k);
			p->Link(sWork[k]);
		}
		else {
			p->Unlink();
			p->Link(sFull[k]);
			p = sWork[k]->next;
		}
	}
}

static inline FreeLink *sMAllocKN(int k)
{
	MPage *p = sWork[k]->next;
	int n = CACHERES;
	int sz = klasz[k];
	FreeLink *l = NULL;
	for(;;) {
		while(p->free >= 32) {
			if(n == 0) return l;
			FreeLink *b = (FreeLink *)((byte *)p + p->free);
			p->free -= sz;
			b->next = l;
			l = b;
			n--;
		}
		for(;;) {
			if(n == 0) return l;
			FreeLink *b = p->freelist;
			if(!b) break;
			p->freelist = b->next;
			--p->freecount;
			b->next = l;
			l = b;
			n--;
		}
		if(p->next == p) {
			p = sFree->next;
			if(p->next == p)
				p = (MPage *)Alloc4KBRaw();
			else
				p->Unlink();
			p->Format(k);
			p->Link(sWork[k]);
		}
		else {
			p->Unlink();
			p->Link(sFull[k]);
			p = sWork[k]->next;
		}
	}
}

static inline void sMFreeK(void *ptr, MPage *p, int k)
{
	((FreeLink *)ptr)->next = p->freelist;
	p->freelist = (FreeLink *)ptr;
	if(p->freecount == 0) {
		p->freecount = 1;
		p->Unlink();
		p->Link(sWork[k]);
	}
	else
		p->freecount++;
	if(p->freecount == p->count) {
		p->Unlink();
		p->Link(sFree);
	}
}

void *MAlloc_Get(MCache& m, int k)
{
	FreeLink *l;
	INTERLOCKED_(sHeapLock)
		l = sMAllocKN(k);
	m.list = l->next;
	m.count = CACHERES - 1;
	return l;
}

void *MAlloc(size_t& size)
{
	LTIMING("MAlloc");
	size_t sz = size;
	if(sz == 0) sz = 1;
	if(sz <= 144) {
		int k = (sz - 1) >> 4;
		MCache& m = mcache[k];
		FreeLink *l = m.list;
		if(l == NULL) {
			ASSERT(m.count == 0);
			INTERLOCKED_(sHeapLock)
				l = sMAllocKN(k);
			m.count = CACHERES;
		}
		m.count--;
		m.list = l->next;
		size = (k + 1) << 4;
		return l;
	}
	if(sz <= 1344) {
		int k = klass[sz];
		size = klasz[k];
		sHeapLock.Enter();
		void *p = sMAllocK(k);
		sHeapLock.Leave();
		return p;
	}
	void *ptr = malloc(sz + 16 + 8);
	uintptr_t p = (uintptr_t)ptr;
	p = (p + 15) & ~(uintptr_t)15;
	*(void **)p = ptr;
	return (void *)(p + 8);
}

inline void MFree_Reduce(MCache& m, int k)
{
	FreeLink *l = m.list;
	INTERLOCKED_(sHeapLock)
		for(int i = 0; i < CACHERES; i++) {
			FreeLink *f = l;
			l = l->next;
			sMFreeK(f, (MPage *)((uintptr_t)f & ~(uintptr_t)4095), k);
		}
	m.list = l;
	m.count -= CACHERES;
}

void MFree(void *ptr)
{
	LTIMING("MFree");
	if(!ptr) return;
	if(((dword)(uintptr_t)ptr) & 8)
		free(*(void **)((byte *)ptr - 8));
	else {
		MPage *p = (MPage *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = p->klass;
		if(k < 9) {
			MCache& m = mcache[k];
			((FreeLink *)ptr)->next = m.list;
			m.list = (FreeLink *)ptr;
			ASSERT(m.count >= 0);
			if(++m.count > CACHEMAX)
				MFree_Reduce(m, k);
		}
		else
			INTERLOCKED_(sHeapLock)
				sMFreeK(ptr, p, k);
	}
}

void MThreadFree()
{
	for(int k = 0; k < 9; k++) {
		MCache& m = mcache[k];
		FreeLink *l = m.list;
		INTERLOCKED_(sHeapLock) {
			while(l) {
				FreeLink *f = l;
				l = l->next;
				sMFreeK(f, (MPage *)((uintptr_t)f & ~(uintptr_t)4095), 1);
			}
			m.list = NULL;
			m.count = 0;
		}
	}
}
