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

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	byte a[497], b[497];
	for(int i = 0; i < 497; i++)
		a[i] = i;
	
	for(int i = 0; i < 497; i++) {
		memcpy(a, b, 497);
		b[i] = 13;

		int len, len2;
		for(int n = 0; n < 497; n++) {
			len = memeq8len(a, b, n);
			len2 = memeq8len_e(a, b, n);
			ASSERT(len == len2);
		}

		for(int n = 0; n < 496; n++) {
			len = memeq8len(a + 1, b + 1, n);
			len2 = memeq8len_e(a + 1, b + 1, n);
			ASSERT(len == len2);
		}
	}
	
	DLOG("============ OK");
}
