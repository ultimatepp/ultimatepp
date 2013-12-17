#include <Core/Core.h>

using namespace Upp;

#include "FixedMap.h"

CONSOLE_APP_MAIN
{
#ifdef _DEBUG
	FixedVectorMap<String, String> fmap;
	for(int i = 0; i < 10; i++)
		fmap.Add(AsString(100 + (i & 1 ? -1 : 1) * i), AsString(i));

	DUMPM(fmap);
	fmap.Finish();
	DUMPM(fmap);

	for(int i = 0; i < 10; i++) {
		ASSERT(fmap.Get(AsString(100 + (i & 1 ? -1 : 1) * i)) == AsString(i));
	}
	
//	return;
#endif
	
	int used = MemoryUsedKb();
	{
		Vector<int> index;
		Vector<String> value;
		VectorMap<int, String> map;
		ArrayMap<int, String> amap;
		SortedVectorMap<int, String> smap;
		FixedVectorMap<int, String> fmap;
		InVector<int> iv;
		{
			RTIMING("time");
			for(int i = 0; i < 100000; i++)
		//		{ index.Add(i); value.Add(AsString(i)); }
				amap.Add(Random(), AsString(i));
		//		iv.Add(i);
			fmap.Finish();
		}
		smap.GetKeys().DumpIndex();
		smap.GetValues().DumpIndex();
		RLOG("---- " << MemoryUsedKb() - used << " KB used -------");
		RLOG(MemoryProfile());

		MemoryProfile m;
		RDUMP(m.big_size);
#if 0		
		for(int i = 0; i < 1024 && m.large_free_size[i]; i++)
			RDUMP(m.large_free_size[i]);
#endif
	}
	RLOG("---- " << MemoryUsedKb() - used << " KB used -------");
	RLOG(MemoryProfile());
	RLOG("======== SHRINK");
	MemoryShrink();
	RLOG("---- " << MemoryUsedKb() - used << " KB used -------");
	RLOG(MemoryProfile());
}
