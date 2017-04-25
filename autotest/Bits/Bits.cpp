#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	Bits h;
	Vector<bool> v;
	
	for(int i = 0; i < 100000; i++) {
		int pos = Random(120000);
		bool b = Random() & 1;
		
		v.At(pos, false) = b;
		h.Set(pos, b);
	}
	
	for(int i = 0; i < v.GetCount(); i++)
		ASSERT(h.Get(i) == v[i]);
	
	LOG("--------- OK");
}
