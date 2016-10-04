#include "Malloc.h"

#define LTIMING(x) // RTIMING(x)

word  Heap::BinSz[LBINS];
byte  Heap::SzBin[MAXBLOCK / 8 + 1];
byte  Heap::BlBin[MAXBLOCK / 8 + 1];

Heap::DLink Heap::lempty[1];

void Heap::GlobalLInit()
{
	ONCELOCK {
		int p = 32;
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
		big->LinkSelf();
		lempty->LinkSelf();
	}
}

inline
void Heap::LinkFree(DLink *b, int size)
{
	int q = BlBin[size >> 4];
	b->Link(freebin[q]);
}

Heap::DLink *Heap::AddChunk()
{
	DLink *ml;
	if(lempty->next != lempty) {
		ml = lempty->next;
		ml->Unlink();
		LLOG("Retrieved empty large " << (void *)ml);
	}
	else {
		ml = (DLink *)AllocRaw64KB();
		LLOG("AllocRaw64KB " << (void *)ml);
	}
	lcount++;
	LLOG("lcount = " << lcount);
	if(!ml) return NULL;
	ml->Link(large);
	Header *bh = (Header *)((byte *)ml + LARGEHDRSZ);
	bh->size = MAXBLOCK;
	bh->prev = 0;
	bh->free = true;
	bh->heap = this;
	DLink *b = bh->GetBlock();
	LinkFree(b, MAXBLOCK);
	DbgFreeFill(b + 1, MAXBLOCK - sizeof(DLink));
	bh = bh->Next();
	bh->prev = MAXBLOCK;
	bh->size = 0;
	bh->free = false;
	bh->heap = this;
	return b;
}

inline
void *Heap::DivideBlock(DLink *b, int size, int ii)
{
	b->Unlink();
	Header *bh = b->GetHeader();
	ASSERT(bh->size >= size && size > 0);
	bh->free = false;
	int sz2 = bh->size - size - sizeof(Header);
	if(sz2 >= 32) {
		Header *bh2 = (Header *)((byte *)b + size);
		bh2->prev = size;
		bh2->free = true;
		bh2->heap = this;
		LinkFree(bh2->GetBlock(), sz2);
		bh->Next()->prev = bh2->size = sz2;
		bh->size = size;
	}
	DbgFreeCheck(b + 1, size - sizeof(DLink));
	return b;
}

void Heap::MoveLarge(Heap *dest, DLink *l)
{
	dest->lcount++;
	LLOG("Moving large " << (void *)l << " to " << (void *)dest << " lcount " << dest->lcount);
	Mutex::Lock __(mutex);
	l->Unlink();
	l->Link(dest->large);
	Header *h = (Header *)((byte *)l + LARGEHDRSZ);
	while(h->size) {
		h->heap = dest;
		if(h->free) {
			DLink *b = h->GetBlock();
			b->Unlink();
			dest->LinkFree(b, h->size);
		}
		h = h->Next();
	}
	aux.lcount = 10000;
}

void Heap::MoveToEmpty(DLink *l, Header *bh)
{
	LLOG("Moving empty large " << (void *)l << " to global empty storage, lcount " << lcount);
	bh->GetBlock()->Unlink();
	l->Unlink();
	Mutex::Lock __(mutex);
	l->Link(lempty);
	aux.lcount = 10000;
}

inline
void *Heap::TryLAlloc(int ii, int size)
{
	while(ii < LBINS) {
		if(freebin[ii] != freebin[ii]->next) {
			void *ptr = DivideBlock(freebin[ii]->next, (int)size, ii);
			LLOG("TryLAlloc succeeded " << (void *)ptr);
			ASSERT((size_t)ptr & 8);
			return ptr;
		}
		ii++;
	}
	return NULL;
}

int sBig__;

void *Heap::LAlloc(size_t& size) {
	LLOG("LAlloc " << size);
	ASSERT(size > 256);
	if(!initialized)
		Init();
	if(size > MAXBLOCK) {
		Mutex::Lock __(mutex);
		BigHdr *h = (BigHdr *)SysAllocRaw(size + BIGHDRSZ);
		if(!h)
			Panic("Out of memory!");
		h->Link(big);
		h->size = size = ((size + BIGHDRSZ + 4095) & ~4095) - BIGHDRSZ;
		Header *b = (Header *)((byte *)h + BIGHDRSZ - sizeof(Header));
		b->size = 0;
		b->free = false;
		sBig__++;
		LLOG("Big alloc " << (void *)b->GetBlock());
		return b->GetBlock();
	}
	int bini = SizeToBin((int)size);
	size = BinSz[bini];
	void *ptr = TryLAlloc(bini, size);
	if(ptr)
		return ptr;
	Mutex::Lock __(mutex);
	if(remote_free) {
		FreeRemoteRaw();
		ptr = TryLAlloc(bini, size);
		if(ptr) return ptr;
	}
	aux.FreeRemoteRaw();
	while(aux.large->next != aux.large) {
		LLOG("Adopting large block " << (void *)aux.large->next);
		MoveLarge(this, aux.large->next);
		lcount++;
		ptr = TryLAlloc(bini, size);
		if(ptr) return ptr;
	}
	DLink *n = AddChunk();
	if(!n)
		Panic("Out of memory!");
	ptr = DivideBlock(n, (int)size, LBINS - 1);
	LLOG("LAlloc via AddChunk " << (void *)ptr);
	ASSERT((size_t)ptr & 8);
	return ptr;
}

void Heap::LFree(void *ptr) {
	DLink  *b = (DLink *)ptr;
	Header *bh = b->GetHeader();
	if(bh->size == 0) {
		Mutex::Lock __(mutex);
		ASSERT(((dword)(uintptr_t)bh & 4095) == BIGHDRSZ - sizeof(Header));
		BigHdr *h = (BigHdr *)((byte *)ptr - BIGHDRSZ);
		h->Unlink();
		LLOG("Big free " << (void *) ptr << " size " << h->size);
		SysFreeRaw(h, h->size);
		sBig__--;
		return;
	}
	if(bh->heap != this) {
		LLOG("Remote large, heap " << (void *)bh->heap);
		bh->heap->RemoteFree(ptr);
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
	DbgFreeFill(b + 1, bh->size - sizeof(DLink));
	LLOG("Freed, joined size " << bh->size << " lcount " << lcount);
	if(bh->size == MAXBLOCK && lcount > 1) {
		DLink *l = (DLink *)((byte *)bh - LARGEHDRSZ);
		lcount--;
		MoveToEmpty(l, bh);
	}
}
