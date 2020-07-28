#ifndef _Painter_AlphaBlend_h_
#define _Painter_AlphaBlend_h_

namespace Upp {

void AlphaBlend(RGBA *t, const RGBA& c, int alpha);
void AlphaBlend(RGBA *t, const RGBA *s, int alpha, int len);
void AlphaBlend(RGBA *t, const RGBA& c, int alpha, int len);

#ifdef CPU_SIMD

force_inline
i16x8 BroadcastAlpha(i16x8 x)
{
#ifdef PLATFORM_MACOS
	return BroadcastLH0(x);
#else
	return BroadcastLH3(x);
#endif
}

force_inline
i16x8 Mul8(i16x8 x, int alpha)
{
	return i16all(alpha) * x >> 8;
}

force_inline
i16x8 MakeAlpha(i16x8 x)
{
	x = BroadcastAlpha(x);
#ifdef PLATFORM_MACOS
	x = i16x8(129, 129, 129, 128, 129, 129, 129, 128) * x >> 7;
#else
	x = i16x8(128, 129, 129, 129, 128, 129, 129, 129) * x >> 7; // a for alpha, 256*a/255 for color
#endif
	return i16all(256) - x; // 256 - a for alpha, 256 - 256*a/255 for color;
}

force_inline
i16x8 AlphaBlendSIMD(i16x8 t, i16x8 s, i16x8 alpha)
{
	return s + (t * alpha >> 8);
}

force_inline
void AlphaBlend1(RGBA *t, i16x8 s, i16x8 alpha)
{
	StoreRGBA(t, AlphaBlendSIMD(LoadRGBA(t), s, alpha));
}

force_inline
void AlphaBlend2(RGBA *t, i16x8 s, i16x8 alpha)
{
	StoreRGBA2(t, AlphaBlendSIMD(LoadRGBA2(t), s, alpha));
}

force_inline
void AlphaBlend4(RGBA *t, i16x8 sl, i16x8 al, i16x8 sh, i16x8 ah)
{
	i16x8 t4(t);
	PackRGBA(
		AlphaBlendSIMD(LoadRGBAL(t4), sl, al),
		AlphaBlendSIMD(LoadRGBAH(t4), sh, ah)
	).Store(t);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c)
{
	i16x8 s = LoadRGBA(&c);
	StoreRGBA(t, AlphaBlendSIMD(LoadRGBA(t), s, MakeAlpha(s)));
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha)
{
	i16x8 s = Mul8(LoadRGBA(&c), alpha);
	StoreRGBA(t, AlphaBlendSIMD(LoadRGBA(t), s, MakeAlpha(s)));
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha, int len)
{
	i16x8 s = Mul8(LoadRGBA2(c), alpha);
	i16x8 a = MakeAlpha(s);
	while(len >= 4) {
		AlphaBlend4(t, s, a, s, a);
		t += 4;
		len -= 4;
	}
	if(len & 2) {
		AlphaBlend2(t, s, a);
		t += 2;
	}
	if(len & 1)
		AlphaBlend1(t, s, a);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA *s, int alpha, int len)
{
	if(alpha == 256) {
		while(len >= 4) {
			i16x8 m(s);
			i16x8 s0 = LoadRGBAL(m);
			i16x8 s1 = LoadRGBAH(m);
			AlphaBlend4(t, s0, MakeAlpha(s0), s1, MakeAlpha(s1));
			t += 4;
			s += 4;
			len -= 4;
		}
		if(len & 2) {
			i16x8 s0 = LoadRGBA2(s);
			AlphaBlend2(t, s0, MakeAlpha(s0));
			t += 2;
			s += 2;
		}
		if(len & 1) {
			i16x8 s0 = LoadRGBA(s);
			AlphaBlend1(t, s0, MakeAlpha(s0));
		}
	}
	else {
		while(len >= 4) {
			i16x8 m(s);
			i16x8 s0 = Mul8(LoadRGBAL(m), alpha);
			i16x8 s1 = Mul8(LoadRGBAH(m), alpha);
			AlphaBlend4(t, s0, MakeAlpha(s0), s1, MakeAlpha(s1));
			t += 4;
			s += 4;
			len -= 4;
		}
		if(len & 2) {
			i16x8 s0 = Mul8(LoadRGBA2(s), alpha);
			AlphaBlend2(t, s0, MakeAlpha(s0));
			t += 2;
			s += 2;
		}
		if(len & 1) {
			i16x8 s0 = Mul8(LoadRGBA(s), alpha);
			AlphaBlend1(t, s0, MakeAlpha(s0));
		}
	}
}

#else

force_inline
void AlphaBlend__(RGBA& t, const RGBA& c)
{
	int alpha = 256 - (c.a + (c.a >> 7));
	t.r = c.r + (alpha * t.r >> 8);
	t.g = c.g + (alpha * t.g >> 8);
	t.b = c.b + (alpha * t.b >> 8);
	t.a = c.a + ((256 - c.a) * t.a >> 8);
}

force_inline
void AlphaBlend__(RGBA& t, const RGBA& c, int cover)
{
	int a = c.a * cover >> 8;
	int alpha = 256 - a - (a >> 7);
	t.r = (c.r * cover >> 8) + (alpha * t.r >> 8);
	t.g = (c.g * cover >> 8) + (alpha * t.g >> 8);
	t.b = (c.b * cover >> 8) + (alpha * t.b >> 8);
	t.a = a + (alpha * t.a >> 8);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c)
{
	AlphaBlend__(*t, c);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha)
{
	AlphaBlend__(*t, c, alpha);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha, int len)
{
	RGBA c1;
	if(alpha != 256)
		c1 = Mul8(c, alpha);
	else
		c1 = c;
	RGBA *e = t + len;
	while(t < e)
		AlphaBlend__(*t++, c1);
}

force_inline
void AlphaBlend(RGBA *t, const RGBA *s, int alpha, int len)
{
	if(alpha == 256) {
		for(int i = 0; i < len; i++) {
			if(s[i].a == 255)
				t[i] = s[i];
			else
				AlphaBlend__(t[i], s[i]);
		}
	}
	else {
		const RGBA *e = t + len;
		while(t < e)
			AlphaBlend__(*t++, *s++, alpha);
	}
}

#endif

};

#endif
