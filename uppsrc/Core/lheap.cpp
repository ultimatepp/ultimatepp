#include "Core.h"
#include "Core.h"

#define LTIMING(x)  // RTIMING(x)

namespace Upp {

#ifdef UPP_HEAP

#define LLOG(x) //  LOG((void *)this << ' ' << x)

#include "HeapImp.h"

void Heap::LInit()
{
	for(int i = 0; i <= __countof(lheap.freelist); i++)
		Dbl_Self(lheap.freelist[i]);
	big->LinkSelf();
}

void *Heap::TryLAlloc(int i0, word wcount)
{
	for(int i = i0; i < __countof(lheap.freelist); i++) {
		LBlkHeader *l = lheap.freelist[i];
		LBlkHeader *h = l->next;
		while(h != l) {
			word sz = h->GetSize();
			if(sz >= wcount) {
				lheap.MakeAlloc(h, wcount);
				h->heap = this;
				return (BlkPrefix *)h + 1;
			}
			h = h->next;
		}
	}
	return NULL;
}

#if 0
int stat[65536];

EXITBLOCK {
	int cnt = 0;
	for(int i = 0; i < 65536; i++) {
		cnt += stat[i];
		if(stat[i])
			RLOG(i * 256 << ": " << stat[i] << " / " << cnt);
	}
}
#endif

void *Heap::LAlloc(size_t& size)
{
	if(!initialized)
		Init();

	if(size > LUNIT * LPAGE - sizeof(BlkPrefix)) { // big block allocation
		LTIMING("Big alloc");
		Mutex::Lock __(mutex);
		size_t count = (size + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;
		DLink *d = (DLink *)HugeAlloc(count);
		d->Link(big);
		d->size = size = (count << 12) - sizeof(DLink) - sizeof(BlkPrefix);
		BlkPrefix *h = (BlkPrefix *)(d + 1);
		h->heap = NULL; // mark this as huge block
		big_size += size;
		big_count++;
		LLOG("Big alloc " << size << ": " << h + 1);
		return h + 1;
	}

	RTIMING("Large Alloc");
	
	word wcount = word((size + sizeof(BlkPrefix) + LUNIT - 1) >> 8);

#if 0
	stat[wcount]++;
#endif

	size = ((int)wcount * LUNIT) - sizeof(BlkPrefix);
	int i0 = lheap.Cv(wcount);

	if(large_remote_list)  // there might be blocks of this heap freed in other threads
		LargeFreeRemote(); // free them first

	void *ptr = TryLAlloc(i0, wcount);
	if(ptr)
		return ptr;

	RTIMING("Large Alloc 2");
	Mutex::Lock __(mutex);
	aux.LargeFreeRemoteRaw();
	if(aux.large->next != aux.large) {
		while(aux.large->next != aux.large) { // adopt all abandoned large blocks
			DLink *ml = aux.large->next;
			ml->Unlink();
			ml->Link(large);
		}
		ptr = TryLAlloc(i0, wcount);
		if(ptr)
			return ptr;
	}

	LTIMING("Large More");
	DLink *ml = (DLink *)HugeAlloc(((LPAGE + 1) * LUNIT) / 4096);
	ml->Link(large);
	LBlkHeader *h = ml->GetFirst();
	lheap.AddChunk(h, LPAGE);
	lheap.MakeAlloc(h, wcount);
	h->heap = this;
	return (BlkPrefix *)h + 1;
}

void Heap::FreeLargePage(DLink *l)
{
	LLOG("Moving empty large " << (void *)l << " to global storage, lcount " << lcount);
	l->Unlink();
	Mutex::Lock __(mutex);
	HugeFree(l);
}

void Heap::LFree(void *ptr)
{
	BlkPrefix *h = (BlkPrefix *)ptr - 1;

	if(h->heap == this) {
		LTIMING("Large Free");
		LBlkHeader *fh = lheap.Free((LBlkHeader *)h);
		if(fh->GetSize() == LPAGE) {
			LTIMING("FreeLargePage");
			fh->UnlinkFree();
			FreeLargePage((DLink *)((byte *)fh - LOFFSET));
		}
		return;
	}

	Mutex::Lock __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big Free");
		DLink *d = (DLink *)h - 1;
		big_size -= h->size;
		big_count--;
		d->Unlink();
		LLOG("Big free " << (void *) ptr << " size " << h->size);
		HugeFree(d);
		return;
	}

	LTIMING("Remote Free");
	// this is remote heap
	FreeLink *f = (FreeLink *)ptr;
	f->next = h->heap->large_remote_list;
	h->heap->large_remote_list = f;
}

bool   Heap::TryRealloc(void *ptr, size_t& newsize)
{
	ASSERT(ptr);

#ifdef _DEBUG
	if(IsSmall(ptr))
		return false;
#endif

	BlkPrefix *h = (BlkPrefix *)ptr - 1;

	if(h->heap == this) {
		if(newsize > LUNIT * LPAGE - sizeof(BlkPrefix))
			return false;
		word wcount = word(((newsize ? newsize : 1) + sizeof(BlkPrefix) + LUNIT - 1) >> 8);
		size_t dummy;
		if(wcount == h->GetSize() || lheap.TryRealloc(h, wcount, dummy)) {
			newsize = ((int)wcount * LUNIT) - sizeof(BlkPrefix);
			return true;
		}
	}
	
	Mutex::Lock __(mutex);
	if(h->heap == NULL) { // this is big block
		LTIMING("Big Free");

		DLink *d = (DLink *)h - 1;
		BlkPrefix *h = (BlkPrefix *)(d + 1);

		size_t count = (newsize + sizeof(DLink) + sizeof(BlkPrefix) + 4095) >> 12;
		
		if(HugeTryRealloc(d, count)) {
			big_size -= h->size;
			d->size = newsize = (count << 12) - sizeof(DLink) - sizeof(BlkPrefix);
			big_size += h->size;
			return true;
		}
	}

	return false;
}

size_t Heap::LGetBlockSize(void *ptr) {
	LBlkHeader *h = (LBlkHeader *)ptr - 1;

	if(h->heap == NULL) { // huge block
		Mutex::Lock __(mutex);
		DLink *h = (DLink *)ptr - 1;
		return h->size;
	}
	
	return h->GetSize();
}

#endif

}
