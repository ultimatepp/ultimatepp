#include "Malloc.h"

#define LTIMING(x) // RTIMING(x)

struct Link;

word  Heap::BinSz[LBINS];
byte  Heap::SzBin[MAXBLOCK / 8 + 1];
byte  Heap::BlBin[MAXBLOCK / 8 + 1];

Heap::DLink Heap::allmedium;
Heap::DLink Heap::allbig;

void Heap::GlobalLInit()
{
	ONCELOCK {
		int p = 24;
		int bi = 0;
		while(p < MAXBLOCK) {
			BinSz[bi++] = p;
			int add = minmax(6 * p / 100 / 32 * 32, 32, 2048);
			p += add;
		}
		ASSERT(bi == LBINS - 1);
		BinSz[LBINS - 1] = MAXBLOCK;
		int k = 0;
		for(int i = 0; i < MAXBLOCK / 8; i++) {
			while(i * 8 + 7 > BinSz[k])
				k++;
			SzBin[i] = k;
		}
		k = LBINS - 1;
		for(int i = MAXBLOCK / 8; i >= 0; i--) {
			while(i * 8 < BinSz[k]) k--;
			BlBin[i] = k;
		}
		BlBin[0] = 0;
		allmedium.LinkSelf();
		allbig.LinkSelf();
	}
}

void Heap::LInit()
{
	ASSERT(sizeof(Header) == 8);
	ASSERT(sizeof(DLink) <= 16);

	GlobalLInit();
	for(int i = 0; i < LBINS; i++)
		freebin[i]->LinkSelf();
}

Heap::DLink *Heap::AddChunk()
{
	Mutex::Lock __(global_lock);
	DLink *ml = (DLink *)AllocRaw64KB();
	if(!ml) return NULL;
	ml->Link(&allmedium);
	Header *bh = (Header *)((byte *)ml + 16);
	bh->size = MAXBLOCK;
	bh->prev = 0;
	bh->free = true;
//	bh->heap = this; //!!!!!
	DLink *b = bh->GetBlock();
	LinkFree(b, MAXBLOCK);
	bh = bh->Next(); 
	bh->prev = MAXBLOCK;
	bh->size = 0;
	bh->free = false;
	return b;
}

void *Heap::DivideBlock(DLink *b, int size, int ii)
{
	b->Unlink();
	Header *bh = b->GetHeader();
	ASSERT(bh->size >= size);
	bh->free = false;
	int sz2 = bh->size - size - sizeof(Header);
	if(sz2 >= 32) {
		Header *bh2 = (Header *)((byte *)b + size);
		bh2->prev = size;
		bh2->free = true;
//		bh2->heap = this; // Heap here!!!
		LinkFree(bh2->GetBlock(), sz2);
		bh->Next()->prev = bh2->size = sz2;
		bh->size = size;
	}
	return b;
}

void *Heap::LAlloc(size_t& size) {
	ASSERT(size > 256);
	static bool inited;
	if(!inited) {
		LInit();
		inited = true;
	}
	if(size > MAXBLOCK) {
		Mutex::Lock __(global_lock);
		LTIMING("Big alloc");
		sHeapStat(17);
		DLink *b = (DLink *)SysAllocRaw(size + 40);
		if(!b)
			Panic("Out of memory!");
		b->Link(&allbig);
		size = ((size + 40 + 4095) & ~4095) - 40;
		*(size_t *)((byte *)b + 16) = size;
		Header *h = (Header *)((byte *)b + 32);
		h->size = 0;
		h->free = false;
		return (byte *)b + 40;
	}
	sHeapStat(16);
	int ii = SizeToBin((int)size);
	size = BinSz[ii];
	while(ii < LBINS) {
		DLink *b = freebin[ii];
		DLink *n = b->next;
		if(b != n) {
			void *ptr = DivideBlock(n, (int)size, ii);
			sDoPeakProfile();
			return ptr;
		}
		ii++;
	}

	DLink *n = AddChunk();
	if(!n)
		Panic("Out of memory!");
	void *ptr = DivideBlock(n, (int)size, LBINS - 1);
	sDoPeakProfile();
	return ptr;
}

void Heap::LFree(void *ptr) {
	DLink  *b = (DLink *)ptr;
	Header *bh = b->GetHeader();
	if(bh->size == 0) {
		Mutex::Lock __(global_lock);
		LTIMING("Big free");
		ASSERT(((dword)(uintptr_t)bh & 4095) == 32);
		b = (DLink *)((byte *)bh - 32);
		b->Unlink();
		SysFreeRaw(b, *(size_t *)((byte *)b + 16));
		return;
	}
	LTIMING("Large free");
	if(bh->prev) {
		Header *p = bh->Prev();
		if(p->free) {
			b = p->GetBlock();
			b->Unlink();
			p->size += bh->size + sizeof(Header);
			p->Next()->prev = p->size;
			bh = p;
		}
	}
	Header *n = bh->Next();
	if(n->free) {
		n->GetBlock()->Unlink();
		bh->size += n->size + sizeof(Header);
		n->Next()->prev = bh->size;
	}
	bh->free = true;
	LinkFree(b, bh->size);
}

/*
void LMake(MemoryProfile& p)
{
	CriticalSection::Lock __(llock);
	int ii = 0;
	int fi = 0;
	Link *m = s_ball.next;
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
		Header *h = (Header *)((byte *)m + 16);
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
*/
