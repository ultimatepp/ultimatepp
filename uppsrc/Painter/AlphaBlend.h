#ifndef _Painter_AlphaBlend_h_
#define _Painter_AlphaBlend_h_

namespace Upp {

void AlphaBlend(RGBA *t, const RGBA& c, int alpha);
void AlphaBlend(RGBA *t, const RGBA *s, int alpha, int len);
void AlphaBlend(RGBA *t, const RGBA& c, int alpha, int len);

#if defined(CPU_SSE2) && !defined(flagNOSIMD)

force_inline
__m128i BroadcastAlpha(__m128i x)
{
#ifdef PLATFORM_MACOS
	return _mm_shufflelo_epi16(_mm_shufflehi_epi16(x, 0x00), 0x00);
#else
	return _mm_shufflelo_epi16(_mm_shufflehi_epi16(x, 0xff), 0xff);
#endif
}

force_inline
__m128i Mul8(__m128i x, int alpha)
{
	return _mm_srli_epi16(_mm_mullo_epi16(_mm_set1_epi16(alpha), x), 8); // c.a * alpha >> 8;
}

force_inline
__m128i MakeAlpha(__m128i x)
{
	x = BroadcastAlpha(x);
#ifdef PLATFORM_MACOS
	x = _mm_srli_epi16(_mm_mullo_epi16(_mm_set_epi16(129, 129, 129, 128, 129, 129, 129, 128), x), 7); // a for alpha, 256*a/255 for color
#else
	x = _mm_srli_epi16(_mm_mullo_epi16(_mm_set_epi16(128, 129, 129, 129, 128, 129, 129, 129), x), 7); // a for alpha, 256*a/255 for color
#endif
	x = _mm_sub_epi16(_mm_set1_epi16(256), x); // 256 - a for alpha, 256 - 256*a/255 for color;
	return x;
}

force_inline
__m128i AlphaBlendSSE2(__m128i t, __m128i s, __m128i alpha)
{
	return _mm_adds_epi16(s, _mm_srli_epi16(_mm_mullo_epi16(t, alpha), 8)); // t = c + (t * alpha >> 8);
}

force_inline
void AlphaBlend1(RGBA *t, __m128i s, __m128i alpha)
{
	StoreRGBA(t, AlphaBlendSSE2(LoadRGBA(t), s, alpha));
}

force_inline
void AlphaBlend2(RGBA *t, __m128i s, __m128i alpha)
{
	StoreRGBA2(t, AlphaBlendSSE2(LoadRGBA2(t), s, alpha));
}

force_inline
void AlphaBlend4(RGBA *t, __m128i sl, __m128i al, __m128i sh, __m128i ah)
{
	__m128i t4 = _mm_loadu_si128((__m128i *)t);
	_mm_storeu_si128((__m128i *)t,
		PackRGBA(
			AlphaBlendSSE2(LoadRGBAL(t4), sl, al),
			AlphaBlendSSE2(LoadRGBAH(t4), sh, ah)));
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c)
{
	__m128i s = LoadRGBA(&c);
	StoreRGBA(t, AlphaBlendSSE2(LoadRGBA(t), s, MakeAlpha(s)));
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha)
{
	__m128i s = Mul8(LoadRGBA(&c), alpha);
	StoreRGBA(t, AlphaBlendSSE2(LoadRGBA(t), s, MakeAlpha(s)));
}

force_inline
void AlphaBlend(RGBA *t, const RGBA& c, int alpha, int len)
{
	__m128i s = Mul8(LoadRGBA2(c), alpha);
	__m128i a = MakeAlpha(s);
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
			__m128i m = _mm_loadu_si128((__m128i *)s);
			__m128i s0 = LoadRGBAL(m);
			__m128i s1 = LoadRGBAH(m);
			AlphaBlend4(t, s0, MakeAlpha(s0), s1, MakeAlpha(s1));
			t += 4;
			s += 4;
			len -= 4;
		}
		if(len & 2) {
			__m128i s0 = LoadRGBA2(s);
			AlphaBlend2(t, s0, MakeAlpha(s0));
			t += 2;
			s += 2;
		}
		if(len & 1) {
			__m128i s0 = LoadRGBA(s);
			AlphaBlend1(t, s0, MakeAlpha(s0));
		}
	}
	else {
		while(len >= 4) {
			__m128i m = _mm_loadu_si128((__m128i *)s);
			__m128i s0 = Mul8(LoadRGBAL(m), alpha);
			__m128i s1 = Mul8(LoadRGBAH(m), alpha);
			AlphaBlend4(t, s0, MakeAlpha(s0), s1, MakeAlpha(s1));
			t += 4;
			s += 4;
			len -= 4;
		}
		if(len & 2) {
			__m128i s0 = Mul8(LoadRGBA2(s), alpha);
			AlphaBlend2(t, s0, MakeAlpha(s0));
			t += 2;
			s += 2;
		}
		if(len & 1) {
			__m128i s0 = Mul8(LoadRGBA(s), alpha);
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
