#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	Buffer<byte> h(1024*1024*30);
	
	for(int i = 0; i < 1000000; i++) {
		{
			RTIMING("32KB memset");
			memset(h, 0, 1024 * 32);
		}
		{
			RTIMING("32KB memset8");
			memset8(h, 0, 1024 * 32);
		}
		{
			RTIMING("32KB memset32");
			memset32(h, 31525874, 1024 * 32 / 4);
		}
		{
			RTIMING("32KB loop 32");
			dword *s = (dword *)~h;
			dword *e = s + 1024 * 32 / 4;
			while(s < e)
				*s++ = 31515927;
		}
	}
}
