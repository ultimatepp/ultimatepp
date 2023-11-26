#include <Core/Core.h>

using namespace Upp;

Buffer<byte> h(1024*1024*32);
const byte *h1 = h;

CONSOLE_APP_MAIN
{
	for(int i = 0; i < 1000; i++) {
		{
			RTIMING("32MB memset");
			memset(h, 0, 1024*1024 * 32);
		}
		{
			RTIMING("32MB memset8");
			memset8(h, 0, 1024*1024 * 32);
		}
		{
			RTIMING("32MB memcpy");
			memcpy(h, h1, 1024*1024 * 32);
		}
		{
			RTIMING("32MB memcpy8");
			memcpy8(h, h, 1024*1024 * 32);
		}
	}
	
	for(int i = 0; i < 100000000; i++) {
		{
			RTIMING("32B memset");
			memset(h, 0,  32);
		}
		{
			RTIMING("32B memset8");
			memset8(h, 0, 32);
		}
		{
			RTIMING("32B memset32");
			memset32(h, 31525874, 32 / 4);
		}
		{
			RTIMING("32B loop 32");
			dword *s = (dword *)~h;
			dword *e = s + 32 / 4;
			while(s < e)
				*s++ = 31515927;
		}
		{
			RTIMING("32B memcpy");
			memcpy(h, h1, 32);
		}
		{
			RTIMING("32B memcpy8");
			memcpy8(h, h, 32);
		}
	}

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
		{
			RTIMING("32KB memcpy");
			memcpy(h, h1, 1024 * 32);
		}
		{
			RTIMING("32KB memcpy8");
			memcpy8(h, h, 1024 * 32);
		}
	}

	for(int i = 0; i < 1000; i++) {
		{
			RTIMING("32MB memset32");
			memset32(h, 31525874, 1024*1024 * 32 / 4);
		}
		{
			RTIMING("32MB loop 32");
			dword *s = (dword *)~h;
			dword *e = s + 1024*1024 * 32 / 4;
			while(s < e)
				*s++ = 31515927;
		}
	}
}
