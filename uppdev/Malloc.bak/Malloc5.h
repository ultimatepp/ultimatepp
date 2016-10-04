#define V5

struct MSmall {
	struct FreeLink {
		FreeLink *next;
	};
	
	struct Page {
		Page        *next;
		Page        *prev;
		byte         klass;
		byte         active;
		MSmall      *heap;
		FreeLink    *freelist;
		
		void         LinkSelf()       { prev = next = this; }
		void         Unlink()         { prev->next = next; next->prev = prev; }
		void         Link(Page *lnk)  { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }
	
		void         Format(int k);
		byte *Begin()      { return (byte *)this + sizeof(Page); }
		byte *End()        { return (byte *)this + 4096; }
	};
	
	static int Ksz(int k)  { return (k + 1) << 4; }


	static Page       *global_empty[16];
	static StaticMutex global_lock;

	Page  full[16][1];
	Page  work[16][1];
	Page *empty[16];
	
	void  Init();

	void *AllocK(int k);
	void  FreeK(void *ptr, Page *page, int k);
	void *Alloc(size_t sz);
	void  Free(void *ptr);

	static void *CheckFree(void *p, int k);
	static void  FillFree(void *ptr, int k);

	void  Check();
};
