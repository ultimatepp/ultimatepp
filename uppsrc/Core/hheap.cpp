#include <Core/Core.h>

#define LTIMING(x)  // RTIMING(x)
// #define LSTAT

namespace Upp {

#ifdef UPP_HEAP

#include "HeapImp.h"

// this part reserves very large (224MB for 64bit CPUs, 32MB otherwise)
// chunks form the system and then serves as 4KB rounded allocator
// used as manager of huge memory blocks. 4KB and 64KB blocks are allocated from here too
// also able to deal with bigger blocks, those are directly allocated / freed from system

BlkHeader_<4096> HugeHeapDetail::freelist[20][1]; // only single global Huge heap...
Heap::HugePage *Heap::huge_pages;

#ifdef LSTAT
static int hstat[65536];

EXITBLOCK {
	int cnt = 0;
	for(int i = 0; i < 65536; i++) {
		cnt += hstat[i];
		if(hstat[i])
			RLOG(i * 4 << " KB: " << hstat[i] << " / " << cnt);
	}
}
#endif

int sKBLimit = INT_MAX;

void  MemoryLimitKb(int kb)
{
	sKBLimit = kb;
}

static MemoryProfile *sPeak;

MemoryProfile *PeakMemoryProfile()
{
	if(sPeak)
		return sPeak;
	sPeak = (MemoryProfile *)MemoryAllocPermanent(sizeof(MemoryProfile));
	memset((void *)sPeak, 0, sizeof(MemoryProfile));
	return NULL;
}

void *Heap::HugeAlloc(size_t count) // count in 4kb pages
{
	ASSERT(count);

#ifdef LSTAT
	if(count < 65536)
		hstat[count]++;
#endif

	huge_4KB_count += count;

	auto MaxMem = [&] {
		if(huge_4KB_count > huge_4KB_count_max) {
			huge_4KB_count_max = huge_4KB_count;
			if(MemoryUsedKb() > sKBLimit)
				Panic("MemoryLimitKb breached!");
			if(sPeak)
				Make(*sPeak);
		}
	};

	if(!D::freelist[0]->next) { // initialization
		for(int i = 0; i < __countof(D::freelist); i++)
			Dbl_Self(D::freelist[i]);
	}
		
	if(count > HPAGE) { // we are wasting 4KB to store just 4 bytes here, but this is >32MB after all..
		LTIMING("SysAlloc");
		byte *sysblk = (byte *)SysAllocRaw((count + 1) * 4096, 0);
		BlkHeader *h = (BlkHeader *)(sysblk + 4096);
		h->size = 0;
		*((size_t *)sysblk) = count;
		sys_count++;
		sys_size += 4096 * count;
		MaxMem();
		return h;
	}
	
	LTIMING("Huge Alloc");

	word wcount = (word)count;
	
	if(16 * free_4KB > huge_4KB_count) // keep number of free 4KB blocks in check
		FreeSmallEmpty(INT_MAX, int(free_4KB - huge_4KB_count / 32));
	
	for(int pass = 0; pass < 2; pass++) {
		for(int i = Cv(wcount); i < __countof(D::freelist); i++) {
			BlkHeader *l = D::freelist[i];
			BlkHeader *h = l->next;
			while(h != l) {
				word sz = h->GetSize();
				if(sz >= count) {
					void *ptr = MakeAlloc(h, wcount);
					MaxMem();
					return ptr;
				}
				h = h->next;
			}
		}

		if(!FreeSmallEmpty(wcount, INT_MAX)) { // try to coalesce 4KB small free blocks back to huge storage
			void *ptr = SysAllocRaw(HPAGE * 4096, 0);
			HugePage *pg = (HugePage *)MemoryAllocPermanent(sizeof(HugePage));
			pg->page = ptr;
			pg->next = huge_pages;
			huge_pages = pg;
			AddChunk((BlkHeader *)ptr, HPAGE); // failed, add 32MB from the system
			huge_chunks++;
		}
	}
	Panic("Out of memory");
	return NULL;
}

int Heap::HugeFree(void *ptr)
{
	BlkHeader *h = (BlkHeader *)ptr;
	if(h->size == 0) {
		LTIMING("Sys Free");
		byte *sysblk = (byte *)h - 4096;
		size_t count = *((size_t *)sysblk);
		SysFreeRaw(sysblk, (count + 1) * 4096);
		huge_4KB_count -= count;
		sys_count--;
		sys_size -= 4096 * count;
		return 0;
	}
	LTIMING("Huge Free");
	huge_4KB_count -= h->GetSize();
	return BlkHeap::Free(h)->GetSize();
}

bool Heap::HugeTryRealloc(void *ptr, size_t count)
{
	return count <= HPAGE && BlkHeap::TryRealloc(ptr, count, huge_4KB_count);
}

#endif

}