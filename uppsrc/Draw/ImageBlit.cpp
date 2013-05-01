#include "Draw.h"

NAMESPACE_UPP

void Fill(RGBA *t, RGBA c, int len)
{
	while(len >= 16) {
		t[0] = c; t[1] = c; t[2] = c; t[3] = c;
		t[4] = c; t[5] = c; t[6] = c; t[7] = c;
		t[8] = c; t[9] = c; t[10] = c; t[11] = c;
		t[12] = c; t[13] = c; t[14] = c; t[15] = c;
		t += 16;
		len -= 16;
	}
	switch(len) {
	case 15: t[14] = c;
	case 14: t[13] = c;
	case 13: t[12] = c;
	case 12: t[11] = c;
	case 11: t[10] = c;
	case 10: t[9] = c;
	case 9: t[8] = c;
	case 8: t[7] = c;
	case 7: t[6] = c;
	case 6: t[5] = c;
	case 5: t[4] = c;
	case 4: t[3] = c;
	case 3: t[2] = c;
	case 2: t[1] = c;
	case 1: t[0] = c;
	}
}
/*
void Fill(RGBA *t, const RGBA& src, int n)
{
	while(n--)
		*t++ = src;
}
*/

void Copy(RGBA *t, const RGBA *s, int n)
{
	while(n--)
		*t++ = *s++;
}

const byte *UnpackRLE(RGBA *t, const byte *s, int len)
{
	RGBA *e = t + len;
	while(t < e)
		if(*s & 0x80) {
			if(*s == 0x80)
				break;
			int count = min<int>((int)(*s & 0x3F), (int)(e - t));
			RGBA h;
			if(*s++ & 0x40)
				Zero(h);
			else {
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				s += 3;
			}
			count = min<int>(count, (int)(e - t));
			memsetex(t, &h, sizeof(RGBA), count);
			t += count;
		}
		else {
			if(*s == 0)
				break;
			int count = *s++;
			while(count-- && t < e) {
				RGBA h;
				h.b = s[0];
				h.g = s[1];
				h.r = s[2];
				h.a = 255;
				*t++ = h;
				s += 3;
			}
		}
	while(t < e)
		Zero(*t++);
	return s;
}

String PackRLE(const RGBA *s, int len)
{
	StringBuffer r;
	const RGBA *e = s + len;
	while(s < e) {
		const RGBA *q = s;
		if(s->a == 0) {
			s++;
			while(s < e && s->a == 0 && s - q < 0x3f)
				s++;
			r.Cat((0x80|0x40) + (int)(s - q));
		}
		else
		if(s + 1 < e && s[0] == s[1]) {
			s++;
			while(s + 1 < e && s[0] == s[1] && s - q < 0x3e)
				s++;
			s++;
			r.Cat(0x80 + (int)(s - q));
			r.Cat(q->b);
			r.Cat(q->g);
			r.Cat(q->r);
		}
		else {
			s++;
			while(s + 1 < e && s->a && s[0] != s[1] && s - q < 0x3f)
				s++;
			r.Cat((int)(s - q));
			while(q < s) {
				r.Cat(q->b);
				r.Cat(q->g);
				r.Cat(q->r);
				q++;
			}
		}
	}
	return r;
}

int Premultiply(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e) {
				byte a = s->a;
				if(s->a != 0 && s->a != 255) {
					while(s < e) {
						int alpha = s->a + (s->a >> 7);
						t->r = alpha * (s->r) >> 8;
						t->g = alpha * (s->g) >> 8;
						t->b = alpha * (s->b) >> 8;
						t->a = s->a;
						s++;
						t++;
					}
					return IMAGE_ALPHA;
				}
				t->r = a & s->r;
				t->g = a & s->g;
				t->b = a & s->b;
				t->a = s->a;
				s++;
				t++;
			}
			return IMAGE_MASK;
		}
		*t++ = *s++;
	}
	return IMAGE_OPAQUE;
}

int um_table__[256];

void sInitUmTable__()
{
	ONCELOCK {
		for(int i = 1; i < 256; i++)
			um_table__[i] = 65536 / i;
	}
}

int Unmultiply(RGBA *t, const RGBA *s, int len)
{
	sInitUmTable__();
	const RGBA *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e) {
				byte a = s->a;
				if(s->a != 0 && s->a != 255) {
					while(s < e) {
						int alpha = um_table__[s->a];
						t->r = (alpha * s->r) >> 8;
						t->g = (alpha * s->g) >> 8;
						t->b = (alpha * s->b) >> 8;
						t->a = s->a;
						s++;
						t++;
					}
					return IMAGE_ALPHA;
				}
				t->r = a & s->r;
				t->g = a & s->g;
				t->b = a & s->b;
				t->a = s->a;
				s++;
				t++;
			}
			return IMAGE_MASK;
		}
		*t++ = *s++;
	}
	return IMAGE_OPAQUE;
}

void AlphaBlend(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = 256 - (s->a + (s->a >> 7));
		t->r = s->r + (alpha * t->r >> 8);
		t->g = s->g + (alpha * t->g >> 8);
		t->b = s->b + (alpha * t->b >> 8);
		t->a = s->a + (alpha * t->a >> 8);
		s++;
		t++;
	}
}

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = 256 - (s->a + (s->a >> 7));
		t->r = s->r + (alpha * t->r >> 8);
		t->g = s->g + (alpha * t->g >> 8);
		t->b = s->b + (alpha * t->b >> 8);
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlend(RGBA *t, const RGBA *s, int len, Color color)
{
	const RGBA *e = s + len;
	int r = color.GetR();
	int g = color.GetG();
	int b = color.GetB();
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (r - t->r) >> 8;
		t->g += alpha * (g - t->g) >> 8;
		t->b += alpha * (b - t->b) >> 8;
		t->a = s->a + ((256 - alpha) * t->a >> 8);
		s++;
		t++;
	}
}

