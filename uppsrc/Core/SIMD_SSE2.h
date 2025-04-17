struct f32x4 {
	__m128 data;

	f32x4& Load(const void *ptr)   { data = _mm_loadu_ps((float *)ptr); return *this; }
	f32x4& Load64(const void *ptr) { data = _mm_castpd_ps(_mm_load_sd((double *)ptr)); return *this; }
	f32x4& Load32(const void *ptr) { data = _mm_load_ss((float *)ptr); return *this; }

	void   Store(void *ptr)        { _mm_storeu_ps((float *)ptr, data); }
	void   Store64(void *ptr)      { _mm_store_sd((double *)ptr, _mm_castps_pd(data)); }
	void   Store32(void *ptr)      { _mm_store_ss((float *)ptr, data); }
	
	f32x4()                      {}
	f32x4(const void *ptr)       { Load(ptr); }
	f32x4(__m128 d)              { data = d; }
	f32x4(double f)              { data = _mm_set_ss((float)f); }
	f32x4(float f)               { data = _mm_set_ss(f); }
	f32x4(int f)                 { data = _mm_set_ss((float)f); }
	f32x4(double a, double b, double c, double d) { data = _mm_set_ps((float)a, (float)b, (float)c, (float)d); }
	
	operator __m128()            { return data; }
};

force_inline f32x4  f32all(double f)              { return _mm_set1_ps((float)f); }

force_inline f32x4  operator+(f32x4 a, f32x4 b)   { return _mm_add_ps(a.data, b.data); }
force_inline f32x4& operator+=(f32x4& a, f32x4 b) { return a = a + b; }
force_inline f32x4  operator-(f32x4 a, f32x4 b)   { return _mm_sub_ps(a.data, b.data); }
force_inline f32x4& operator-=(f32x4& a, f32x4 b) { return a = a - b; }
force_inline f32x4  operator*(f32x4 a, f32x4 b)   { return _mm_mul_ps(a.data, b.data); }
force_inline f32x4& operator*=(f32x4& a, f32x4 b) { return a = a * b; }
force_inline f32x4  operator/(f32x4 a, f32x4 b)   { return _mm_div_ps(a.data, b.data); }
force_inline f32x4& operator/=(f32x4& a, f32x4 b) { return a = a / b; }

force_inline f32x4  operator==(f32x4 a, f32x4 b)  { return _mm_cmpeq_ps(a.data, b.data); }
force_inline f32x4  operator!=(f32x4 a, f32x4 b)  { return _mm_cmpneq_ps(a.data, b.data); }
force_inline f32x4  operator<(f32x4 a, f32x4 b)   { return _mm_cmplt_ps(a.data, b.data); }
force_inline f32x4  operator>(f32x4 a, f32x4 b)   { return _mm_cmpgt_ps(a.data, b.data); }
force_inline f32x4  operator<=(f32x4 a, f32x4 b)  { return _mm_cmple_ps(a.data, b.data); }
force_inline f32x4  operator>=(f32x4 a, f32x4 b)  { return _mm_cmpge_ps(a.data, b.data); }
force_inline bool   AllTrue(f32x4 a)              { return _mm_movemask_ps(a.data) == 0xf; }
force_inline bool   AnyTrue(f32x4 a)              { return _mm_movemask_ps(a.data); }
force_inline int    CountTrue(f32x4 a)            { return CountBits(_mm_movemask_ps(a.data)); }
force_inline int    FirstTrue(f32x4 a)            { return CountTrailingZeroBits(_mm_movemask_ps(a.data)); }
force_inline int    FirstFalse(f32x4 a)           { return CountTrailingZeroBits(~_mm_movemask_ps(a.data)); }
force_inline bool   IsTrue(f32x4 a, int i)        { return _mm_movemask_ps(a.data) & (1 << i); }

force_inline f32x4 min(f32x4 a, f32x4 b)          { return _mm_min_ps(a.data, b.data); }
force_inline f32x4 max(f32x4 a, f32x4 b)          { return _mm_max_ps(a.data, b.data); }

#define _MM_BCAST(a)                              _MM_SHUFFLE(a,a,a,a)

force_inline f32x4 Broadcast0(f32x4 a)            { return _mm_shuffle_ps(a.data, a.data, _MM_BCAST(0)); }
force_inline f32x4 Broadcast1(f32x4 a)            { return _mm_shuffle_ps(a.data, a.data, _MM_BCAST(1)); }
force_inline f32x4 Broadcast2(f32x4 a)            { return _mm_shuffle_ps(a.data, a.data, _MM_BCAST(2)); }
force_inline f32x4 Broadcast3(f32x4 a)            { return _mm_shuffle_ps(a.data, a.data, _MM_BCAST(3)); }

