#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 10; i++) {
		Vector<int> h;
		{
			RTIMING("Add");
			for(int q = 0; q < 40000000; q++)
				h.Add(q);
		}
		{
			RTIMING("Shrink");
			h.Shrink();
		}
	}
}
