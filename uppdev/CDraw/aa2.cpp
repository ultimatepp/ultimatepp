#include "MDraw.h"

String aa_packed;

INITBLOCK {
	static const byte n_o[] = {
		// 0     1     2     3     4     5
		0x00, 0x80, 0xff, 0xff, 0x80, 0x00,
		0x00, 0xff, 0x10, 0x10, 0xff, 0x00,
		0x80, 0x80, 0x00, 0x00, 0xff, 0x80,
		0xb0, 0x70, 0x00, 0x00, 0x70, 0xb0,
		0xe0, 0x30, 0x00, 0x00, 0x30, 0xe0,
		0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
		0xe0, 0x30, 0x00, 0x00, 0x30, 0xe0,
		0xb0, 0x70, 0x00, 0x00, 0x70, 0xb0,
		0x80, 0x80, 0x00, 0x00, 0xff, 0x80,
		0x00, 0xff, 0x10, 0x10, 0xff, 0x00,
		0x00, 0x80, 0xff, 0xff, 0x80, 0x00,
	};
	StringBuffer pk;
	for(int w = 0; w < 11; w++) {
		int n = 0;
		const byte *ln = n_o + w * 6;
		int last = -1;
		for(int q = 0; q < 6; q++)
			if(ln[q]) {
				int m = 0;
				String data;
				while(q < 6 && ln[q]) {
					data.Cat(ln[q]);
					m++;
					q++;
				}
				while(n > 15) {
					pk.Cat((15 << 3));
					n -= 15;
				}
				if(m > 7) {
					pk.Cat((n << 3) | 7);
					m -= 7;
					while(m > 7) {
						pk.Cat(7);
						m -= 7;
					}
					if(m)
						pk.Cat(m);
				}
				else
					pk.Cat((n << 3) | m);
				last = pk.GetCount() - 1;
				pk.Cat(data); // Not quite correct!!!
				n = 1;
			}
			else
				n++;
		if(last < 0)
			pk.Cat(0x80);
		else
			pk[last] |= 0x80;
	}
	pk.Cat(0);
	aa_packed = pk;
	LOGHEXDUMP(~aa_packed, aa_packed.GetCount());
}

#define AAP_(b) \
	m = (byte *)&t[b]; \
	alpha = s[b] + (s[b] >> 7); \
	m[0] += alpha * (tr - m[0]) >> 8; \
	m[1] += alpha * (tg - m[1]) >> 8; \
	m[2] += alpha * (tb - m[2]) >> 8;


void DrawAAP(PixelBlock& w, int x, int y, Color c)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	const byte *s = aa_packed;
	dword *t = a;
	byte *m;
	int alpha;
	for(;;) {
		dword c = *s++;
		if(c == 0)
			break;
		t += (c >> 3) & 15;
		switch(c & 7) {
		case 7:
			AAP_(6);
		case 6:
			AAP_(5);
		case 5:
			AAP_(4);
		case 4:
			AAP_(3);
		case 3:
			AAP_(2);
		case 2:
			AAP_(1);
		case 1:
			AAP_(0);
		}
		t += c & 7;
		s += c & 7;
		if(c & 0x80) {
			a += d;
			t = a;
		}
	}
}

#define AAPMMX_(b) \
	alpha = _mm_cvtsi32_si64(s[b]); \
	alpha = _mm_unpacklo_pi16(alpha, alpha); \
	alpha = _mm_unpacklo_pi32(alpha, alpha); \
	h = _mm_srli_pi16(alpha, 7); \
	alpha = _mm_adds_pi16(alpha, h); \
	m = _mm_cvtsi32_si64(t[b]); \
	m = _mm_unpacklo_pi8(m, zero); \
	h = _mm_sub_pi16(mc, m); \
	h = _mm_mullo_pi16(alpha, h); \
	h = _mm_srli_pi16(h, 8); \
	m = _mm_add_pi16(m, h); \
	m = _mm_and_si64(m, mask); \
	m = _mm_packs_pu16(m, m); \
	t[b] = _mm_cvtsi64_si32(m); \


void DrawAAPMMX(PixelBlock& w, int x, int y, Color c)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	__m64 zero = _mm_setzero_si64();
	__m64 mc = _mm_unpacklo_pi8(_mm_cvtsi32_si64(c.GetRaw()), zero);
	__m64 mask = _mm_set1_pi16(0xff);
	const byte *s = aa_packed;
	dword *t = a;
	__m64 alpha;
	__m64 h;
	__m64 m;
	for(;;) {
		dword c = *s++;
		if(c == 0)
			break;
		t += (c >> 3) & 15;
		switch(c & 7) {
		case 7:
			AAPMMX_(6);
		case 6:
			AAPMMX_(5);
		case 5:
			AAPMMX_(4);
		case 4:
			AAPMMX_(3);
		case 3:
			AAPMMX_(2);
		case 2:
			AAPMMX_(1);
		case 1:
			AAPMMX_(0);
		}
		t += c & 7;
		s += c & 7;
		if(c & 0x80) {
			a += d;
			t = a;
		}
	}
	_mm_empty();
}
