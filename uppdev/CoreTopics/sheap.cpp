#include <Core/Core.h>
//#BLITZ_APPROVE

NAMESPACE_UPP

#ifdef UPP_HEAP

#include "HeapImp.h"

#define LLOG(x) //  LOG((void *)this << ' ' << x)

inline void Heap::Page::Format(int k)
{
	DbgFreeFill(Begin(), End() - Begin());
	klass = k;
	active = 0;
	int sz = Ksz(k);
	byte *ptr = End() - sz;
	byte *b = Begin();
	FreeLink *l = NULL;
	while(ptr >= b) {
		((FreeLink *)ptr)->next = l;
		l = (FreeLink *)ptr;
		ptr -= sz;
	}
	freelist = l;
}

Heap::Page *Heap::WorkPage(int k)
{
	LLOG("AllocK - next work not available " << k << " empty: " << (void *)empty[k]);
	Page *page = empty[k];
	empty[k] = NULL;
	if(!page) {
		LLOG("AllocK - trying FreeRemote");
		FreeRemote();
		if(work[k]->freelist) {
			LLOG("AllocK - work available after FreeRemote " << k);
			return work[k];
		}
		page = empty[k];
		empty[k] = NULL;
	}
	if(!page)
		for(int i = 0; i < NKLASS; i++)
			if(empty[i]) {
				LLOG("AllocK - free page available for reformatting " << k);
				page = empty[i];
				empty[i] = NULL;
				page->Format(k);
				break;
			}
	if(!page) {
		Mutex::Lock __(mutex);
		aux.FreeRemoteRaw();
		if(aux.work[k]->next != aux.work[k]) {
			page = aux.work[k]->next;
			page->Unlink();
			page->heap = this;
			LLOG("AllocK - adopting aux page " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
		}
		if(!page && aux.empty[k]) {
			page = aux.empty[k];
			aux.empty[k] = page->next;
			LLOG("AllocK - empty aux page available of the same format " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
		}
		if(!page)
			for(int i = 0; i < NKLASS; i++)
				if(aux.empty[i]) {
					page = aux.empty[i];
					aux.empty[i] = page->next;
					page->Format(k);
					LLOG("AllocK - empty aux page available for reformatting " << k << " page: " << (void *)page << ", free " << (void *)page->freelist);
					break;
				}
		if(!page) {
			page = (Page *)AllocRaw4KB();
			LLOG("AllocK - allocated new system page " << (void *)page << " " << k);
			page->Format(k);
		}
		page->heap = this;
	}
	page->Link(work[k]);
	ASSERT(page->klass == k);
	return page;
}

void *Heap::AllocK(int k)
{
	LLOG("AllocK " << k);
	if(!initialized)
		Init();
	Page *page = work[k]->next;
	for(;;) {
		ASSERT(page->klass == k);
		FreeLink *p = page->freelist;
		if(p) {
			LLOG("AllocK allocating from " << (void *)page << " " << (void *)p);
			page->freelist = p->next;
			++page->active;
			return p;
		}
		LLOG("AllocK - page exhausted " << k << " page: " << (void *)page);
		if(page->next != page) {
			LLOG("Moving " << (void *)page << " to full");
			page->Unlink();
			page->Link(full[k]);
			page = work[k]->next;
		}
		if(page->next == page)
			page = WorkPage(k);
	}
}


inline
void *Heap::Allok(int k)
{
	FreeLink *ptr = cache[k];
	if(ptr) {
		cachen[k]++;
		cache[k] = ptr->next;
		return DbgFreeCheckK(ptr, k);
	}
	return DbgFreeCheckK(AllocK(k), k);
}

inline
void *Heap::Alloc(size_t sz)
{
	Stat(sz);
	if(sz <= 224) {
		if(sz == 0) sz = 1;
		return Allok(((int)sz - 1) >> 4);
	}
	if(sz <= 576)
		return Allok(sz <= 368 ? sz <= 288 ? 14 : 15 : sz <= 448 ? 16 : 17);
	return LAlloc(sz);
}

inline
void *Heap::AllocSz(size_t& sz)
{
	Stat(sz);
	if(sz <= 224) {
		if(sz == 0) sz = 1;
		int k = ((int)sz - 1) >> 4;
		sz = (k + 1) << 4;
		return Allok(k);
	}
	if(sz <= 576) {
		int k;
		if(sz <= 368)
			if(sz <= 288)
				sz = 288, k = 14;
			else
				sz = 368, k = 15;
		else
			if(sz <= 448)
				sz = 448, k = 16;
			else
				sz = 576, k = 17;
		return Allok(k);
	}
	return LAlloc(sz);
}

inline
void Heap::FreeK(void *ptr, Page *page, int k)
{
	if(page->freelist) {
		LLOG("Free next in work page " << k);
		((FreeLink *)ptr)->next = page->freelist;
	}
	else {
		LLOG("Free full to work " << k << " heap: " << (void *)page->heap);
		page->Unlink();
		page->Link(work[k]);
		((FreeLink *)ptr)->next = NULL;
	}
	page->freelist = (FreeLink *)ptr;
	if(--page->active == 0) {
		LLOG("Free page is empty" << " " << (void *)page);
		page->Unlink();
		if(this == &aux) {
			LLOG("...is aux");
			page->next = empty[k];
			empty[k] = page;
		}
		else {
			if(empty[k]) {
				LLOG("Global free " << k << " " << (void *)empty[k]);
				Mutex::Lock __(mutex);
				empty[k]->heap = &aux;
				empty[k]->next = aux.empty[k];
				aux.empty[k] = empty[k];
			}
			empty[k] = page;
		}
	}
}

void Heap::Free(void *ptr)
{
	if(!ptr) return;
	LLOG("Free " << ptr);
	if((((dword)(uintptr_t)ptr) & 8) == 0) {
		Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = page->klass;
		LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
		ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(k) == 0);
#ifdef _MULTITHREADED
		if(page->heap != this) {
			page->heap->RemoteFree(ptr);
			return;
		}
#endif
		DbgFreeFillK(ptr, k);
		if(cachen[k]) {
			cachen[k]--;
			FreeLink *l = (FreeLink *)ptr;
			l->next = cache[k];
			cache[k] = l;
			return;
		}
		FreeK(ptr, page, k);
	}
	else
		LFree(ptr);
}

void Heap::FreeDirect(void *ptr)
{
	LLOG("Free Direct " << ptr);
	if((((dword)(uintptr_t)ptr) & 8) == 0) {
		Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = page->klass;
		LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
		ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(k) == 0);
		DbgFreeFillK(ptr, k);
		FreeK(ptr, page, k);
	}
	else
		LFree(ptr);
}

