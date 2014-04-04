#include <Core/Core.h>

using namespace Upp;

// WARNING: 64-bit OS and 8GB RAM required to run this test

String RandomString(int maxlen = 70)
{
	int len = Random(maxlen);
	String h;
	for(int i = 0; i < len; i++)
		h.Cat(Random(26) + 'a');
	return h;
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	MemoryLimitKb(8000000);
	for(int sz = 0; sz < 2; sz++) {
		for(int pass = 0; pass < 2; pass++) {
			LOG("--------------------");
			DUMP(sz);
			DUMP(pass);
			{
				NanoStrings ns;
				Vector<dword> ws;
				
				ns.ZeroTerminated(sz);
			
				SeedRandom();
				int i = 0;
				int time0 = msecs();
				while(msecs(time0) < 20000) {
					if(++i % 10000000 == 0)
						RLOG("Created " << i);
					String s = pass ? "x" : RandomString(Random(4) ? 5 : 50);
					ws.Add(ns.Add(s));
				}
				
				ns.DumpProfile();
				LOG("---- Strings " << MemoryUsedKb() << " KB used -------");
				LOG(MemoryProfile());
				
				SeedRandom();
				for(int i = 0; i < ws.GetCount(); i++) {	
					if(i % 10000000 == 0)
						RLOG("Tested " << i);
					String s = pass ? "x" : RandomString(Random(4) ? 5 : 50);
					if((sz ? String(ns.GetPtr(ws[i])) : ns.Get(ws[i])) != s) {
						DUMP(i);
						DUMP(ns.Get(ws[i]));
						DUMP(s);
						NEVER();
					}
				}
				LOG("Test OK");
			}
			LOG("===== EMPTY " << MemoryUsedKb() << " KB used -------");
			LOG(MemoryProfile());		
		}
	}
	
	LOG("============ Everything OK ===================");
}
