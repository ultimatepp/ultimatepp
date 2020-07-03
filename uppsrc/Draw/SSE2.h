#ifdef CPU_X86

#ifdef CPU_SIMD

force_inline
i16x8 LoadRGBA(const RGBA *s)
{
	return Unpack8L(i32x4(*(dword *)s));
}

force_inline
i16x8 LoadRGBA2(const RGBA *s0, const RGBA *s1)
{
	return Unpack8L(i32x4(0, 0, *(dword *)s1, *(dword *)s0));
}

force_inline
i16x8 LoadRGBA2(const RGBA& c)
{
	return LoadRGBA2(&c, &c);
}

force_inline
i16x8 LoadRGBA2(const RGBA *s)
{
	i16x8 h;
	h.Load64(s);
	return Unpack8L(h);
}

force_inline
i16x8 LoadRGBAL(i16x8 x)
{
	return Unpack8L(x);
}

force_inline
i16x8 LoadRGBAH(i16x8 x)
{
	return Unpack8H(x);
}

force_inline
void LoadRGBA4(const RGBA *s, i16x8& l, i16x8& h)
{
	i16x8 t4(s);
	l = LoadRGBAL(t4);
	h = LoadRGBAH(t4);
}

force_inline
i8x16 PackRGBA(i16x8 l, i16x8 h)
{
	return Pack16(l, h);
}

force_inline
void StoreRGBA(RGBA *rgba, i16x8 x)
{
	PackRGBA(x, 0).Store32(rgba);
}

force_inline
void StoreRGBA2(RGBA *rgba, i16x8 x)
{
	PackRGBA(x, 0).Store64(rgba);
}

force_inline
void StoreRGBA4(RGBA *rgba, i16x8 l, i16x8 h)
{
	PackRGBA(l, h).Store(rgba);
}

force_inline
f32x4 LoadRGBAF(const RGBA *s)
{
	return ToFloat(Unpack16L(Unpack8L(i16x8().Load32(s))));
}

force_inline
void StoreRGBAF(RGBA *t, f32x4 s)
{
	Pack16(Pack32(Truncate(s))).Store32(t);
}

force_inline
f32x4 ClampRGBAF(f32x4 p)
{return p;
#ifdef PLATFORM_MACOS
	f32x4 alpha = Broadcast0(p);
#else
	f32x4 alpha = Broadcast3(p);
#endif
	alpha = min(alpha, f32all(255.0));
	return min(p, alpha);
}

#else

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

#endif