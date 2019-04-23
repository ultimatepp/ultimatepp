#include "Core.h"

namespace Upp {

#ifdef UPP_HEAP

#define LLOG(x) //  LOG((void *)this << ' ' << x)

#include "HeapImp.h"

word  Heap::BinSz[LBINS];
byte  Heap::SzBin[MAXBLOCK / 8 + 1]; // Minimal bin for size (request -> bin)
byte  Heap::BlBin[MAXBLOCK / 8 + 1]; // Largest bin less or equal to size (free -> bin)

Heap::DLink Heap::lempty[1];

const char *asString(int i)
{
	static thread_local char h[4][1024];
	static thread_local int ii;
	ii = (ii + 1) & 3;
	sprintf(h[ii], "%d", i);
	return h[ii];
}

void Heap::GlobalLInit()
{
	ONCELOCK {
		int p = 64;
		int bi = 0;
		while(p < MAXBLOCK) {
			BinSz[bi++] = p - 16;
			int add = minmax(8 * p / 100 / 64 * 64, 64, INT_MAX);
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
			while(k >= 0 && i * 8 < BinSz[k]) k--;
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
{ // gets a free chunk, returns pointer to heap block
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
{ // unlink from free and truncate block, move truncated part back to free blocks
	b->Unlink();
	Header *bh = b->GetHeader();
	ASSERT(bh->size >= (dword)size && size > 0);
	bh->free = false;
	int sz2 = bh->size - size - sizeof(Header);
	if(sz2 >= 48) {
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
{ // move large block page (64KB) current heap (aux) to dest
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
			ASSERT((size_t)ptr & 16);
			return ptr;
		}
		ii++;
	}
	return NULL;
}

void *Heap::LAlloc(size_t& size)
{ // allocate large or big block
	LLOG("+++ LAlloc " << size);
	ASSERT(size > 256);
	if(!initialized)
		Init();
	if(size > MAXBLOCK) { // big block allocation
		Mutex::Lock __(mutex);
		BigHdr *h = (BigHdr *)SysAllocRaw(size + BIGHDRSZ, size);
		h->Link(big);
		h->size = size = ((size + BIGHDRSZ + 4095) & ~4095) - BIGHDRSZ;
		Header *b = (Header *)((byte *)h + BIGHDRSZ - sizeof(Header));
		b->size = 0; // header contains large header with size = 0, to detect big during free
		b->free = false;
		LLOG("Big alloc " << size << ": " << (void *)b->GetBlock());
		return b->GetBlock();
	}
	int bini = SizeToBin((int)size); // get the bin
	size = BinSz[bini]; // get the real bin size
	LLOG("Binned size " << asString(size));
	void *ptr = TryLAlloc(bini, size); // try current working blocks first
	if(ptr)
		return ptr;
	if(large_remote_list) { // there might be blocks freed in other threads
		LargeFreeRemote(); // free them
		ptr = TryLAlloc(bini, size); // try again
		if(ptr) return ptr;
	}
	Mutex::Lock __(mutex);
	aux.LargeFreeRemoteRaw();
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
	ASSERT((size_t)ptr & 16);
	return ptr;
}

void Heap::LFree(void *ptr)
{ // free large or big block
	DLink  *b = (DLink *)ptr;
	Header *bh = b->GetHeader();
	if(bh->size == 0) {
		Mutex::Lock __(mutex);
		ASSERT(((dword)(uintptr_t)bh & 4095) == BIGHDRSZ - sizeof(Header));
		BigHdr *h = (BigHdr *)((byte *)ptr - BIGHDRSZ);
		h->Unlink();
		LLOG("Big free " << (void *) ptr << " size " << h->size);
		SysFreeRaw(h, h->size);
		return;
	}
	if(bh->heap != this) {
		LLOG("Remote large, heap " << (void *)bh->heap);
		Mutex::Lock __(mutex); // TODO: Replace with SpinLock
		FreeLink *f = (FreeLink *)ptr;
		f->next = bh->heap->large_remote_list;
		bh->heap->large_remote_list = f;
		return;
	}
	LLOG("--- LFree " << asString(bh->size));
	if(bh->prev) { // there is previous block
		Header *p = bh->Prev();
		if(p->free) { // previous block is free, join
			b = p->GetBlock();
			b->Unlink(); // remove previous block from free list
			p->size += bh->size + sizeof(Header);
			p->Next()->prev = p->size;
			bh = p;
		}
	}
	Header *n = bh->Next();
	if(n->free) { // next block block is free, join
		n->GetBlock()->Unlink(); // remove next block from free list
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

}
