#include "Entropy.h"

#ifdef COMPRESS2

struct Recode {
	dword data[2];
	byte  len;
	byte  filler[7];
};

String Compress(const char *data, const char *end)
{
	DUMP(sizeof(Recode));
	ASSERT(sizeof(Recode) == 16);
	
	int stat[256] = { 0 };
	{
		RTIMESTOP("Stat");
		for(const char *s = data; s < end; s++)
			stat[(byte)*s]++;
	}
	

	for(int i = 0; i < 256; i++) {
		LOG(FChar(i) << " " << stat[i]);
	}
	
	LOG("======================");
	
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

		LOG("==== " << m << ":" << (15 - m) * 16 << " ===");		
		DUMP(mini);
		DUMP(big);
		DUMP((mini - big) / 2);
		LOG("--------------------");
	}

	int mini = 0;
	int big = 0;
	for(int i = 0; i < 256; i++) {
		if(i < 12)
			mini += stat[i];
		if(i >= 12 + 48)
			big += stat[i];
		LOG(FChar(ndx[i]) << " " << stat[i]);
	}
	
	DUMP(mini);
	DUMP(big);
	DUMP((mini - big) / 2);
	LOG("--------------------");
	
	Recode recode[256];
	
	Buffer<byte> output(3 * (end - data) / 2 + 10000);
	byte *t = ~output;
	*t++ = 1;
	Poke32le(t, (end - data));
	t += 4;
	for(int i = 0; i < 256; i++) {
		int c = ndx[i];
		LOG(i << ": " << FChar(c));
		Recode& r = recode[c];
		if(i < 12 + 48) {
			*t++ = c;
			if(i < 12) {
				r.len = 1;
				r.data[0] = i;
			}
			else {
				r.len = 2;
				r.data[0] = (i / 16 + 12) | (i << 4);
			}
		}
		else {
			r.len = 3;
			r.data[0] = 0xf | (i << 4);
		}
		r.data[1] = r.data[0] << 4;
	}
	
	NibblePtr p;

	p.Set(t);

	int pos = 0;
	{
		RTIMESTOP("Compress");
		const char *s = data;
		while(s < end - 4) {
			Recode *r;
			byte   *t0;

			r = &recode[(byte)*s++];
			t0 = t + (pos >> 1);
			Poke32le(t0, Peek32le(t0) | r->data[pos & 1]);
			pos += r->len;

			r = &recode[(byte)*s++];
			t0 = t + (pos >> 1);
			Poke32le(t0, Peek32le(t0) | r->data[pos & 1]);
			pos += r->len;

			r = &recode[(byte)*s++];
			t0 = t + (pos >> 1);
			Poke32le(t0, Peek32le(t0) | r->data[pos & 1]);
			pos += r->len;

			r = &recode[(byte)*s++];
			t0 = t + (pos >> 1);
			Poke32le(t0, Peek32le(t0) | r->data[pos & 1]);
			pos += r->len;
		}
		while(s < end) {
			Recode& r = recode[(byte)*s++];
			byte *t0 = t + (pos >> 1);
			Poke32le(t0, Peek32le(t0) | r.data[pos & 1]);
			pos += r.len;
		}
	}
	return String(~output, (pos + 1) / 2);
}

#endif
