#ifndef _BM_bm_h_
#define _BM_bm_h_

#include <Core/Core.h>

using namespace Upp;

struct BMFinder {
	int         delta1[256];
	Buffer<int> delta2;
	String      pattern;

	void SetPattern(const String& pattern);

	int  FindIn(const char *data, int len) const;
	int  FindIn(const String& data) const     { return FindIn(data, data.GetCount()); }
	
	BMFinder(const String& pattern)           { SetPattern(pattern); }
};

uint8_t* boyer_moore (uint8_t *string, uint32_t stringlen, uint8_t *pat, uint32_t patlen);

#endif