template <class T>
struct iTxN { // 8xint16
	__m128i data;
	
	T& AsT()                       { return *static_cast<T *>(this); }

	T& Load(const void *ptr)       { data = _mm_loadu_si128((__m128i *)ptr); return AsT(); }
	T& Load64(const void *ptr)     { data = _mm_castpd_si128(_mm_load_sd((double *)ptr)); return AsT(); }
	T& Load32(const void *ptr)     { data = _mm_castps_si128(_mm_load_ss((float *)ptr)); return AsT(); }

	void   Store(void *ptr)        { _mm_storeu_si128((__m128i *)ptr, data); }
	void   Store64(void *ptr)      { _mm_store_sd((double *)ptr, _mm_castsi128_pd(data)); }
	void   Store32(void *ptr)      { _mm_store_ss((float *)ptr, _mm_castsi128_ps(data)); }
	void   Stream(void *ptr)       { _mm_stream_si128((__m128i *)ptr, data); };
};

struct i16x8 : iTxN<i16x8> { // 8xint16
	i16x8()                      {}
	i16x8(const void *ptr)       { Load(ptr); }
	i16x8(__m128i d)             { data = d; }
	i16x8(int v)                 { data = _mm_set_epi16(0, 0, 0, 0, 0, 0, 0, v); }
	i16x8(int a, int b, int c, int d, int e, int f, int g, int h)  { data = _mm_set_epi16(a, b, c, d, e, f, g, h); }
	operator __m128i()           { return data; }
};

force_inline i16x8  i16all(int v)                  { return _mm_set1_epi16(v); }

force_inline i16x8  operator+(i16x8 a, i16x8 b)    { return _mm_add_epi16(a.data, b.data); }
force_inline i16x8& operator+=(i16x8& a, i16x8 b)  { return a = a + b; }
force_inline i16x8  operator-(i16x8 a, i16x8 b)    { return _mm_sub_epi16(a.data, b.data); }
force_inline i16x8& operator-=(i16x8& a, i16x8 b)  { return a = a - b; }
force_inline i16x8  operator*(i16x8 a, i16x8 b)    { return _mm_mullo_epi16(a.data, b.data); }
force_inline i16x8& operator*=(i16x8& a, i16x8 b)  { return a = a * b; }

force_inline i16x8  operator&(i16x8 a, i16x8 b)    { return _mm_and_si128(a.data, b.data); }
force_inline i16x8& operator&=(i16x8& a, i16x8 b)  { return a = a & b; }
force_inline i16x8  operator|(i16x8 a, i16x8 b)    { return _mm_or_si128(a.data, b.data); }
force_inline i16x8& operator|=(i16x8& a, i16x8 b)  { return a = a | b; }
force_inline i16x8  operator^(i16x8 a, i16x8 b)    { return _mm_xor_si128(a.data, b.data); }
force_inline i16x8& operator^=(i16x8& a, i16x8 b)  { return a = a ^ b; }
force_inline i16x8  operator~(i16x8 a)             { return _mm_xor_si128(a.data, i16all(0xffff).data); }

force_inline i16x8  operator>>(i16x8 a, int b)     { return _mm_srli_epi16(a.data, b); }
force_inline i16x8& operator>>=(i16x8& a, int b)   { return a = a >> b; }
force_inline i16x8  operator<<(i16x8 a, int b)     { return _mm_slli_epi16(a.data, b); }
force_inline i16x8& operator<<=(i16x8& a, int b)   { return a = a << b; }

force_inline i16x8  operator==(i16x8 a, i16x8 b)   { return _mm_cmpeq_epi16(a.data, b.data); }
force_inline i16x8  operator<(i16x8 a, i16x8 b)    { return _mm_cmplt_epi16(a.data, b.data); }
force_inline i16x8  operator>(i16x8 a, i16x8 b)    { return _mm_cmpgt_epi16(a.data, b.data); }
force_inline bool   AllTrue(i16x8 a)               { return _mm_movemask_epi8(a.data) == 0xffff; }
force_inline bool   AnyTrue(i16x8 a)               { return _mm_movemask_epi8(a.data); }
force_inline int    CountTrue(i16x8 a)             { return CountBits(_mm_movemask_epi8(a.data)) >> 1; }
force_inline int    FirstTrue(i16x8 a)             { return CountTrailingZeroBits(_mm_movemask_epi8(a.data)) >> 1; }
force_inline int    FirstFalse(i16x8 a)             { return CountTrailingZeroBits(~_mm_movemask_epi8(a.data)) >> 1; }
force_inline int    IsTrue(i16x8 a, int i)         { return _mm_movemask_epi8(a.data) & (1 << 2 * i); }

