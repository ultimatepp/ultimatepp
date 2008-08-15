#include <Core/Core.h>

#include <emmintrin.h>

void Set(dword *t, dword value, int count)
{
	while(count--)
		*t++ = value;
}

void Set2(dword *t, dword value, int count)
{
	count >>= 3;
	while(count--) {
		t[0] = value;
		t[1] = value;
		t[2] = value;
		t[3] = value;
		t[4] = value;
		t[5] = value;
		t[6] = value;
		t[7] = value;
		t += 8;
	}
}

void Set3(dword *t, dword value, int count)
{
	count >>= 4;
	while(count--) {
		t[0] = value;
		t[1] = value;
		t[2] = value;
		t[3] = value;
		t[4] = value;
		t[5] = value;
		t[6] = value;
		t[7] = value;
		t[8] = value;
		t[9] = value;
		t[10] = value;
		t[11] = value;
		t[12] = value;
		t[13] = value;
		t[14] = value;
		t[15] = value;
		t += 16;
	}
}

void SetMMX(dword *_t, dword value, int count)
{
	__m64 val = _mm_set_pi32(value, value);
	count >>= 3;
	__m64 *t = (__m64 *)_t;
	while(count--) {
		t[0] = val;
		t[1] = val;
		t[2] = val;
		t[3] = val;
		t += 4;
	}
	_mm_empty();
}

void SetMMX2(dword *_t, dword value, int count)
{
	__m64 val = _mm_set_pi32(value, value);
	count >>= 3;
	__m64 *t = (__m64 *)_t;
	while(count--) {
		_mm_stream_pi(t + 0, val);
		_mm_stream_pi(t + 1, val);
		_mm_stream_pi(t + 2, val);
		_mm_stream_pi(t + 3, val);
		t += 4;
	}
	_mm_sfence();
	_mm_empty();
}

void SetMMX3(dword *_t, dword value, int count)
{
	__m64 val = _mm_set_pi32(value, value);
	count >>= 4;
	__m64 *t = (__m64 *)_t;
	while(count--) {
		t[0] = val;
		t[1] = val;
		t[2] = val;
		t[3] = val;
		t[4] = val;
		t[5] = val;
		t[6] = val;
		t[7] = val;
		t += 8;
	}
	_mm_empty();
}

void SetSSE2(dword *_t, dword value, int count)
{
	__m128i val;
	dword *x = (dword *)&val;
	x[0] = value;
	x[1] = value;
	x[2] = value;
	x[3] = value;
	count >>= 4;
	__m128i *t = (__m128i *)_t;
	__m128i v1 = val;
	while(count--) {
		t[0] = v1;
		t[1] = v1;
		t[2] = v1;
		t[3] = v1;
		t += 4;
	}
}

void SetSSE22(dword *_t, dword value, int count)
{
	__m128i val;
	dword *x = (dword *)&val;
	x[0] = value;
	x[1] = value;
	x[2] = value;
	x[3] = value;
	count >>= 5;
	__m128i *t = (__m128i *)_t;
	__m128i v1 = val;
	while(count--) {
		t[0] = v1;
		t[1] = v1;
		t[2] = v1;
		t[3] = v1;
		t[5] = v1;
		t[6] = v1;
		t[7] = v1;
		t[8] = v1;
		t += 8;
	}
}

dword target[1024*1024];

void Test(const char *name, void (*set)(dword *t, dword value, int count), int kb)
{
	TimeStop tm;
	int N = (kb < 256 ? 1500 : 500) * 1024 / kb;
	int m = kb * 1024;
	for(int i = 0; i < N; i++)
		(*set)(target, 123456, m);
	double ms = (double)tm.Elapsed() / N;
	RLOG("\t" << name << ": " << 1000 / max(ms, 0.001) * 4 * kb / 1024 << " MB/s");
	Cout() << name << "\n";
}

CONSOLE_APP_MAIN
{
//	for(int i = 4; i <= 512; i = 150 * i / 100) {
	int i = 1024;
	{
		RLOG(4 * i << "KB");
		Cout() << 4 * i << "KB \n";
		Test("Set", Set, i);
		Test("Set2", Set2, i);
		Test("Set3", Set3, i);
		Test("SetMMX", SetMMX, i);
		Test("SetMMX2", SetMMX2, i);
		Test("SetMMX3", SetMMX3, i);
		Test("SetSSE2", SetSSE2, i);
		Test("SetSSE22", SetSSE22, i);
		RLOG("-------------");
	}
}
