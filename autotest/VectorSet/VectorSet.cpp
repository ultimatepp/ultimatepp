#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Vector<String> h;
	Vector<String> H;
	
	for(int i = 0; i < 10000; i++) {
		if(Random(100) == 0) {
			h.Clear();
			H.Clear();
		}
		int from = Random(h.GetCount() + 10);
		String what = AsString(i);
		int count = Random(20);
		
		h.Set(from, what, count);
		for(int i = 0; i < count; i++)
			H.At(i + from) = what;
			
		if(i < 100) {
			DDUMP(h);
			DDUMP(H);
		}
		
		ASSERT(H == h);
	}
}
