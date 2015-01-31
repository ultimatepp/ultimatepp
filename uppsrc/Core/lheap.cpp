#include "Core.h"

NAMESPACE_UPP

#ifdef UPP_HEAP

#define LLOG(x) //  LOG((void *)this << ' ' << x)

#include "HeapImp.h"

word  Heap::BinSz[LBINS];
byte  Heap::SzBin[MAXBLOCK / 8 + 1]; // Minimal bin for size (request -> bin)
byte  Heap::BlBin[MAXBLOCK / 8 + 1]; // Largest bin less or equal to size (free -> bin)

Heap::DLink Heap::lempty[1];

const char *asString(int i)
{
	static thread__ char h[4][1024];
	static thread__ int ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%d", i);
	return h[ii];
}

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
	int q = BlBin[size >> 3];
	b->Link(freebin[q]);
	LLOG("Linked " << asString(size) << " to freebin " << asString(q));
}

Heap::DLink *Heap::AddChunk(int reqsize)
{
	DLink *ml;
	if(lempty->next != lempty) {
		ml = lempty->next;
		ml->Unlink();
		LLOG("Retrieved empty large " << (void *)ml);
	}
	else {
		ml = (DLink *)AllocRaw64KB(reqsize);
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
void *Heap::DivideBlock(DLink *b, int size)
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
void *Heap::TryLAlloc(int ii, size_t size)
{
	LLOG("TryLAlloc bin: " << asString(ii) << " size: " << asString(size));
	while(ii < LBINS) {
		if(freebin[ii] != freebin[ii]->next) {
			void *ptr = DivideBlock(freebin[ii]->next, (int)size);
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
	LLOG("+++ LAlloc " << size);
	ASSERT(size > 256);
	if(!initialized)
		Init();
	if(size > MAXBLOCK) {
		Mutex::Lock __(mutex);
		BigHdr *h = (BigHdr *)SysAllocRaw(size + BIGHDRSZ, size);
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
	LLOG("Binned size " << asString(size));
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
	DLink *n = AddChunk((int)size);
	if(!n)
		Panic("Out of memory!");
	ptr = DivideBlock(n, (int)size);
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
	LLOG("--- LFree " << asString(bh->size));
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
	LLOG("Freed, joined size " << asString(bh->size) << " lcount " << asString(lcount));
	if(bh->size == MAXBLOCK && lcount > 1) {
		DLink *l = (DLink *)((byte *)bh - LARGEHDRSZ);
		lcount--;
		MoveToEmpty(l, bh);
	}
}

bool   Heap::LTryRealloc(void *ptr, size_t newsize)
{
	DLink  *b = (DLink *)ptr;
	Header *bh = b->GetHeader();
	if(bh->size == 0) {
		Mutex::Lock __(mutex);
		ASSERT(((dword)(uintptr_t)bh & 4095) == BIGHDRSZ - sizeof(Header));
		BigHdr *h = (BigHdr *)((byte *)ptr - BIGHDRSZ);
		return newsize <= h->size;
	}
	if(bh->heap != this) // if another thread's heap, do not bother to be smart
		return newsize <= bh->size;
	if(bh->size >= newsize)
		return true;
	LLOG("--- TryRealloc " << asString(bh->size));
	Header *n = bh->Next();
	if(n->free && newsize <= (size_t)n->size + (size_t)bh->size) {
		DivideBlock(n->GetBlock(), int(newsize - n->size));
		bh->size += n->size + sizeof(Header);
		n->Next()->prev = bh->size;
		return true;
	}
	return false;
}

size_t Heap::LGetBlockSize(void *ptr) {
	DLink  *b = (DLink *)ptr;
	Header *bh = b->GetHeader();
	if(bh->size == 0) {
		BigHdr *h = (BigHdr *)((byte *)ptr - BIGHDRSZ);
		return h->size;
	}
	return bh->size;
}

#endif

END_UPP_NAMESPACE
