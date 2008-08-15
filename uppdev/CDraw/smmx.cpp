#include "MDraw.h"

void memsetds(dword *a, dword c, int cx)
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
		_mm_stream_pi(ma + 0, mc);
		_mm_stream_pi(ma + 1, mc);
		_mm_stream_pi(ma + 2, mc);
		_mm_stream_pi(ma + 3, mc);
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
	_mm_sfence();
	_mm_empty();
}

void  DrawRectS(PixelBlock& w, int x, int y, int cx, int cy, Color c)
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
		memsetds(a, color, cx);
		a += d;
	}
}
