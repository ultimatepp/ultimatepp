#include <Core/Core.h>
#include <plugin/lzma/lzma.h>

using namespace Upp;

void *ptr[20000000];

struct Record {
	int    tid;
	int    ptr;
	size_t sz;
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	RLOG("Decompressing");
	Record *r;
	int l;
	{
		String s = LZMADecompress(LoadDataFile(
//			"ide.lzma"
			"index3.lzma"
		));
		l = s.GetCount();
		RLOG("decompressed size: " << l << ", records: " << l / 16);
		r = (Record *)malloc(l);
		memcpy(r, ~s, l);
		RDUMP(MemoryProfile());
	}
	RDUMP(MemoryProfile());

	if(l & 15) {
		Cout() << "Invalid input";
		exit(1);
	}

	int smalln = 0;
	int largen = 0;
	int hugen = 0;

	Record *end = r + l / 16;
	for(Record *s = r; s < end; s++)
		if(s->sz != 0xffffffffffffffff) {
			if(s->sz <= 992)
				smalln++;
			else
			if(s->sz < 255 * 256)
				largen++;
			else
				hugen++;
		}

	RDUMP(smalln);
	RDUMP(largen);
	RDUMP(hugen);
	
	{
		RTIMING("Heap");
		while(r < end) {
			if(r->sz == 0xffffffffffffffff)
				MemoryFree(ptr[r->ptr]);
			else
				ptr[r->ptr] = MemoryAlloc(r->sz);
			r++;
		}
	}

	RDUMP(MemoryProfile());
}
