#include <Core/Core.h>

using namespace Upp;

void *ptr[10000000];

struct Record {
	int    tid;
	int    ptr;
	size_t sz;
};

CONSOLE_APP_MAIN
{
	String name = "c:/xxx/heap.bog";
	
	int64 l = GetFileLength(name);
	Record *r = (Record *)malloc(l);
	FileIn in(name);
	if((l & 15) || in.Get64(r, l) != l) {
		Cout() << "Invalid input";
		exit(1);
	}

	RTIMING("Heap");
	Record *end = r + l / 16;
	while(r < end) {
		if(r->sz == 0xffffffffffffffff)
			MemoryFree(ptr[r->ptr]);
		else
			ptr[r->ptr] = MemoryAlloc(r->sz);
		r++;
	}
}
