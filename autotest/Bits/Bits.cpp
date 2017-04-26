#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	{
		Bits h;
		Vector<bool> v;
		
		SeedRandom();
		
		for(int i = 0; i < 100000; i++) {
			int pos = Random(120000);
			bool b = Random() & 1;
			
			v.At(pos, false) = b;
			h.Set(pos, b);
		}
		
		for(int i = 0; i < v.GetCount(); i++)
			ASSERT(h.Get(i) == v[i]);
	}
	
	{
		Bits r;
		dword *h = r.CreateRaw(2);
		*h = 0x4444;
		
		ASSERT(r.Get(0) == 0);
		ASSERT(r.Get(1) == 0);
		ASSERT(r.Get(2) == 1);
		ASSERT(r.Get(3) == 0);
		ASSERT(r.Get(4) == 0);
		ASSERT(r.Get(5) == 0);
		ASSERT(r.Get(6) == 1);
		
		r.Set(31, 1);
		ASSERT(*h == 0x80004444);
		
		int n;
		const dword *x = r.Raw(n);
		
		DUMP(n);
		ASSERT(n >= 2);
		ASSERT(*x == 0x80004444);
		
		r.Set(1000, 1);
		r.Raw(n);
		DUMP(n);
		r.Set(2000, 1);
		r.Raw(n);
		DUMP(n);
		r.Set(2000, 0);
		r.Shrink();
		ASSERT(*r.Raw(n) == 0x80004444);
		DUMP(n);
	}
	
	LOG("--------- OK");
}