struct i32x4 : iTxN<i32x4> { // 4xint32
	i32x4()                      {}
	i32x4(const void *ptr)       { Load(ptr); }
	i32x4(__m128i d)             { data = d; }
	i32x4(int v)                 { data = _mm_set_epi32(0, 0, 0, v); }
	i32x4(int a, int b, int c, int d)  { data = _mm_set_epi32(a, b, c, d); }
	operator int()               { return _mm_cvtsi128_si32(data); }
	operator i16x8() const       { return i16x8(data); }
};

force_inline i32x4  i32all(int v)                 { return _mm_set1_epi32(v); }

force_inline i32x4  operator+(i32x4 a, i32x4 b)   { return _mm_add_epi32(a.data, b.data); }
force_inline i32x4& operator+=(i32x4& a, i32x4 b) { return a = a + b; }
force_inline i32x4  operator-(i32x4 a, i32x4 b)   { return _mm_sub_epi32(a.data, b.data); }
force_inline i32x4& operator-=(i32x4& a, i32x4 b) { return a = a - b; }

force_inline i32x4  operator&(i32x4 a, i32x4 b)   { return _mm_and_si128(a.data, b.data); }
force_inline i32x4& operator&=(i32x4& a, i32x4 b) { return a = a & b; }
force_inline i32x4  operator|(i32x4 a, i32x4 b)   { return _mm_or_si128(a.data, b.data); }
force_inline i32x4& operator|=(i32x4& a, i32x4 b) { return a = a | b; }
force_inline i32x4  operator^(i32x4 a, i32x4 b)   { return _mm_xor_si128(a.data, b.data); }
force_inline i32x4& operator^=(i32x4& a, i32x4 b) { return a = a ^ b; }
force_inline i32x4  operator~(i32x4 a)            { return _mm_xor_si128(a.data, i32all(0xffffffff).data); }

force_inline i32x4  operator>>(i32x4 a, int b)    { return _mm_srli_epi32(a.data, b); }
force_inline i32x4& operator>>=(i32x4& a, int b)  { return a = a >> b; }
force_inline i32x4  operator<<(i32x4 a, int b)    { return _mm_slli_epi32(a.data, b); }
force_inline i32x4& operator<<=(i32x4& a, int b)  { return a = a << b; }

force_inline i32x4  operator==(i32x4 a, i32x4 b)   { return _mm_cmpeq_epi32(a.data, b.data); }
force_inline i32x4  operator<(i32x4 a, i32x4 b)    { return _mm_cmplt_epi32(a.data, b.data); }
force_inline i32x4  operator>(i32x4 a, i32x4 b)    { return _mm_cmpgt_epi32(a.data, b.data); }
force_inline bool   AllTrue(i32x4 a)               { return _mm_movemask_epi8(a.data) == 0xffff; }
force_inline bool   AnyTrue(i32x4 a)               { return _mm_movemask_ps(_mm_castsi128_ps(a.data)); }
force_inline int    CountTrue(i32x4 a)             { return CountBits(_mm_movemask_ps(_mm_castsi128_ps(a.data))); }
force_inline int    FirstTrue(i32x4 a)             { return CountTrailingZeroBits(_mm_movemask_ps(_mm_castsi128_ps(a.data))); }
force_inline int    FirstFalse(i32x4 a)            { return CountTrailingZeroBits(~_mm_movemask_ps(_mm_castsi128_ps(a.data))); }
force_inline bool   IsTrue(i32x4 a, int i)         { return _mm_movemask_ps(_mm_castsi128_ps(a.data)) & (1 << i); }

struct i8x16 : iTxN<i8x16> { // 16xint8
	i8x16()                      {}
	i8x16(const void *ptr)       { Load(ptr); }
	i8x16(__m128i d)             { data = d; }
	i8x16(int v)                 { data = _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,v); }
	i8x16(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p)
	                             { data = _mm_set_epi8(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p); }
	operator i16x8() const       { return i16x8(data); }
};

