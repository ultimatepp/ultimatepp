#ifdef CPU_X86

force_inline
__m128i LoadRGBA(const RGBA *s)
{
	return _mm_unpacklo_epi8(_mm_set_epi32(0, 0, 0, *(dword *)s), _mm_setzero_si128());
}

force_inline
__m128i LoadRGBA2(const RGBA& c)
{
	return _mm_unpacklo_epi8(_mm_set_epi32(0, 0, *(dword *)&c, *(dword *)&c), _mm_setzero_si128());
}

force_inline
__m128i LoadRGBA2(const RGBA *s0, const RGBA *s1)
{
	return _mm_unpacklo_epi8(_mm_set_epi32(0, 0, *(dword *)s1, *(dword *)s0), _mm_setzero_si128());
}

force_inline
__m128i LoadRGBA2(const RGBA *s)
{
	return _mm_unpacklo_epi8(_mm_set_epi32(0, 0, *(dword *)(s + 1), *(dword *)s), _mm_setzero_si128());
}

force_inline
__m128i LoadRGBAL(__m128i x)
{
	return _mm_unpacklo_epi8(x, _mm_setzero_si128());
}

force_inline
__m128i LoadRGBAH(__m128i x)
{
	return _mm_unpackhi_epi8(x, _mm_setzero_si128());
}

force_inline
void LoadRGBA4(const RGBA *s, __m128i& l, __m128i& h)
{
	__m128i t4 = _mm_loadu_si128((__m128i *)s);
	l = LoadRGBAL(t4);
	h = LoadRGBAH(t4);
}

force_inline
__m128i PackRGBA(__m128i l, __m128i h)
{
	return _mm_packus_epi16(l, h);
}

force_inline
void StoreRGBA(RGBA *rgba, __m128i x)
{
	_mm_store_ss((float *)rgba, _mm_castsi128_ps(PackRGBA(x, _mm_setzero_si128())));
}

force_inline
void StoreRGBA2(RGBA *rgba, __m128i x)
{
	_mm_storel_pd((double *)rgba, _mm_castsi128_pd(PackRGBA(x, _mm_setzero_si128())));
}

force_inline
void StoreRGBA4(RGBA *rgba, __m128i l, __m128i h)
{
	_mm_storeu_si128((__m128i *)rgba, PackRGBA(l, h));
}

force_inline
__m128 LoadRGBAF(const RGBA *s)
{
	return _mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_unpacklo_epi8(_mm_set_epi32(0, 0, 0, *(dword *)s), _mm_setzero_si128()), _mm_setzero_si128()));
}

force_inline
void StoreRGBAF(RGBA *t, __m128 s)
{
	_mm_store_ss((float *)t, _mm_castsi128_ps(
		_mm_packus_epi16(
			_mm_packs_epi32(_mm_cvttps_epi32(s), _mm_setzero_si128()),
			_mm_setzero_si128()
		)));
}

force_inline
__m128 ClampRGBAF(__m128 p)
{ // TODO: MacOS
	__m128 alpha = _mm_shuffle_ps(p, p, _MM_SHUFFLE(3, 3, 3, 3));
	alpha = _mm_min_ps(alpha, _mm_set1_ps(255.0));
	return _mm_min_ps(p, alpha);
}

#endif