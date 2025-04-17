#include <Core/Core.h>

using namespace Upp;

size_t memeq8len_e(const void *p, const void *q, size_t count)
{
	const byte *t = (byte *)p;
	const byte *t0 = t;
	const byte *s = (byte *)q;
	const byte *et = t0 + count;
	
	while(t < et) {
		if(*t != *s)
			return t - t0;
		t++;
		s++;
	}
	
	return count;
}

int len, len2;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	byte a[497], b[497];
	for(int i = 0; i < 497; i++)
		a[i] = Random();

	for(int i = 0; i < 10000; i++)
		for(int i = 0; i < 497; i++) {
			memcpy(a, b, 497);
			b[Random(497)] = 13;
	
			{ RTIMING("memeqlen"); len += memeq8len(a, b, 497); }
			{ RTIMING("memeqlen_e"); len2 += memeq8len_e(a, b, 497); }

			{ RTIMING("memeqlen1"); len += memeq8len(a + 1, b + 1, 496); }
			{ RTIMING("memeqlen_e1"); len2 += memeq8len_e(a + 1, b + 1, 496); }
		}
}
