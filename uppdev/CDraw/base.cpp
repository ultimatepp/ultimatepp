#include "MDraw.h"

void  DrawRectNOP(PixelBlock& w, int x, int y, int cx, int cy, Color c) {}
void  DrawNOP(PixelBlock& w, int x, int y, Color c) {}

void  DrawRectBASE(PixelBlock& w, int x, int y, int cx, int cy, Color c)
{
	RTIMING("DrawRect");
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
		memsetd(a, color, cx);
		a += d;
	}
}


static const byte n_o[] = {
	0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
	0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
	0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
	0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
	0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
	0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
	0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
	0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
	0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
	0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
	0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
};

String of_packed;

void DrawBASE(PixelBlock& w, int x, int y, Color c)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	dword ic = c;
	const byte *s = of_packed;
	dword *t = a;
	for(;;) {
		byte c = *s++;
		if(c == 0)
			break;
		t += (c >> 3) & 15;
		switch(c & 7) {
		case 7:
			t[6] = ic;
		case 6:
			t[5] = ic;
		case 5:
			t[4] = ic;
		case 4:
			t[3] = ic;
		case 3:
			t[2] = ic;
		case 2:
			t[1] = ic;
		case 1:
			t[0] = ic;
		}
		t += c & 7;
		if(c & 0x80) {
			a += d;
			t = a;
		}
	}
}

INITBLOCK {
	StringBuffer pk;
	for(int w = 0; w < 11; w++) {
		int n = 0;
		const byte *ln = n_o + w * 6;
		bool wasnot = true;
		for(int q = 0; q < 6; q++)
			if(ln[q]) {
				int m = 0;
				while(q < 6 && ln[q]) {
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
				n = 1;
				wasnot = false;
			}
			else
				n++;
		if(wasnot)
			pk.Cat(0x80);
		else
			*(pk.End() - 1) |= 0x80;
	}
	pk.Cat(0);
	of_packed = pk;
}

String op;

INITBLOCK {
	static const byte o[] = {
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
	const byte *s = o;
	int cx = 6;
	int cy = 11;
	StringBuffer result;
	for(int i = 0; i < cy; i++) {
		int j = 0;
		while(j < cx) {
			if(*s == 0) {
				int n = 0;
				while(*s == 0 && j < cx) {
					n++;
					j++;
					s++;
				}
				while(n >= 255) {
					result.Cat(0);
					result.Cat(254);
					n -= 254;
				}
				result.Cat(0);
				result.Cat(n);
			}
			else {
				result.Cat(*s++);
				j++;
			}
		}
		result.Cat(0);
		result.Cat(0);
	}
	result.Cat(0);
	result.Cat(255);
	op = result;
}

void AADrawBASE(PixelBlock& w, int x, int y, Color c)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	const byte *s = op;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	int q = 0;
	for(;;) {
		byte c = *s++;
		if(c) {
			byte *m = (byte *)&a[q];
			int alpha = c + (c >> 7);
			m[0] += alpha * (tr - m[0]) >> 8;
			m[1] += alpha * (tg - m[1]) >> 8;
			m[2] += alpha * (tb - m[2]) >> 8;
			q++;
		}
		else {
			c = *s++;
			if(c)
				q += c;
			else {
				if(s[0] == 0 && s[1] == 255)
					return;
				a += d;
				q = 0;
			}
		}
	}
	a += d;
}

void AADrawBASEPF(PixelBlock& w, int x, int y, Color c)
{
	if(!Rect(w.GetSize()).Contains(Rect(x, y, x + 6, y + 11)))
		return;
	const byte *s = op;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	int q = 0;
	dword *h = a;
	for(;;) {
		byte c = *s++;
		if(c) {
			byte *m = (byte *)&a[q];
			int alpha = c + (c >> 7);
			m[0] += alpha * (tr - m[0]) >> 8;
			m[1] += alpha * (tg - m[1]) >> 8;
			m[2] += alpha * (tb - m[2]) >> 8;
			q++;
		}
		else {
			c = *s++;
			if(c)
				q += c;
			else {
				if(s[0] == 0 && s[1] == 255)
					return;
				a += d;
				q = 0;
			}
		}
	}
	a += d;
}
