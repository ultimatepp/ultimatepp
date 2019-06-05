#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String text = "0123456789";
	for(int i = 0; i < 10; i++) {
		StringBuffer h;
		{
			RTIMING("Cat");
			for(int i = 0; i < 10000000; i++)
				h.Cat(text);
		}
		RTIMING("Shrink");
		h.Shrink();
	}
	return;

	for(int i = 0; i < 10; i++) {
		StringStream h;
		{
			RTIMING("Put");
			for(int i = 0; i < 200000000; i++)
				h.Put('a');
		}
	}
}
