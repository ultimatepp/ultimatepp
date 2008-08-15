#include <Core/Core.h>

using namespace Upp;

namespace Upp {
extern byte *Alloc4KBRaw();
};

#define LTIMING(x)  // RTIMING(x)

#define NKLASS      21
#define CACHEMAX    31
#define CACHERES    (CACHEMAX / 2)
#define THREADCACHE
//#define THREADCACHE2

static int klass[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
                       16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
                       20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20 };
static int klasz[] = { 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 288, 352, 448, 576, 672, 800, 992, 1344 };
static int klasl[] = { 4080, 4064, 4016, 4000, 3952, 3968, 3952, 3872, 3920, 3872, 3904, 3872, 3840, 3776, 3552, 3616, 3488, 3392, 3232, 3008, 2720 };
static int klasn[] = { 254, 127, 84, 63, 50, 42, 36, 31, 28, 25, 23, 21, 18, 14, 11, 9, 7, 6, 5, 4, 3 };

struct FreeLink {
	FreeLink *next;
};

struct MPage {
	byte         klass;
	byte         freecount;
	byte         count;
	byte         filler;
	int          free;
	FreeLink    *freelist;
	MPage       *next;
	MPage       *prev;
#ifdef CPU_32
	byte         align32[12];
#endif

	void         LinkSelf()        { prev = next = this; }
	void         Unlink()          { prev->next = next; next->prev = prev; }
	void         Link(MPage *lnk)  { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }

	void         Format(int k);
};

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

#ifdef THREADCACHE
struct MCache {
	FreeLink  *list;
	int32      count;
};
#endif

#ifdef THREADCACHE2
struct MCache {
	void      *block[CACHEMAX];
	int        count;
#ifdef CPU_64
	int        filler; // alignment to 256 bytes
#endif
};
#endif

#ifdef flagMT
__declspec(thread)
#endif
MCache mcache[10];


static MPage sFull[NKLASS][1] = {
#define SFULL(x)   MPAGE(sFull, x)
	SFULL(0)  SFULL(1)  SFULL(2)  SFULL(3)  SFULL(4)  SFULL(5)  SFULL(6)  SFULL(7)
	SFULL(8)  SFULL(9)  SFULL(10) SFULL(11) SFULL(12) SFULL(13) SFULL(14) SFULL(15)
	SFULL(17) SFULL(18) SFULL(19) SFULL(20)
};

static MPage sWork[NKLASS][1] = {
#define SWORK(x)   MPAGE(sWork, x)
	SWORK(0)  SWORK(1)  SWORK(2)  SWORK(3)  SWORK(4)  SWORK(5)  SWORK(6)  SWORK(7)
	SWORK(8)  SWORK(9)  SWORK(10) SWORK(11) SWORK(12) SWORK(13) SWORK(14) SWORK(15)
	SWORK(17) SWORK(18) SWORK(19) SWORK(20)
};

static MPage sFree[1] = { MPAGE(&sFree, 0) };

static StaticCriticalSection sHeapLock;

