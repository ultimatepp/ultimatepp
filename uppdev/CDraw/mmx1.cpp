#include "MDraw.h"

void memsetdf(dword *a, dword c, int cx)
{
	if(cx == 0)
		return;
	if((dword)a & 4) {
		*a++ = c;
		cx--;
	}
	int r = cx & 7;
	__m64 mc = _mm_cvtsi32_si64(c);
	mc = _mm_unpacklo_pi32(mc, mc);
	cx >>= 3;
	__m64 *ma = (__m64 *)a;
	while(cx-- > 0) {
		ma[0] = mc;
		ma[1] = mc;
		ma[2] = mc;
		ma[3] = mc;
		ma += 4;
	}
	a = (dword *)ma;
	switch(r) {
	case 7:	a[6] = c;
	case 6:	a[5] = c;
	case 5:	a[4] = c;
	case 4:	a[3] = c;
	case 3:	a[2] = c;
	case 2:	a[1] = c;
	case 1:	a[0] = c;
	}
	_m_empty();
}

void  DrawRectF(PixelBlock& w, int x, int y, int cx, int cy, Color c)
{
	Rect r = RectC(x, y, cx, cy);
	r.Intersect(w.GetSize());
	if(r.IsEmpty())
		return;
	dword color = c.GetRaw();
	dword *a = w.PointAdr(r.left, r.top);
	int d = w.LineDelta();
	cy = r.Height();
	cx = r.Width();
	while(cy--) {
		memsetdf(a, color, cx);
		a += d;
	}
}

extern String op;

void DrawMMX1(PixelBlock& w, int x, int y, Color color)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	const byte *s = op;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	int q = 0;
	__m64 zero = _mm_setzero_si64();
	__m64 mc = _mm_unpacklo_pi8(_mm_cvtsi32_si64(color.GetRaw()), zero);
	__m64 mask = _mm_set1_pi16(0xff);
	for(;;) {
		byte c = *s++;
		if(c) {
			__m64 alpha = _mm_cvtsi32_si64(c);
			alpha = _mm_unpacklo_pi16(alpha, alpha);
			alpha = _mm_unpacklo_pi32(alpha, alpha);
			__m64 h = _mm_srli_pi16(alpha, 7);
			alpha = _mm_adds_pi16(alpha, h);
			__m64 m = _mm_cvtsi32_si64(a[q]);
			m = _mm_unpacklo_pi8(m, zero);
			h = _mm_sub_pi16(mc, m);
			h = _mm_mullo_pi16(alpha, h);
			h = _mm_srli_pi16(h, 8);
			m = _mm_add_pi16(m, h);
			m = _mm_and_si64(m, mask);
			m = _mm_packs_pu16(m, m);
			a[q++] = _mm_cvtsi64_si32(m);
		}
		else {
			c = *s++;
			if(c)
				q += c;
			else {
				if(s[0] == 0 && s[1] == 255)
					break;
				a += d;
				q = 0;
			}
		}
	}
	_m_empty();
}
