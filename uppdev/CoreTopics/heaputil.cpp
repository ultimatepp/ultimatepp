#include "Core.h"

#ifdef UPP_HEAP

#ifdef PLATFORM_POSIX
#include <sys/mman.h>
#endif

NAMESPACE_UPP

#include "HeapImp.h"

static MemoryProfile *sPeak;

void *MemoryAllocPermanentRaw(size_t size)
{
	if(size >= 256)
		return malloc(size);
	static byte *ptr = NULL;
	static byte *limit = NULL;
	if(ptr + size >= limit) {
		ptr = (byte *)AllocRaw4KB();
		limit = ptr + 4096;
	}
	void *p = ptr;
	ptr += size;
	return p;
}

void *MemoryAllocPermanent(size_t size)
{
	Mutex::Lock __(Heap::mutex);
	return MemoryAllocPermanentRaw(size);
}

MemoryProfile *PeakMemoryProfile()
{
	if(sPeak)
		return sPeak;
	sPeak = (MemoryProfile *)MemoryAllocPermanent(sizeof(MemoryProfile));
	memset(sPeak, 0, sizeof(MemoryProfile));
	return NULL;
}

void DoPeakProfile()
{
	if(sPeak)
		heap.Make(*sPeak);
}

int sKB;

int   MemoryUsedKb() { return sKB; }

void *SysAllocRaw(size_t size)
{
	sKB += int(((size + 4095) & ~4095) >> 10);
#ifdef PLATFORM_WIN32
	void *ptr = VirtualAlloc(NULL, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#else
#ifdef PLATFORM_LINUX
	void *ptr =  mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#else
	void *ptr =  mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
#endif
	if(ptr == MAP_FAILED)
		ptr = NULL;
#endif
	if(!ptr)
		Panic("Out of memory!");
	DoPeakProfile();
	return ptr;
}

void  SysFreeRaw(void *ptr, size_t size)
{
	sKB -= int(((size + 4095) & ~4095) >> 10);
#ifdef PLATFORM_WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	munmap(ptr, size);
#endif
}

int s4kb__;
int s64kb__;

void *AllocRaw4KB()
{
	static int   left;
	static byte *ptr;
	static int   n = 32;
	if(left == 0) {
		left = n >> 5;
		ptr = (byte *)SysAllocRaw(left * 4096);
	}
	n = n + 1;
	if(n > 4096) n = 4096;
	void *p = ptr;
	ptr += 4096;
	left--;
	s4kb__++;
	DoPeakProfile();
	return p;
}

void *AllocRaw64KB()
{
	static int   left;
	static byte *ptr;
	static int   n = 32;
	if(left == 0) {
		left = n >> 5;
		ptr = (byte *)SysAllocRaw(left * 65536);
	}
	n = n + 1;
	if(n > 256) n = 256;
	void *p = ptr;
	ptr += 65536;
	left--;
	s64kb__++;
	DoPeakProfile();
	return p;
}

void HeapPanic(const char *text, void *pos, int size)
{
	RLOG("\n\n" << text << "\n");
	HexDump(VppLog(), pos, size, 64);
	Panic(text);
}

#ifdef HEAPDBG

void Heap::DbgFreeFill(void *p, size_t size)
{
	size_t count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		*ptr++ = 0x65657246;
}

void Heap::DbgFreeCheck(void *p, size_t size)
{
	size_t count = size >> 2;
	dword *ptr = (dword *)p;
	while(count--)
		if(*ptr++ != 0x65657246)
			HeapPanic("Writes to freed blocks detected", p, (int)(uintptr_t)size);
}

void *Heap::DbgFreeCheckK(void *p, int k)
{
	Page *page = (Page *)((uintptr_t)p & ~(uintptr_t)4095);
	ASSERT((byte *)page + sizeof(Page) <= (byte *)p && (byte *)p < (byte *)page + 4096);
	ASSERT((4096 - ((uintptr_t)p & (uintptr_t)4095)) % Ksz(k) == 0);
	ASSERT(page->klass == k);
	DbgFreeCheck((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
	return p;
}

void Heap::DbgFreeFillK(void *p, int k)
{
	DbgFreeFill((FreeLink *)p + 1, Ksz(k) - sizeof(FreeLink));
}

#endif


void Heap::Make(MemoryProfile& f)
{
	Mutex::Lock __(mutex);
	memset(&f, 0, sizeof(MemoryProfile));
	for(int i = 0; i < NKLASS; i++) {
		int qq = Ksz(i) / 4;
		Page *p = work[i]->next;
		while(p != work[i]) {
			f.allocated[qq] += p->active;
			f.fragmented[qq] += p->Count() - p->active;
			p = p->next;
		}
		p = full[i]->next;
		while(p != full[i]) {
			f.allocated[qq] += p->active;
			p = p->next;
		}
	}
	int ii = 0;
	int fi = 0;
	DLink *m = big->next;
	while(m != big) {
		size_t sz = ((BigHdr *)((byte *)m + BIGHDRSZ - sizeof(Header)))->size;
		f.large_count++;
		f.large_total += sz;
		if(ii < 4096)
			f.large_size[ii++] = sz;
		m = m->next;
	}
	m = large->next;
	while(m != large) {
		Header *h = (Header *)((byte *)m + LARGEHDRSZ);
		while(h->size) {
			if(h->free) {
				f.large_free_count++;
				f.large_free_total += h->size;
				if(fi < 4096)
					f.large_free_size[fi++] = h->size;
			}
			else {
				f.large_count++;
				f.large_total += h->size;
				if(ii < 4096)
					f.large_size[ii++] = h->size;
			}
			h = h->Next();
		}
		m = m->next;
	}
}

MemoryProfile::MemoryProfile()
{
	heap.Make(*this);
}

#ifdef flagHEAPSTAT
int stat[65536];
int bigstat;

void Heap::Stat(size_t sz)
{
	if(sz < 65536)
		stat[sz]++;
	else
		bigstat++;
}

EXITBLOCK {
	int sum = 0;
	for(int i = 0; i < 65536; i++)
		sum += stat[i];
	sum += bigstat;
	int total = 0;
	VppLog() << Sprintf("Allocation statistics: (total allocations: %d)\n", sum);
	for(int i = 0; i < 65536; i++)
		if(stat[i]) {
			total += stat[i];
			VppLog() << Sprintf("%5d %8dx %2d%%, total %8dx %2d%%\n",
			                    i, stat[i], 100 * stat[i] / sum, total, 100 * total / sum);
		}
	if(bigstat) {
		total += bigstat;
		VppLog() << Sprintf(">64KB %8dx %2d%%, total %8dx %2d%%\n",
		 	                bigstat, 100 * bigstat / sum, total, 100 * total / sum);
	}
}
#endif

END_UPP_NAMESPACE

#endif
