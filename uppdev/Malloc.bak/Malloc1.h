#define V1

struct MSmall {
	struct FreeLink {
		FreeLink *next;
	};
	
	enum { HEADERSZ = 48 };

	struct Page {
		MSmall      *heap;
		byte         klass;
		byte         freecount;
		byte         count;
		word         sz;
		byte        *free;
		FreeLink    *freelist;
		Page        *next;
		Page        *prev;
		
		void         LinkSelf()       { prev = next = this; }
		void         Unlink()         { prev->next = next; next->prev = prev; }
		void         Link(Page *lnk)  { prev = lnk; next = lnk->next; next->prev = this; lnk->next = this;  }
	
		void         Format(int k);
		byte *Begin()      { return (byte *)this + HEADERSZ; }
	};
	
	Page full[16][1];
	Page work[16][1];
	Page empty[16][1];
	
	int  emptypages;
	
	void  Init();

	void *AllocK(int k);
	void  FreeK(void *ptr, Page *page, int k);
	void *Alloc(size_t sz);
	void  Free(void *ptr);
};
