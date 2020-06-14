#ifdef CPU_X86

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
{
	__m128 alpha = _mm_shuffle_ps(p, p, _MM_SHUFFLE(3, 3, 3, 3));
	alpha = _mm_min_ps(alpha, _mm_set1_ps(255.0));
	return _mm_min_ps(p, alpha);
}

#endif