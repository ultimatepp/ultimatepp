#include <CtrlLib/CtrlLib.h>

inline int Saturate1(int x)
{
	byte *p = (byte *)&x;
	int y = -p[1];
	byte *n = (byte *)&y;
	return ~(p[3]) & (x | n[3]) & 0xff;
}

inline int Saturate2(int x)
{
	return ~(x >> 24) & (x | (-(x >> 8) >> 24)) & 0xff;
}

inline int Saturate3(int x)
{
	return minmax(x, 0, 255);
}


#define N 10000

int x[N];

GUI_APP_MAIN
{
	for(int i = 0; i < 64; i++)
		LOG(255 * ((double)i / 63) << "   " << 255 * i / 63 << "   " << (i << 2) + (i >> 4));

	for(int i = 0; i < N; i++)
		x[i] = (rand() & 2047) - 1024;
	for(int i = 0; i < 1000; i++) {
		{
			RTIMING("Saturate1");
			for(int i = 0; i < N; i++)
				x[i] = Saturate1(x[i]);
		}
		{
			RTIMING("Saturate2");
			for(int i = 0; i < N; i++)
				x[i] = Saturate2(x[i]);
		}
		{
			RTIMING("Saturate3");
			for(int i = 0; i < N; i++)
				x[i] = Saturate3(x[i]);
		}
	}
	PromptOK("Finished!");
}
