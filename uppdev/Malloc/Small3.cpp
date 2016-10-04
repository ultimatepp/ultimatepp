#include "Malloc.h"

#ifdef V3

#define ATIMING(x)

#define sHeapStat(k)
#define FreeFill(p, c)
#define CheckFree(p, k) p

inline int Ksz(int k)   { return (k + 1) << 4; }

static byte sKount[16];
static word sFreem[16];

inline void MSmall::Page::Format(int k)
{
	LOG("Formatting " << (void *)this << " to " << k);
	klass = k;
	sz = Ksz(k);
	count = sKount[k];
	ASSERT(count == (4096 - HEADERSZ) / sz);
//	freecount = count;
	active = 0;
	byte *ptr = Begin();
	byte *end = (byte *)this + 4096;
	FreeLink *l = NULL;
	while(ptr < end) {
		((FreeLink *)ptr)->next = l;
		l = (FreeLink *)ptr;
		ptr += sz;
	}
	freelist = l;
#ifdef HEAPDBG
	FreeFill((dword *)Begin(), (4096 - HEADERSZ) / 4);
#endif
}

void MSmall::Init()
{
	for(int i = 0; i < 16; i++) {
		int sz = Ksz(i);
		sKount[i] = (4096 - HEADERSZ) / sz;
		sFreem[i] = sKount[i] * sz - sz;
	}
	for(int i = 0; i < 16; i++) {
		empty[i]->LinkSelf();
		full[i]->LinkSelf();
		work[i]->LinkSelf();
		work[i]->freelist = NULL;
		work[i]->klass = i;
	}
	emptypages = 0;
}

inline void *MSmall::AllocK(int k)
{
	LOG("AllocK " << (int)k);
	Page *page = work[k]->next;
	for(;;) {
		ASSERT(page->klass == k);
		FreeLink *p = page->freelist;
		if(p) {
			page->freelist = p->next;
			++page->active;
			DUMP((void *)page);
			DUMP((int)page->active);
			return p;
		}
		if(page->next != page) {
			ATIMING("NextWork");
			page->Unlink();
			page->Link(full[k]);
			page = work[k]->next;
		}
		else {
			page = empty[k]->next;
			if(page->next == page) {
				if(emptypages) {
					ATIMING("FromEmpty other");
					emptypages--;
					for(int i = 0; i < 16; i++) { // try inverse direction
						page = empty[i]->next;
						if(page->next != page) {
							page->Unlink();
							break;
						}
					}
					ASSERT(page->next != page);
				}
				else // remote delete should be here!
					page = (Page *)AllocRaw4KB();
				page->Format(k);
				ASSERT(page->klass == k);
			}
			else {
				ATIMING("FromEmpty");
				ASSERT(emptypages > 0);
				page->Unlink();
				emptypages--;
				ASSERT(page->klass == k);
			}
			page->Link(work[k]);
			ASSERT(page->klass == k);
		}
	}
}

inline void MSmall::FreeK(void *ptr, Page *page, int k)
{
	if(page->freelist)
		((FreeLink *)ptr)->next = page->freelist;
	else {
		page->Unlink();
		page->Link(work[k]);
	}
	page->freelist = (FreeLink *)ptr;
	if(--page->active == 0) {
		ATIMING("ToEmpty");
		page->Unlink();
		page->Link(empty[k]);
		emptypages++;
		return;
	}
}

void *MSmall::Alloc(size_t sz)
{
	if(sz <= 256) {
		ATIMING("Small alloc");
		int k = ((int)sz + (sz == 0) - 1) >> 4;
		sHeapStat(k);
		return CheckFree(AllocK(k), k);
	}
	NEVER();
	return NULL;
//	return LAlloc(sz);
}

void MSmall::Free(void *ptr)
{
	if(!ptr) return;
	if(((dword)(uintptr_t)ptr) & 8)
		NEVER();
//		LFree(ptr);
	else {
		ATIMING("Small free");
		Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = page->klass;
		ASSERT((((uintptr_t)ptr & (uintptr_t)4095) - HEADERSZ) % Ksz(k & 15) == 0);
#ifdef HEAPDBG
#ifdef CPU_64
		FreeFill((dword *)ptr + 2, (Ksz(k & 15) >> 2) - 2);
#else
		FreeFill((dword *)ptr + 1, (Ksz(k & 15) >> 2) - 1);
#endif
#endif
		FreeK(ptr, page, k);
	}
}

#endif