inline
void *Heap::Alloc32()
{
	Stat(32);
	return Allok(1);
}

inline
void Heap::Free32(void *ptr)
{
	Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
	LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(1) == 0);
#ifdef _MULTITHREADED
	if(page->heap != this) {
		page->heap->RemoteFree(ptr);
		return;
	}
#endif
	DbgFreeFillK(ptr, 1);
	if(cachen[1]) {
		cachen[1]--;
		FreeLink *l = (FreeLink *)ptr;
		l->next = cache[1];
		cache[1] = l;
		return;
	}
	FreeK(ptr, page, 1);
}

inline
void *Heap::Alloc48()
{
	Stat(48);
	return Allok(2);
}

inline
void Heap::Free48(void *ptr)
{
	Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
	LLOG("Small free page: " << (void *)page << ", k: " << k << ", ksz: " << Ksz(k));
	ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(2) == 0);
#ifdef _MULTITHREADED
	if(page->heap != this) {
		page->heap->RemoteFree(ptr);
		return;
	}
#endif
	DbgFreeFillK(ptr, 2);
	if(cachen[2]) {
		cachen[2]--;
		FreeLink *l = (FreeLink *)ptr;
		l->next = cache[2];
		cache[2] = l;
		return;
	}
	FreeK(ptr, page, 2);
}

Heap::Page    dummy;
#define DI    { { 0, 0, 0, 0, &dummy, &dummy } }

thread__ Heap heap = {
	{ DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI, DI }
};

#if defined(HEAPDBG)
void *MemoryAlloc_(size_t sz)
{
	return heap.Alloc(sz);
}

void  MemoryFree_(void *ptr)
{
	heap.Free(ptr);
}
#else
void *MemoryAlloc(size_t sz)
{
	return heap.Alloc(sz);
}

void *MemoryAllocSz(size_t& sz)
{
	return heap.AllocSz(sz);
}

void  MemoryFree(void *ptr)
{
	heap.Free(ptr);
}

void *MemoryAlloc32()
{
	return heap.Alloc32();
}

void  MemoryFree32(void *ptr)
{
	heap.Free32(ptr);
}

void *MemoryAlloc48()
{
	return heap.Alloc48();
}

void  MemoryFree48(void *ptr)
{
	heap.Free48(ptr);
}

#endif

#endif

END_UPP_NAMESPACE