void AlphaBlendOpaque(RGBA *t, const RGBA *s, int len, Color color)
{
	const RGBA *e = s + len;
	int r = color.GetR();
	int g = color.GetG();
	int b = color.GetB();
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (r - t->r) >> 8;
		t->g += alpha * (g - t->g) >> 8;
		t->b += alpha * (b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendStraightOpaque(RGBA *t, const RGBA *s, int len)
{
	const RGBA *e = s + len;
	while(s < e) {
		int alpha = s->a + (s->a >> 7);
		t->r += alpha * (s->r - t->r) >> 8;
		t->g += alpha * (s->g - t->g) >> 8;
		t->b += alpha * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

void AlphaBlendStraightOpaque(RGBA *t, const RGBA *s, int len, int alpha)
{
	if(alpha >= 256) {
		AlphaBlendStraightOpaque(t, s, len);
		return;
	}
	const RGBA *e = s + len;
	alpha *= 0x102;
	while(s < e) {
		int a = (s->a * alpha) >> 16;
		t->r += a * (s->r - t->r) >> 8;
		t->g += a * (s->g - t->g) >> 8;
		t->b += a * (s->b - t->b) >> 8;
		t->a = 255;
		s++;
		t++;
	}
}

struct sBlends {
	int16 m;
	byte  a;
};

sBlends *sblends;

void sOnceInitBlends()
{
	ONCELOCK {
		sblends = (sBlends *)MemoryAllocPermanent(256 * 256 * sizeof(sBlends));
		for(int Fa = 0; Fa <= 255; Fa++)
			for(int Ba = 0; Ba <= 255; Ba++) {
				double A = (Fa / 255.0 + Ba / 255.0 - Fa / 255.0 * Ba / 255.0);
				sblends[(Ba << 8) + Fa].a = minmax((int)(255 * A + 0.5), 0, 255);
				sblends[(Ba << 8) + Fa].m = A > 0.001 ? int(256 * (Fa / 255.0) / A + 0.5) : 0;
			}
	}
}

inline void sInitBlends()
{
	if(!sblends)
		sOnceInitBlends();
}

void AlphaBlendStraight(RGBA *b, const RGBA *f, int len)
{
	sInitBlends();
	const RGBA *e = f + len;
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (f->r - b->r) >> 8;
		b->g += m * (f->g - b->g) >> 8;
		b->b += m * (f->b - b->b) >> 8;
		b++;
		f++;
	}
}

void AlphaBlendOverBgStraight(RGBA *b, RGBA bg, int len)
{
	sInitBlends();
	const RGBA *e = b + len;
	while(b < e) {
		sBlends& x = sblends[(bg.a << 8) + b->a];
		int m = x.m;
		b->a = x.a;
		b->r = bg.r + (m * (b->r - bg.r) >> 8);
		b->g = bg.g + (m * (b->g - bg.g) >> 8);
		b->b = bg.b + (m * (b->b - bg.b) >> 8);
		b++;
	}
}

void AlphaBlendStraight(RGBA *b, const RGBA *f, int len, Color color)
{
	sInitBlends();
	const RGBA *e = f + len;
	int cr = color.GetR();
	int cg = color.GetG();
	int cb = color.GetB();
	while(f < e) {
		sBlends& x = sblends[(b->a << 8) + f->a];
		int m = x.m;
		b->a = x.a;
		b->r += m * (cr - b->r) >> 8;
		b->g += m * (cg - b->g) >> 8;
		b->b += m * (cb - b->b) >> 8;
		b++;
		f++;
	}
}

int GetChMaskPos32(dword mask)
{
	if(mask == 0xff000000)
		return 3;
	if(mask == 0xff0000)
		return 2;
	if(mask == 0xff00)
		return 1;
	return 0;
}

Vector<Image> UnpackImlData(const void *ptr, int len)
{
	Vector<Image> img;
	String data = ZDecompress(ptr, len);
	const char *s = data;
	while(s + 6 * 2 + 1 <= data.End()) {
		ImageBuffer ib(Peek16le(s + 1), Peek16le(s + 3));
		ib.SetHotSpot(Point(Peek16le(s + 5), Peek16le(s + 7)));
		ib.Set2ndSpot(Point(Peek16le(s + 9), Peek16le(s + 11)));
		s += 13;
		int len = ib.GetLength();
		RGBA *t = ib;
		const RGBA *e = t + len;
		if(s + 4 * len > data.End())
			break;
		while(t < e) {
			t->a = s[3];
			t->r = s[0];
			t->g = s[1];
			t->b = s[2];
			s += 4;
			t++;
		}
		img.Add() = ib;
	}
	return img;
}

Vector<Image> UnpackImlData(const String& d)
{
	return UnpackImlData(~d, d.GetLength());
}

void TransformComponents(RGBA *t, const RGBA *s, int len,
	const byte r[], const byte g[], const byte b[], const byte a[])
{
	while(--len >= 0) {
		t->r = r[s->r];
		t->g = g[s->g];
		t->b = b[s->b];
		t->a = a[s->a];
		s++;
		t++;
	}
}

void MultiplyComponents(RGBA *t, const RGBA *s, int len, int num, int den)
{
	byte trans[256];
	for(int i = 0; i < 256; i++)
		trans[i] = (byte)minmax((2 * i + 1) * num / (2 * den), 0, 255);
	TransformComponents(t, s, len, trans, trans, trans, trans);
}

END_UPP_NAMESPACE
