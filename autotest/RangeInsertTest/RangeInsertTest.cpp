#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	SeedRandom();

	Vector<int> v;
	Array<int> a;
	InVector<int> iv;
	InArray<int> av;

#ifdef CPU_ARM
	for(int i = 0; i < 500; i++)
#else
	for(int i = 0; i < 10000; i++)
#endif
	{
		if(v.GetCount() > 100000) {
			int pos = Random(v.GetCount() - 1);
			int count = Random(v.GetCount() - pos);
			LOG(i << ": removing " << pos << ", " << count);
			v.Remove(pos, count);
			a.Remove(pos, count);
			iv.Remove(pos, count);
			av.Remove(pos, count);
		}
		else {
			Vector<int> h;
			int count = Random(10000);
			while(count--)
				h << Random();
			int pos = Random(v.GetCount() + 1);
			LOG(i << ": inserting " << pos << ", " << h.GetCount());
			v.InsertRange(pos, h);
			a.InsertRange(pos, h);
			iv.InsertRange(pos, h);
			av.InsertRange(pos, h);
		}
		ASSERT(v == a);
		ASSERT(v == iv);
		ASSERT(v == av);
		ASSERT(a == iv);
		ASSERT(av == iv);
	}
	
	LOG("========== OK");
}
