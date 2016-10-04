$uvs: PENDING CONFLICT
#include <Core/Core.h>

using namespace Upp;

$uvs: REPOSITORY INSERT
void LMemoryCheck()
{
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
#define RESERVE_CHUNK    4 * 1024 * 1024

static byte **free_raw64_page;
static int    free_raw64_alloc;
static int    free_raw64_count;

int sMediumKb;

byte *Alloc64KBRaw() {
	sMediumKb += 64;
	if(free_raw64_count) {
		byte *ptr = free_raw64_page[--free_raw64_count];
		VirtualAlloc(ptr, 65536, MEM_COMMIT, PAGE_READWRITE);
		return ptr;
	}
	static byte *ptr     = NULL;
	static byte *limit   = NULL;
	if(ptr >= limit) {
		ptr = (byte *) VirtualAlloc(NULL, RESERVE_CHUNK, MEM_RESERVE, PAGE_READWRITE);
		limit = ptr + RESERVE_CHUNK;
	}
	VirtualAlloc(ptr, 65536, MEM_COMMIT, PAGE_READWRITE);
	byte *p = ptr;
	ptr += 65536;
	return p;
}

void Free64KBRaw(byte *ptr)
{
	sMediumKb -= 64;
	if(free_raw64_count >= free_raw64_alloc) {
		byte **prev = free_raw64_page;
		if(free_raw64_alloc)
			free_raw64_alloc *= 2;
		else
			free_raw64_alloc = 100000;
		free_raw64_page = (byte **)malloc(sizeof(byte *) * free_raw64_alloc);
		if(prev) {
			memcpy(free_raw64_page, prev, free_raw64_count * sizeof(byte *));
			free(prev);
		}
	}
	free_raw64_page[free_raw64_count++] = ptr;
	VirtualFree(ptr, 65536, MEM_DECOMMIT);
}

void *AllocBigRaw(size_t size)
{
	return VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

void  FreeBigRaw(void *ptr, size_t size)
{
	VirtualFree(ptr, 0, MEM_RELEASE);
}

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

	void         LinkSelf()            { prev = next = this; }
	void         Unlink()              { prev->next = next; next->prev = prev; }
	void         Link(MLink *lnk)      { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }

	MHeader     *Header()              { return (MHeader *)this - 1; }
};

#define BINS     113
#define MAXBLOCK 65504

static word      BinSz[BINS];
static byte      sSzBin[MAXBLOCK / 8 + 1];
static byte      sBlBin[MAXBLOCK / 8 + 1];

static MLink freebin[BINS];
static int   binaccel[BINS / 8 + 1];
static MLink mall;
static MLink ball;

void Init()
{
	int p = 24;
	int bi = 0;
	while(p < MAXBLOCK) {
		BinSz[bi++] = p;
		int add = minmax(6 * p / 100 / 32 * 32, 32, 2048);
		p += add;
	}
	ASSERT(bi == BINS - 1);
	BinSz[BINS - 1] = MAXBLOCK;
	int k = 0;
	for(int i = 0; i <= MAXBLOCK / 8; i++) {
		while(i * 8 + 7 > BinSz[k]) k++;
		sSzBin[i] = k;
	}
	k = BINS - 1;
	for(int i = MAXBLOCK / 8; i >= 0; i--) {
		while(i * 8 < BinSz[k]) k--;
		sBlBin[i] = k;
	}
	sBlBin[0] = 0;
	for(int i = 0; i < BINS; i++)
		freebin[i].LinkSelf();
	mall.LinkSelf();
	ball.LinkSelf();
	binaccel[BINS / 8] = 1;
}

inline int SzBin(int n)
{
	return sSzBin[(n - 1) >> 3];
}

inline int BlBin(int n)
{
	return sBlBin[n >> 3];
}

void LinkFree(MLink *b, int size)
{
	int q = BlBin(size);
	b->Link(&freebin[q]);
$uvs: YOUR INSERT (REPOSITORY DELETE)
//	binaccel[q >> 3]++;
$uvs: END YOUR INSERT (REPOSITORY DELETE)
$uvs: YOUR DELETE (REPOSITORY DELETE)
	binaccel[q >> 3]++;
$uvs: END YOUR DELETE (REPOSITORY DELETE)
}

MLink *AddChunk()
{
	ASSERT(sizeof(MHeader) == 8);
	ASSERT(sizeof(MLink) <= 16);
	MLink *ml = (MLink *)Alloc64KBRaw();
	ml->Link(&mall);
	MHeader *bh = (MHeader *)((byte *)ml + 16);
	bh->size = MAXBLOCK;
	bh->prev = 0;
	bh->free = true;
	MLink *b = bh->Block();
	LinkFree(b, MAXBLOCK);
	bh = bh->Next();
	bh->prev = MAXBLOCK;
	bh->size = 0;
	bh->free = false;
	return b;
}

void *DivideBlock(MLink *b, int size, int ii)
{
	b->Unlink();
$uvs: YOUR INSERT (REPOSITORY DELETE)
//	binaccel[ii >> 3]--;
$uvs: END YOUR INSERT (REPOSITORY DELETE)
$uvs: YOUR DELETE (REPOSITORY DELETE)
	binaccel[ii >> 3]--;
$uvs: END YOUR DELETE (REPOSITORY DELETE)
	MHeader *bh = b->Header();
	ASSERT(bh->size >= size);
	bh->free = false;
	int sz2 = bh->size - size - sizeof(MHeader);
	if(sz2 >= 32) {
		MHeader *bh2 = (MHeader *)((byte *)b + size);
		bh2->prev = size;
		bh2->free = true;
		LinkFree(bh2->Block(), sz2);
		bh->Next()->prev = bh2->size = sz2;
		bh->size = size;
	}
	return b;
}

void *Alloc(size_t size) {
	static inited;
	if(!inited) {
		Init();
		inited = true;
	}
	if(size > MAXBLOCK) {
		MLink *b = (MLink *)AllocBigRaw(size + 40);
		b->Link(&ball);
		*(size_t *)((byte *)b + 16) = size;
		MHeader *h = (MHeader *)((byte *)b + 32);
		h->size = 0;
		h->free = false;
		return (byte *)b + 40;
	}
	if(size < 256)
		size = 256;
	int ii = SzBin(size);
	size = BinSz[ii];

	while(ii < BINS) {
/*		int q = ii >> 3;
		if(binaccel[q] == 0) {
			while(binaccel[q] == 0) q++;
			ii = q << 3;
		}
		else {*/
			MLink *b = &freebin[ii];
			MLink *n = b->next;
			if(b != n)
				return DivideBlock(n, size, ii);
			ii++;
//		}
	}

	return DivideBlock(AddChunk(), size, BINS - 1);
}

void Free(void *ptr) {
	MLink   *b = (MLink *)ptr;
	MHeader *bh = b->Header();
	if(bh->size == 0) {
		ASSERT(((dword)bh & 4095) == 32);
		b = (MLink *)((byte *)bh - 32);
		b->Unlink();
		FreeBigRaw(b, *(size_t *)((byte *)b + 16));
		return;
	}
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
	LinkFree(b, bh->size);
$uvs: END REPOSITORY DELETE
}

void DumpHeap()
{
	LOG("--------------------------------------------------------");
$uvs: REPOSITORY INSERT
	LOG("heap");
	MLink *m = s_mall.next;
	while(m != &s_mall) {
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	MLink *m = mall.next;
	while(m != &mall) {
$uvs: END REPOSITORY DELETE
		MHeader *h = (MHeader *)((byte *)m + 16);
		while(h->size) {
			LOG((h->free ? "FREE" : "USED") << " " << h->size);
			h = h->Next();
		}
		m = m->next;
		LOG("===========");
	}
$uvs: REPOSITORY INSERT
	LOG("......");
$uvs: END REPOSITORY INSERT
}

void   *ptr[8192];

$uvs: REPOSITORY INSERT
void *LAlloc(int sz) { size_t s = sz; return LAlloc(s); }

$uvs: END REPOSITORY INSERT
void Benchmark()
{
	RTIMING("Benchmark");
	void *ptr[8192];
$uvs: REPOSITORY INSERT
	for(int q = 0; q < 20; q++) {
		for(int i = 0; i < 8192; i++)
			ptr[i] = LAlloc(2048);
		for(int i = 0; i < 8192; i++)
			LFree(ptr[i]);
$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	for(int q = 0; q < 2000; q++) {
		for(int i = 0; i < 8192; i++)
			ptr[i] = Alloc(2048);
		for(int i = 0; i < 8192; i++)
			Free(ptr[i]);
$uvs: END REPOSITORY DELETE
	}
}

CONSOLE_APP_MAIN {
$uvs: REPOSITORY INSERT
	sLInit();
	for(int i = 0; i < BINS; i++)
		LOG(i << ": " << sBinSz[i]);
	for(int i = 32; i <= MAXBLOCK; i++)
		LOG(i << ": bin " << (int)sSzBin(i) << " - " << sBinSz[sSzBin(i)]
		      << ", bl " << (int)sBlBin(i) << " - " << sBinSz[sBlBin(i)]);

$uvs: END REPOSITORY INSERT
	Benchmark();
	DumpHeap();
$uvs: REPOSITORY INSERT
	LMemoryShrink();
	DumpHeap();
	memset(ptr, 0, sizeof(ptr));
	for(int n = 1; n < 5000000; n++) {
		int q = rand() & 511;
		if(ptr[q])
			LFree(ptr[q]);
		int sz = 5 * ((rand() & 16383) + 270);
		ptr[q] = LAlloc(sz);
	}
	DumpHeap();
	for(int q = 0; q < 512; q++)
		if(ptr[q])
			LFree(ptr[q]);
	DumpHeap();
	LMemoryShrink();
	LOG("Final");
	DumpHeap();
	return;


	void *p1, *p2, *p3;
	p1 = LAlloc(500);
	DumpHeap();
	p2 = LAlloc(300);
	DumpHeap();
	p3 = LAlloc(400);
	DumpHeap();
	LFree(p2);
	DumpHeap();
	LFree(p1);
	DumpHeap();
	LFree(p3);
	DumpHeap();

}

$uvs: END REPOSITORY INSERT
$uvs: REPOSITORY DELETE
	return;

	Init();
	for(int i = 0; i < BINS; i++)
		LOG(i << ": " << BinSz[i]);
	for(int i = 32; i <= MAXBLOCK; i++)
		LOG(i << ": bin " << (int)SzBin(i) << " - " << BinSz[SzBin(i)]
		      << ", bl " << (int)BlBin(i) << " - " << BinSz[BlBin(i)]);

	void *p1, *p2, *p3;
	p1 = Alloc(500);
	DumpHeap();
	p2 = Alloc(300);
	DumpHeap();
	p3 = Alloc(400);
	DumpHeap();
	Free(p2);
	DumpHeap();
	Free(p1);
	DumpHeap();
	Free(p3);
	DumpHeap();

	memset(ptr, 0, sizeof(ptr));
	for(int n = 1; n < 500; n++) {
		int q = rand() & 63;
		if(ptr[q])
			Free(ptr[q]);
		DumpHeap();
		int sz = (rand() & 16383) + 270;
		DUMP(sz);
		ptr[q] = Alloc(sz);
		DumpHeap();
	}
	for(int q = 0; q < 64; q++)
		if(ptr[q]) {
			Free(ptr[q]);
			DumpHeap();
		}
}

$uvs: END REPOSITORY DELETE