force_inline i8x16  i8all(int v)                  { return _mm_set1_epi8(v); }

force_inline i8x16  operator+(i8x16 a, i8x16 b)   { return _mm_add_epi8(a.data, b.data); }
force_inline i8x16& operator+=(i8x16& a, i8x16 b) { return a = a + b; }
force_inline i8x16  operator-(i8x16 a, i8x16 b)   { return _mm_sub_epi8(a.data, b.data); }
force_inline i8x16& operator-=(i8x16& a, i8x16 b) { return a = a - b; }

force_inline i8x16  operator&(i8x16 a, i8x16 b)   { return _mm_and_si128(a.data, b.data); }
force_inline i8x16& operator&=(i8x16& a, i8x16 b) { return a = a & b; }
force_inline i8x16  operator|(i8x16 a, i8x16 b)   { return _mm_or_si128(a.data, b.data); }
force_inline i8x16& operator|=(i8x16& a, i8x16 b) { return a = a | b; }
force_inline i8x16  operator^(i8x16 a, i8x16 b)   { return _mm_xor_si128(a.data, b.data); }
force_inline i8x16& operator^=(i8x16& a, i8x16 b) { return a = a ^ b; }
force_inline i8x16  operator~(i8x16 a)            { return _mm_xor_si128(a.data, i8all(0xff).data); }

force_inline i8x16  operator==(i8x16 a, i8x16 b)   { return _mm_cmpeq_epi8(a.data, b.data); }
force_inline i8x16  operator<(i8x16 a, i8x16 b)    { return _mm_cmplt_epi8(a.data, b.data); }
force_inline i8x16  operator>(i8x16 a, i8x16 b)    { return _mm_cmpgt_epi8(a.data, b.data); }
force_inline bool   AllTrue(i8x16 a)               { return _mm_movemask_epi8(a.data) == 0xffff; }
force_inline bool   AnyTrue(i8x16 a)               { return _mm_movemask_epi8(a.data); }
force_inline int    CountTrue(i8x16 a)             { return CountBits(_mm_movemask_epi8(a.data)); }
force_inline int    FirstTrue(i8x16 a)             { return CountTrailingZeroBits(_mm_movemask_epi8(a.data)); }
force_inline int    FirstFalse(i8x16 a)            { return CountTrailingZeroBits(~_mm_movemask_epi8(a.data)); }
force_inline bool   IsTrue(i8x16 a, int i)         { return _mm_movemask_epi8(a.data) & (1 << i); }

force_inline f32x4 ToFloat(i32x4 a)               { return _mm_cvtepi32_ps(a.data); }
force_inline i32x4 Truncate(f32x4 a)              { return _mm_cvttps_epi32(a.data); }
// force_inline i32x4 Round(f32x4 a)                 { return _mm_cvtps_epi32(a.data); }

force_inline i16x8 Unpack8L(i16x8 a)              { return _mm_unpacklo_epi8(a.data, _mm_setzero_si128()); }
force_inline i16x8 Unpack8H(i16x8 a)              { return _mm_unpackhi_epi8(a.data, _mm_setzero_si128()); }
force_inline i32x4 Unpack16L(i16x8 a)             { return _mm_unpacklo_epi16(a.data, _mm_setzero_si128()); }
force_inline i32x4 Unpack16H(i16x8 a)             { return _mm_unpackhi_epi16(a.data, _mm_setzero_si128()); }

force_inline i8x16 Pack16(i16x8 l, i16x8 h)       { return _mm_packus_epi16(l.data, h.data); }
force_inline i8x16 Pack16(i16x8 l)                { return _mm_packus_epi16(l.data, _mm_setzero_si128()); }
force_inline i16x8 Pack32(i32x4 a)                { return _mm_packs_epi32(a.data, _mm_setzero_si128()); }

force_inline i16x8 BroadcastLH0(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.data, _MM_BCAST(0)), _MM_BCAST(0)); }
force_inline i16x8 BroadcastLH1(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.data, _MM_BCAST(1)), _MM_BCAST(1)); }
force_inline i16x8 BroadcastLH2(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.data, _MM_BCAST(2)), _MM_BCAST(2)); }
force_inline i16x8 BroadcastLH3(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.data, _MM_BCAST(3)), _MM_BCAST(3)); }

force_inline i16x8 i64all(qword data)             { return _mm_set1_epi64x(data); }
