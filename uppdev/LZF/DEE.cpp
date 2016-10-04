#include "Core/Core.h"

using namespace Upp;

#ifndef COMPRESS2

#define LLOG(x)  // LOG(x)
#define LDUMP(x) // DUMP(x)

String FChar(int c)
{
	String s = Format("0x%02x", c) + " ";
	if(c >= ' ' && c < 128)
		return s + String(c, 1);
	return s + ".";
}

struct Recode {
	byte m;
	byte h1, h2, h3;
	byte l1, h1l2, h2l3;
	byte filler;
};

String DEECompress(const char *data, const char *end)
{
	int stat[256] = { 0 };
	{
		RTIMESTOP("Stat");
		for(const char *s = data; s < end; s++)
			stat[(byte)*s]++;
	}
	

	LLOG("======================");
	
	// 0000 - 1011                     most frequent 12
	// 1100 0000 - 1110 1111           average 48
	// 1111 0000 0000 - 1111 1111 1111 rest

	byte ndx[256];
	for(int i = 0; i < 256; i++)
		ndx[i] = i;
	
	IndexSort(stat, stat + 256, ndx, StdGreater<int>());

	for(int m = 1; m < 15; m++) {
		int mini = 0;
		int big = 0;
		for(int i = 0; i < 256; i++) {
			if(i < m)
				mini += stat[i];
			if(i >= (15 - m) * 16)
				big += stat[i];
//			LOG(FChar(ndx[i]) << " " << stat[i]);
		}

		LLOG("==== " << m << ":" << (15 - m) * 16 << " ===");		
		LDUMP(mini);
		LDUMP(big);
		LDUMP((mini - big) / 2);
		LLOG("--------------------");
	}

	int mini = 0;
	int big = 0;
	for(int i = 0; i < 256; i++) {
		if(i < 12)
			mini += stat[i];
		if(i >= 12 + 48)
			big += stat[i];
		LLOG(FChar(ndx[i]) << " " << stat[i]);
	}
	
	LDUMP(mini);
	LDUMP(big);
	LDUMP((mini - big) / 2);
	LLOG("--------------------");
	
	Recode recode[256];
	
	Buffer<byte> output(3 * (end - data) / 2 + 10000);
	byte *t = ~output;
	*t++ = 1;
	Poke32le(t, (end - data));
	t += 4;
	for(int i = 0; i < 256; i++) {
		int c = ndx[i];
		LLOG(i << ": " << FChar(c));
		Recode& r = recode[c];
		if(i < 12 + 48) {
			*t++ = c;
			if(i < 12) {
				r.m = 1;
				r.h1 = i << 4;
				r.h2 = 0;
				r.h3 = 0;
				r.l1 = i;
//				r.l2 = 0;
//				r.l3 = 0;
			}
			else {
				int q = i + 0xc0;
				r.m = 2;
				r.h1 = q & 0xf0;
				r.h2 = (q << 4) & 0xf0;
				r.h3 = 0;
				r.l1 = (q >> 4) & 0x0f;
				r.h1l2 = r.h1 | (q & 0x0f);
//				r.l3 = 0;
			}
		}
		else {
			r.m = 3;
			r.h1 = 0xf0;
			r.h2 = 0xf0 & i;
			r.h3 = 0xf0 & (i << 4);
			r.l1 = 0x0f;
//			r.l2 = 0x0f & (i >> 4);
			r.h2l3 = r.h2 | (0x0f & i);
		}
	}
	
	{
		RTIMESTOP("Compress");
		const char *s = data;
		byte g = 0;
		for(;;) {
		nibble0:
			{
				if(s >= end) break;
				Recode& r = recode[(byte)*s++];
				if(r.m == 1) {
					g = r.h1;
					goto nibble1;
				}
				if(r.m == 2) {
					*t++ = r.h1l2;
					goto nibble0;
				}
				*t++ = r.h1l2;
				g = r.h3;
				goto nibble1;
			}
		nibble1:
			{
				if(s >= end) break;
				Recode& r = recode[(byte)*s++];
				if(r.m == 1) {
					*t++ = g | r.l1;
					goto nibble0;
				}
				if(r.m == 2) {
					*t++ = g | r.l1;
					g = r.h2;
					goto nibble1;
				}
				*t++ = g | r.l1;
				*t++ = r.h2l3;
				goto nibble0;
			}
		}
	}
	return String(~output, t - ~output + 1);
}

#endif
