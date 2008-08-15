#include <Core/Core.h>

using namespace Upp;

int PreMultiply(byte a, byte c)
{
	return int(c * ((double)a / 255));
}

int PreMultiply2(byte a, byte c)
{
	int alpha = a + (a >> 7);
	return (alpha * c) >> 8;
}

int UnMultiply(byte a, byte c)
{
	if(a == 0)
		return 0;
	return int(c / ((double)a / 255));
}

int UnMultiply3(byte a, byte c)
{
	if(a == 0)
		return 0;
	return int(((c << 8) - c) / a);
}

int UnMultiply2(byte a, byte c)
{
	int alpha = a ? 65536 / a : 0;
	return int((alpha * c) >> 8);
}

void UnMultiplyA(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = s->a ? 65536 / s->a : 0;
		t->r = (alpha * s->r) >> 8;
		t->g = (alpha * s->g) >> 8;
		t->b = (alpha * s->b) >> 8;
		t->a = s->a;
		s++;
		t++;
	}
}


byte mm_table[65536];

void UnMultiplyC(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		byte *q = mm_table + (s->a << 8);
		t->r = q[s->r];
		t->g = q[s->g];
		t->b = q[s->b];
		t->a = s->a;
		s++;
		t++;
	}
}

CONSOLE_APP_MAIN
{
/*
	for(int a = 0; a <= 255; a += 5)
		for(int c = 0; c <= 255; c += 5) {
			int pc = PreMultiply(a, c);
			LOG("a: " << a << ", c: " << c << ", PM = " << pc << ":" << PreMultiply2(a, c) <<
			    ", ST = " << UnMultiply(a, pc) << ":" << UnMultiply2(a, pc));
		}
*/
	for(int i = 1; i < 256; i++)
		um_table[i] = 65536 / i;
	
	int ii = 0;
	for(int a = 0; a <= 255; a++)
		for(int c = 0; c <= 255; c++)
			mm_table[ii++] = UnMultiply2(a, c);
	
	Buffer<RGBA> rgba(200000), t(200000);
	for(int i = 0; i < 200000; i++)
		rgba[i].a = rand();
	
	{
		RTIMING("A0")
		for(int i = 0; i < 100; i++)
			UnMultiplyA(t, rgba, 200000);
	}
	{
		RTIMING("A")
		for(int i = 0; i < 100; i++)
			UnMultiplyA(t, rgba, 200000);
	}
	{
		RTIMING("B")
		for(int i = 0; i < 100; i++)
			UnMultiplyB(t, rgba, 200000);
	}
	{
		RTIMING("C")
		for(int i = 0; i < 100; i++)
			UnMultiplyC(t, rgba, 200000);
	}
}
