#include "Draw.h"

namespace Upp {

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
			Fill(t, h, count);
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
	return String(r);
}

int Premultiply(RGBA *t, const RGBA *s, size_t len)
{
	const RGBA *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e)
				*t++ = Premultiply(*s++);
			return IMAGE_ALPHA;
		}
		*t++ = *s++;
	}
	return IMAGE_OPAQUE;
}

void FillDown(RGBA *t, int linecy, RGBA c, int cy)
{
	for(int n = cy >> 3; n; n--) {
		t[0] = t[linecy] = t[2 * linecy] = t[3 * linecy] =
		t[4 * linecy] = t[5 * linecy] = t[6 * linecy] = t[7 * linecy] = c;
		t += 8 * linecy;
	}
	if(cy & 4) {
		t[0] = t[linecy] = t[2 * linecy] = t[3 * linecy] = c;
		t += 4 * linecy;
	}
	if(cy & 2) {
		t[0] = t[linecy] = c;
		t += 2 * linecy;
	}
	if(cy & 1)
		t[0] = c;
}

int um_table__[256] = {
	0, 65536, 32768, 21845, 16384, 13107, 10922, 9362, 8192, 7281, 6553, 5957, 5461, 5041, 4681, 4369, 4096, 3855, 3640, 3449, 3276, 3120, 2978, 2849, 2730, 2621, 2520, 2427, 2340, 2259, 2184, 2114, 2048, 1985, 1927, 1872, 1820, 1771, 1724, 1680, 1638, 1598, 1560, 1524, 1489, 1456, 1424, 1394, 1365, 1337, 1310, 1285, 1260, 1236, 1213, 1191, 1170, 1149, 1129, 1110, 1092, 1074, 1057, 1040, 1024, 1008, 992, 978, 963, 949, 936, 923, 910, 897, 885, 873, 862, 851, 840, 829, 819, 809, 799, 789, 780, 771, 762, 753, 744, 736, 728, 720, 712, 704, 697, 689, 682, 675, 668, 661, 655, 648, 642, 636, 630, 624, 618, 612, 606, 601, 595, 590, 585, 579, 574, 569, 564, 560, 555, 550, 546, 541, 537, 532, 528, 524, 520, 516, 512, 508, 504, 500, 496, 492, 489, 485, 481, 478, 474, 471, 468, 464, 461, 458, 455, 451, 448, 445, 442, 439, 436, 434, 431, 428, 425, 422, 420, 417, 414, 412, 409, 407, 404, 402, 399, 397, 394, 392, 390, 387, 385, 383, 381, 378, 376, 374, 372, 370, 368, 366, 364, 362, 360, 358, 356, 354, 352, 350, 348, 346, 344, 343, 341, 339, 337, 336, 334, 332, 330, 329, 327, 326, 324, 322, 321, 319, 318, 316, 315, 313, 312, 310, 309, 307, 306, 304, 303, 302, 300, 299, 297, 296, 295, 293, 292, 291, 289, 288, 287, 286, 284, 283, 282, 281, 280, 278, 277, 276, 275, 274, 273, 271, 270, 269, 268, 267, 266, 265, 264, 263, 262, 261, 260, 259, 258, 257
};

int Unmultiply(RGBA *t, const RGBA *s, size_t len)
{
	const RGBA *e = s + len;
	while(s < e) {
		if(s->a != 255) {
			while(s < e) {
				byte a = s->a;
				if(s->a != 0 && s->a != 255) {
					while(s < e)
						*t++ = Unmultiply(*s++);
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

}
