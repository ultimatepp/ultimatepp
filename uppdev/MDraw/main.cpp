#include "MDraw.h"

#include <emmintrin.h>

void DrawR(PixelBlock& w, int x, int y, Color c)
{}

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

void DrawO(PixelBlock& w, int x, int y, Color c)
{
	const byte *s = n_o;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	dword ic = c;
	for(int w = 0; w < 11; w++) {
		for(int q = 0; q < 6; q++) {
			if(*s)
				a[q] = ic;
			s++;
		}
		a += d;
	}
}

String of_packed;

void DrawOF(PixelBlock& w, int x, int y, Color c)
{
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
			t[7] = ic;
		case 6:
			t[6] = ic;
		case 5:
			t[5] = ic;
		case 4:
			t[4] = ic;
		case 3:
			t[3] = ic;
		case 2:
			t[2] = ic;
		case 1:
			t[1] = ic;
		}
		t += c & 7;
		if(c & 0x80) {
			a += d;
			t = a;
		}
	}
}

void PackOF() {
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

int BlendColor1(int a, int c, int cc)
{
	return (byte)((((a + (a >> 7)) * (c - cc)) >> 8) + cc);
}

#define BLEND(i, d) \
{ \
	const int alpha = d + (d >> 7); \
	byte *m = (byte *)&a[i]; \
	m[0] += (alpha * (tr - m[0]) >> 8); \
	m[1] += (alpha * (tg - m[1]) >> 8); \
	m[2] += (alpha * (tb - m[2]) >> 8); \
}


#define SET(i) a[i] = ic

void DrawF(PixelBlock& w, int x, int y, Color c)
{
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	dword ic = c;
	BLEND(1, 0x80); SET(2); SET(3); BLEND(4, 0x80); a += d;
	SET(1); BLEND(2, 0x10); BLEND(3, 0x10); SET(4); a += d;
	BLEND(0, 0x80); BLEND(1, 0x80); BLEND(4, 0x80); BLEND(5, 0x80); a += d;
	BLEND(0, 0xb0); BLEND(1, 0x70); BLEND(4, 0x70); BLEND(5, 0xb0); a += d;
//	BLEND(0, 0xe0); BLEND(1, 0x30); BLEND(4, 0x30); BLEND(5, 0xe0); a += d;
	SET(0); SET(5); a += d;
	SET(0); SET(5); a += d;
	SET(0); SET(5); a += d;
//	BLEND(0, 0xe0); BLEND(1, 0x30); BLEND(4, 0x30); BLEND(5, 0xe0); a += d;
	BLEND(0, 0xb0); BLEND(1, 0x70); BLEND(4, 0x70); BLEND(5, 0xb0); a += d;
	BLEND(0, 0x80); BLEND(1, 0x80); BLEND(4, 0x80); BLEND(5, 0x80); a += d;
	SET(1); BLEND(2, 0x10); BLEND(3, 0x10); SET(4); a += d;
	BLEND(1, 0x80); SET(2); SET(3); BLEND(4, 0x80); a += d;
}

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

void DrawO1(PixelBlock& w, int x, int y, Color c)
{
	const byte *s = o;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	for(int w = 0; w < 11; w++) {
		for(int q = 0; q < 6; q++) {
			int alpha = *s + (*s >> 7);
			dword cc = a[q];
			byte r = (byte)(cc >> 16);
			byte g = (byte)(cc >> 8);
			byte b = (byte)(cc);
			r = (alpha * (tr - r) >> 8) + r;
			g = (alpha * (tg - g) >> 8) + g;
			b = (alpha * (tb - b) >> 8) + b;
			a[q] = (byte(r) << 16) | (byte(g) << 8) | byte(b);
			s++;
		}
		a += d;
	}
}

void DrawO2(PixelBlock& w, int x, int y, Color c)
{
	const byte *s = o;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = c.GetR();
	byte tg = c.GetG();
	byte tb = c.GetB();
	for(int w = 0; w < 11; w++) {
		for(int q = 0; q < 6; q++) {
			int alpha = *s + (*s >> 7);
			s++;
			byte *m = (byte *)&a[q];
			m[0] += (alpha * (tr - m[0]) >> 8);
			m[1] += (alpha * (tg - m[1]) >> 8);
			m[2] += (alpha * (tb - m[2]) >> 8);
		}
		a += d;
	}
}

String op;

String PackOP(const byte *s, int cx, int cy)
{
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
	return result;
}

void DrawOP(PixelBlock& w, int x, int y, Color c)
{
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

void DrawOPMMX(PixelBlock& w, int x, int y, Color color)
{
	const byte *s = op;
	dword *a = w.PointAdr(x, y);
	int d = w.LineDelta();
	byte tr = color.GetR();
	byte tg = color.GetG();
	byte tb = color.GetB();
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

#define N 300

void DoTest(Draw& w, const char *name, void (*od)(PixelBlock&, int, int, Color))
{
	for(int section = 0; section < 2; section++) {
		String ss = section ? "SECTION " : "SETDIB ";
		{
			TimeStop tm;
			PixelBlock b(1024, 768, section);
			for(int i = 0; i < N; i++) {
				if(od) {
					b.DrawRect(0, 0, 512, 768, White);
					b.DrawRect(512, 0, 512, 768, Red);
					for(int x = 0; x < 1024 / 8 - 1; x++)
						for(int y = 0; y < 768 / 16 - 1; y++)
							(*od)(b, 8 * x, 16 * y, x < 512 / 8 ? Black : White);
				}
				b.Put(w, 0, 0);
				Draw::Flush();
			}
			RLOG(ss << name << ": " << tm.Elapsed() / N << " ms");
		}
		{
			TimeStop tm;
			PixelBlock b(1024, 768, section);
			for(int i = 0; i < N; i++) {
				if(od) {
					for(int y = 0; y < 768 / 16 - 1; y++) {
						b.DrawRect(0, 16 * y, 1024, 16, Color(y & 255, y & 255, y & 255));
						for(int x = 0; x < 1024 / 8 - 1; x++)
							(*od)(b, 8 * x, 16 * y, x < 512 / 8 ? Black : White);
					}
				}
				b.Put(w, 0, 0);
				Draw::Flush();
			}
			RLOG(ss << name << " cached: " << tm.Elapsed() / N << " ms");
		}
	}
}

void MDraw::Paint(Draw& w)
{
	Size sz = GetSize();
/*
	{
		TimeStop tm;
		for(int i = 0; i < 1000; i++)
			w.DrawRect(0, 0, 1024, 768, Color(i, i, i));
		Draw::Flush();
		RLOG("Painting using GDI: " << tm);
	}

	{
		TimeStop tm;
		for(int i = 0; i < 1000; i++) {
			PixelBlock b(1024, 768, true);
			b.DrawRect(0, 0, sz.cx, sz.cy, Color(i, i, i));
			b.Put(w, 0, 0);
		}
		Draw::Flush();
		RLOG("Painting using Section: " << tm);
	}
*/
/*	{
		FontInfo fi = w.GetFontInfo(Arial(15));
		String text = String('0', 1024 / fi['0']);
		TimeStop tm;
		for(int i = 0; i < N; i++) {
			w.DrawRect(0, 0, 1024, 768, White);
			for(int y = 0; y < 768; y += fi.GetHeight())
				w.DrawText(0, y, text, Arial(15), Color(i, i, i));
			Draw::Flush();
		}
		RLOG("GDI: " << tm.Elapsed() / N << " ms");
	}
*/
	DoTest(w, "R - Just PUT", NULL);
	DoTest(w, "R - Just rect", DrawR);
	DoTest(w, "01 - no antialiasing", DrawO);
	DoTest(w, "0F - no antialiasing packed", DrawOF);
	DoTest(w, "01 - shifts", DrawO1);
	DoTest(w, "02 - loads", DrawO2);
	DoTest(w, "F - fast", DrawF);
	DoTest(w, "0P - packed", DrawOP);
	DoTest(w, "0P - packed MMX", DrawOPMMX);

	w.DrawRect(0, 0, 100, 100, SBlue);
}

GUI_APP_MAIN
{
	PackOF();
	DUMP(of_packed.GetLength());
	op = PackOP(o, 6, 11);
	LOGHEXDUMP(op, op.GetLength());
	RDUMP(op.GetLength());
	Ctrl::AutoBackPaintAreaSize = 0;
	MDraw t;
	t.SetRect(0, 0, 1024, 768);
	t.Sizeable().Zoomable();
	t.Run();
}
