#include <Core/Core.h>

using namespace Upp;

int MemCmp_aligned__(const char *a, const char *b, size_t len)
{
	const dword *x = (dword *)a;
	const dword *y = (dword *)b;
	const dword *e = x + (len >> 2);
	while(x < e) {
		if(*x != *y)
			return SwapEndian32(*x) < SwapEndian32(*y) ? -1 : 1;
		x++;
		y++;
	}
	len &= 3;
	if(len == 0) return 0;
	len <<= 3;
	dword mask = 0xffffffff << (len << 3);
	return (SwapEndian32(*x) >> mask) < (SwapEndian32(*y & mask) >> mask) ? -1 : 1;
}
