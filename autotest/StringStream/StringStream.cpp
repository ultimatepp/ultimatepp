#include <Core/Core.h>

using namespace Upp;

#ifdef flagAUTOTEST
#define N 1000000
#else
#define N 100000000
#endif

String RandomString(int n)
{
	String h;
	while(n-- > 0)
		h.Cat((byte)Random());
	return h;
}

CONSOLE_APP_MAIN
{
	StringStream ss;
	String es;
	for(int i = 0; i < 100000000; i++) {
		if(i % 10000 == 0)
			Cout() << i << "\r\n";
		int p = Random(1000);
		String h = RandomString(p);
		ss.Put(h);
		es.Cat(h);
		
		if(Random(15) == 0) {
			h = ss.GetResult();
			ASSERT(h == es);
		}
		if(Random(15) == 0) {
			ss.Create();
			es.Clear();
		}
//		LOGHEXDUMP(es, es.GetCount());
	}
	
	LOG("========= OK");
}
