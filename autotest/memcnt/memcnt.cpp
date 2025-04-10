#include <Core/Core.h>

using namespace Upp;

template <class T>
size_t memcnt_t_e(const T *p, const T& value, size_t sz)
{
	const T *e = p + sz;
	size_t n = 0;
	while(p < e)
		n += (*p++ == value);
	return n;
}

template <class T>
size_t memcnt_t(const T *data, dword value, size_t sz)
{
	if(sizeof(T) == 1)
		return memcnt8(data, value, sz);
	if(sizeof(T) == 2)
		return memcnt16(data, value, sz);
	if(sizeof(T) == 4)
		return memcnt32(data, value, sz);
	return 0;
}

template <class T>
void Test(int mul) {
	const int N = 255533;
	T data[N];
	for(int i = 0; i < N; i++)
		data[i] = Random(256) * mul;
	for(int value = 0; value < 256; value++) {
		T x = mul * value;
		DLOG((int)x << " " << memcnt_t(data, x, N) << " " << memcnt_t_e(data, x, N));
		ASSERT(memcnt_t(data, x, N) == memcnt_t_e(data, x, N));
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

#if 0
	word h[] = { 0, 0, 0, 4, 4, 0, 0, 4,
	             0, 4, 0 };

	int cnt = memcnt16(h, 4, __countof(h));
	DDUMP(cnt);
	cnt = memcnt_t(h, 4, __countof(h));
	DDUMP(cnt);
	cnt = memcnt16(h, 0, __countof(h));
	DDUMP(cnt);

	Test<word>(253);
	return;
#endif

	Test<byte>(1);
	Test<word>(253);
	Test<dword>(70000);
	
	DLOG("========== OK");
}
