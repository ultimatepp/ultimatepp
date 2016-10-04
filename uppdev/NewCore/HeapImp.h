#ifndef _NewCore_HeapImp_h_
#define _NewCore_HeapImp_h_

struct FreeLink {
	FreeLink *next;
};

struct MPage {
	byte         klass;
	byte         freecount;
	byte         count;
	byte         filler;
	int          free;
	FreeLink    *freelist;
	MPage       *next;
	MPage       *prev;
#ifdef CPU_32
	byte         align32[12];
#endif

	void         LinkSelf()        { prev = next = this; }
	void         Unlink()          { prev->next = next; next->prev = prev; }
	void         Link(MPage *lnk)  { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }

	void         Format(int k);
};

struct MCache {
	FreeLink  *list;
	int32      count;
};

extern
#ifdef flagMT
#ifdef COMPILER_MSC
__declspec(thread)
#else
__thread
#endif
#endif
MCache mcache[10];

void *MAlloc_Get(MCache& m, int k);
void  MFree_Reduce(MCache& m, int k);

#define NKLASS      21
#define CACHEMAX    31
#define CACHERES    (CACHEMAX / 2)

#endif