inline void *MAllocK(int k)
{
	MPage *p = sWork[k]->next;
	for(;;) {
		if(p->free >= 32) {
			void *r = (byte *)p + p->free;
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

FreeLink *MAllocKN(int k)
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

inline void MFreeK(void *ptr, MPage *p, int k)
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
				l = MAllocKN(k);
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
		void *p = MAllocK(k);
		sHeapLock.Leave();
		return p;
	}
	void *ptr = malloc(sz + 16 + 8);
	uintptr_t p = (uintptr_t)ptr;
	p = (p + 15) & ~(uintptr_t)15;
	*(void **)p = ptr;
	return (void *)(p + 8);
}

void MFree(void *ptr)
{
	LTIMING("MFree");
	if(!ptr) return;
	if(((dword)ptr) & 8)
		free(*(void **)((byte *)ptr - 8));
	else {
		MPage *p = (MPage *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = p->klass;
		if(k < 9) {
			MCache& m = mcache[k];
			((FreeLink *)ptr)->next = m.list;
			m.list = (FreeLink *)ptr;
			ASSERT(m.count >= 0);
			if(++m.count > CACHEMAX) {
				FreeLink *l = m.list;
				INTERLOCKED_(sHeapLock)
					for(int i = 0; i < CACHERES; i++) {
						FreeLink *ptr = l;
						l = l->next;
						MFreeK(ptr, (MPage *)((uintptr_t)ptr & ~(uintptr_t)4095), k);
					}
				m.list = l;
				m.count -= CACHERES;
			}
		}
		else
			INTERLOCKED_(sHeapLock)
				MFreeK(ptr, p, k);
	}
}

void *MAlloc_Get(MCache& m)
{
	FreeLink *l;
	INTERLOCKED_(sHeapLock) {
		l = MAllocKN(1);
	}
	m.list = l->next;
	m.count = CACHERES - 1;
	return l;
}

inline void *MAlloc_32()
{
	MCache& m = mcache[1];
	FreeLink *l = m.list;
	if(l) {
		m.list = l->next;
		m.count--;
		return l;
	}
	else
		return MAlloc_Get(m);
}

void MFree_Reduce(MCache& m)
{
	FreeLink *l = m.list;
	INTERLOCKED_(sHeapLock)
		for(int i = 0; i < CACHERES; i++) {
			FreeLink *f = l;
			l = l->next;
			MFreeK(f, (MPage *)((uintptr_t)f & ~(uintptr_t)4095), 1);
		}
	m.list = l;
	m.count -= CACHERES;
}

inline void MFree_32(void *ptr)
{
	MCache& m = mcache[1];
	((FreeLink *)ptr)->next = m.list;
	m.list = (FreeLink *)ptr;
	if(++m.count > CACHEMAX)
		MFree_Reduce(m);
}

/*
void MFlush()
{
	for(int k = 0; k < 9; k++) {
		MCache& m = mcache[k];
		INTERLOCKED_(sHeapLock)
			for(int i = 0; i < m.count; i++) {
				void *f = m.block[i];
				MFreeK(f, (MPage *)((uintptr_t)f & ~(uintptr_t)4095), k);
			}
	}
}
*/

void Inc(int& a);
void Dec(int& a);

void Benchmark();
void Benchmark2();
void Benchmark3();

CONSOLE_APP_MAIN {
	Benchmark();
	RLOG("---------");
	Benchmark2();
	RLOG("---------");
	Benchmark3();
	return;
	size_t sz;
	void *p = MAlloc(sz = 5000);
	MFree(p);
}

void Benchmark() {
	size_t sz;
	TimeStop tm1;
	void *p = MAlloc(sz = 25);
	for(int i = 0; i < 200000000; i++) {
		void *ss = MAlloc(sz = 25);
		MFree(ss);
	}
	RDUMP(tm1.Elapsed());
	TimeStop tm2;
	p = MemoryAlloc(25);
	for(int i = 0; i < 200000000; i++) {
		void *ptr = MemoryAlloc(sz = 25);
		MemoryFree(ptr);
	}
	RDUMP(tm2.Elapsed());
}

#define B2N 50000000
#define AND 127
#define RND 127

void Benchmark2() {
	size_t sz;
	static void *ptr[AND + 1];
///*
	TimeStop tm1;
	for(int i = 0; i < AND + 1; i++)
		ptr[i & AND] = MAlloc(sz = rand() & RND);
	for(int i = 0; i < B2N; i++) {
		*(byte *)ptr[i & AND] = 0;
	//	__BREAK__;
		MFree(ptr[i & AND]);
		ptr[i & AND] = MAlloc(sz = rand() & RND);
	}
	for(int i = 0; i < AND + 1; i++)
		MFree(ptr[i & AND]);
	RDUMP(tm1.Elapsed());
//*/
//*
	TimeStop tm2;
	for(int i = 0; i < AND + 1; i++)
		ptr[i & AND] = MemoryAlloc(sz = rand() & RND);
	for(int i = 0; i < B2N; i++) {
		*(byte *)ptr[i & AND] = 0;
		MemoryFree(ptr[i & AND]);
		ptr[i & AND] = MemoryAlloc(sz = rand() & RND);
	}
	for(int i = 0; i < AND + 1; i++)
		MemoryFree(ptr[i & AND]);
	RDUMP(tm2.Elapsed());
//*/
}

void Benchmark3() {
	size_t sz;
	static void *ptr[65536];
	TimeStop tm1;
	for(int j = 0; j < 100; j++) {
		for(int i = 0; i < 65536; i++)
			ptr[i] = MAlloc(sz = rand() & 127);
		for(int i = 0; i < 65536; i++)
			MFree(ptr[i]);
	}
	RDUMP(tm1.Elapsed());
	TimeStop tm2;
	for(int j = 0; j < 100; j++) {
		for(int i = 0; i < 65536; i++)
			ptr[i] = MemoryAlloc(sz = rand() & 127);
		for(int i = 0; i < 65536; i++)
			MemoryFree(ptr[i]);
	}
	RDUMP(tm2.Elapsed());
}

void Benchmark4() {
	size_t sz;
	TimeStop tm1;
//	void *p = MAlloc(sz = 25);
	String s("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	for(int i = 0; i < 200000000; i++) {
		dword *ss = (dword *)MAlloc_32();
		ss[0] = 'a';
		ss[1] = 0;
		dword *t = (dword *)MAlloc_32();
		t[0] = ss[0];
		t[1] = ss[1];
		t[2] = ss[2];
		t[3] = ss[3];
		t[4] = ss[4];
		t[5] = ss[5];
		t[6] = ss[6];
		t[7] = ss[7];
		MFree_32(t);
		MFree_32(ss);
	}
	RDUMP(tm1.Elapsed());
	TimeStop tm2;
	void *p = MemoryAlloc(25);
	for(int i = 0; i < 200000000; i++) {
		void *ptr = MemoryAlloc(sz = 25);
		MemoryFree(ptr);
	}
	RDUMP(tm2.Elapsed());
	TimeStop tm3;
	for(int i = 0; i < 200000000; i++) {
		String a('a', 1);
		String b = a;
	}
	RDUMP(tm3.Elapsed());
/*	TimeStop tm3;
	for(int i = 0; i < 50000000; i++) {
		int x = 1;
		Inc(x);
		Dec(x);
		Dec(x);
	}
	RDUMP(tm3.Elapsed());
	TimeStop tm4;
	for(int i = 0; i < 50000000; i++) {
		String a('a', 1);
	}
	RDUMP(tm4.Elapsed());


	String src('1', 1024);
	for(int i = 0; i < 50000000; i++) {
		HString a('a', 1);
		void *ptr = MAlloc(sz = 256);
		dword *t = (dword *)ptr;
		dword *s = (dword *)~src;

		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		t += 8; s += 8;
		t[0] = s[0];
		t[1] = s[1];
		t[2] = s[2];
		t[3] = s[3];
		t[4] = s[4];
		t[5] = s[5];
		t[6] = s[6];
		t[7] = s[7];

		MFree(ptr);
	}
	RDUMP(tm6.Elapsed());
*/
}
