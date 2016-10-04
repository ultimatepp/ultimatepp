#include "Malloc.h"

#ifdef V7

//#define HEAPDBG

#define ATIMING(x)
#define BTIMING(x)

void HeapPanic(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	HexDump(VppLog(), pos, size, 64);
	Panic(text);
}

#ifdef HEAPDBG

void FreeFill(dword *ptr, int count)
{
	//DUMP((void *)ptr);
	//DUMP((void *)(ptr + count));
	while(count--)
		*ptr++ = 0x65657246;
}

void FreeCheck(dword *ptr, int count)
{
	int c = count;
	while(c--)
		if(*ptr++ != 0x65657246)
			HeapPanic("Writes to freed blocks detected", ptr, sizeof(dword) * count);
}

void *MSmall::CheckFree(void *p, int k)
{
	Page *page = (Page *)((uintptr_t)p & ~(uintptr_t)4095);
	ASSERT((byte *)page + sizeof(Page) <= (byte *)p && (byte *)p < (byte *)page + 4096);
	ASSERT((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ASSERT(page->klass == k);
#ifdef CPU_64
	FreeCheck((dword *)p + 2, (Ksz(k) >> 2) - 2);
#else
	FreeCheck((dword *)p + 1, (Ksz(k) >> 2) - 1);
#endif
	return p;
}

void MSmall::FillFree(void *ptr, int k)
{
#ifdef HEAPDBG
#ifdef CPU_64
	FreeFill((dword *)ptr + 2, (Ksz(k) >> 2) - 2);
#else
	FreeFill((dword *)ptr + 1, (Ksz(k) >> 2) - 1);
#endif
#endif
}

#else

inline void FreeFill(dword *ptr, int count)
{}

inline void FreeCheck(dword *ptr, int count)
{}

inline void *MSmall::CheckFree(void *p, int k)
{ return p; }

inline void MSmall::FillFree(void *ptr, int k)
{}

#endif


MSmall::Page *MSmall::global_empty[16];
StaticMutex   MSmall::global_lock;

inline void MSmall::Page::Format(int k)
{
#ifdef HEAPDBG
	FreeFill((dword *)Begin(), (dword *)End() - (dword *)Begin());
#endif
	klass = k;
	active = 0;
	int sz = Ksz(k);
	byte *ptr = End() - sz;
	byte *bg = Begin();
	FreeLink *l = NULL;
	while(ptr > bg) {
		((FreeLink *)ptr)->next = l;
		l = (FreeLink *)ptr;
		ptr -= sz;
	}
	freelist = l;
}

void MSmall::Init()
{
	for(int i = 0; i < 16; i++) {
		int sz = Ksz(i);
		empty[i] = NULL;
		full[i]->LinkSelf();
		work[i]->LinkSelf();
		work[i]->freelist = NULL;
		work[i]->klass = i;
	}
}

inline void *MSmall::AllocK(int k)
{
	Page *page = work[k]->next;
	if(!page) {
		Init();
		page = work[k]->next;
	}
	for(;;) {
		ASSERT(page->klass == k);
		FreeLink *p = page->freelist;
		if(p) {
			CheckFree(p, k);
			page->freelist = p->next;
			++page->active;
			return p;
		}
		if(page->next != page) {
			BTIMING("NextWork");
			page->Unlink();
			page->Link(full[k]);
			page = work[k]->next;
		}
		else {
			BTIMING("EmptyWork");
			page = empty[k];
			empty[k] = NULL;
			if(!page) {
				BTIMING("global get");
				Mutex::Lock __(global_lock); // Put remote free here!!!
				if(global_empty[k]) {
					page = global_empty[k];
					global_empty[k] = page->next;
				}
				else {
					for(int i = 0; i < 16; i++) // try inverse direction
						if(global_empty[i]) {
							page = global_empty[i];
							global_empty[i] = page->next;
							break;
						}
					if(!page)
						for(int i = 0; i < 16; i++) // try inverse direction
							if(empty[i]) {
								page = empty[i];
								empty[i] = NULL;
								break;
							}
					if(!page)
						page = (Page *)AllocRaw4KB();
					page->Format(k);
				}
				ASSERT(page->klass == k);
			}
			page->Link(work[k]);
			ASSERT(page->klass == k);
		}
	}
}

void *MSmall::Alloc(size_t sz)
{
	if(sz <= 256) {
		ATIMING("Small alloc");
//		int k = ((int)sz + (sz == 0) - 1) >> 4;
		if(sz == 0) sz = 1;
		int k = ((int)sz - 1) >> 4;
		if(level[k]) {
			--level[k];
			FreeLink *l = cache[k];
			cache[k] = l->next;
			return CheckFree(l, k);
		}
		sHeapStat(k);
//		DUMP(Ksz(k));
		return CheckFree(AllocK(k), k);
	}
	return LAlloc(sz);
}

void MSmall::Free(void *ptr)
{
	if(!ptr) return;
	if(((dword)(uintptr_t)ptr) & 8)
		LFree(ptr);
	else {
		ATIMING("Small free");
		Page *page = (Page *)((uintptr_t)ptr & ~(uintptr_t)4095);
		int k = page->klass;
		ASSERT((4096 - ((uintptr_t)ptr & (uintptr_t)4095)) % Ksz(k) == 0);
		FillFree(ptr, k);
		if(level[k] < CACHE) {
			level[k]++;
			((FreeLink *)ptr)->next = cache[k];
			cache[k] = (FreeLink *)ptr;
			return;
		}
		if(page->freelist)
			((FreeLink *)ptr)->next = page->freelist;
		else {
			BTIMING("FromFull");
			page->Unlink();
			page->Link(work[k]);
			((FreeLink *)ptr)->next = NULL;
		}
		page->freelist = (FreeLink *)ptr;
		if(--page->active == 0) {
			BTIMING("FreeEmpty");
			page->Unlink();
			if(empty[k]) {
				BTIMING("global free");
				Mutex::Lock __(global_lock);
				empty[k]->next = global_empty[k];
				global_empty[k] = empty[k];
			}
			empty[k] = page;
		}
	}
}

void MSmall::Check() {
	for(int i = 0; i < 16; i++) {
		Page *p = work[i]->next;
		while(p != work[i]) {
			FreeLink *l = p->freelist;
			while(l) {
				CheckFree(l, p->klass & 15);
				l = l->next;
			}
			p = p->next;
		}
/*		p = sFull[i]->next;
		while(p != sFull[i]) {
			ASSERT(p->free <= 4096);
			ASSERT((((MPage *)((uintptr_t)p & ~(uintptr_t)4095))->klass & 15) == i);
			ASSERT(p->count == (4096 - 32) / Ksz(i));
			p = p->next;
		}*/
	}
}

thread__ MSmall heap;

#endif
