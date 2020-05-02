#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
//	SeedRandom();

	SortedIndex<int> si0;
	for(int i = 0; i < 10000; i++) {
		int v = Random(100);
		int q = si0.FindAdd(v);
		ASSERT(si0[q] == v);
	}
	for(int i = 0; i < 100; i++) {
		int q = si0.FindAdd(i);
	}
	ASSERT(si0.GetCount() == 100);
	for(int i = 0; i < 100; i++)
		ASSERT(si0[i] == i);
	LOG("FindAdd passed");
	SortedIndex<int> si;
	int count = 0;
#ifdef CPU_ARM
	for(int i = 0; i < 100000; i++)
#else
	for(int i = 0; i < 1000000; i++)
#endif
	{
		if(i % 1000 == 0)
			LOG(i);
		if(si.GetCount() > 1000 && Random(1000) == 0) {
			si.Clear();
			count = 0;
		}
		int v = Random(100);
		int h0 = si.FindUpperBound(v);
		int q = si.Add(v);
		count++;
		ASSERT(h0 == q);
		ASSERT(si[q] == v);
		for(int pass = 0; pass < 2; pass++) {
			ASSERT(si.GetCount() == count);
			int l = si.FindLowerBound(v);
			int h = si.FindUpperBound(v);
			ASSERT(l == 0 || si[l - 1] < v);
			ASSERT(l == si.GetCount() || si[l] >= v);
			ASSERT(h == si.GetCount() || si[h] > v);
			ASSERT(h == 0 || si[h - 1] <= v);
			ASSERT(si.GetCount() == 0 || si.FindUpperBound(si.Top()) == si.GetCount());
			for(int i = l; i < h; i++)
				ASSERT(si[i] == v);
			int i = si.Find(v);
			ASSERT(i < 0 ? l >= si.GetCount() || si[l] > v : l == i);
			while(i >= 0) {
				ASSERT(si[i] == v);
				i = si.FindNext(i);
			}
			i = si.FindLast(v);
			while(i >= 0) {
				ASSERT(si[i] == v);
				i = si.FindPrev(i);
			}
			if(i >= 0)
				ASSERT(h - 1 == i);
			if(si.GetCount() > 500 && Random(40) == 0) {
				count -= h - l;
				si.RemoveKey(v);
				ASSERT(si.FindUpperBound(v) == si.FindLowerBound(v));
			}
			v = Random(100);
		}
	}
	
	DDUMPC(si);
}
