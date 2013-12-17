#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	FixedVectorMap<String, String> fmap;
	
	ASSERT(fmap.IsEmpty());
	
	for(int i = 0; i < 10; i++)
		fmap.Add(AsString(100 + (i & 1 ? -1 : 1) * i), AsString(i));

	DUMPM(fmap);
	fmap.Finish();
	DUMPM(fmap);

	ASSERT(!fmap.IsEmpty());
	ASSERT(fmap.GetCount() == 10);
	ASSERT(fmap.Find("foo") < 0);
	ASSERT(fmap.FindPtr("foo") == NULL);

	for(int i = 0; i < 10; i++) {
		ASSERT(fmap.Get(AsString(100 + (i & 1 ? -1 : 1) * i)) == AsString(i));
		ASSERT(*fmap.FindPtr(AsString(100 + (i & 1 ? -1 : 1) * i)) == AsString(i));
	}
	
	fmap.Clear();
	
	ASSERT(fmap.IsEmpty());
	
	for(int i = 1; i < 10; i++)
		for(int j = 0; j < i; j++)
			fmap.Add(AsString(j)) = AsString(j);

	fmap.Finish();

	DUMPM(fmap);

	for(int i = 0; i < 10; i++) {
		int q = fmap.Find(AsString(i));
		int n = 0;
		while(q >= 0) {
			n++;
			ASSERT(fmap[q] == AsString(i));
			q = fmap.FindNext(q);
		}
		ASSERT(n == 9 - i);
	}
	
	LOG("========= EVERYTHING OK ==========");
}
