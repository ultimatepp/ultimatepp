#include <arm_neon.h>

using namespace Upp;

struct f32x4 {
	float32x4_t data;

	f32x4& Load(const void *ptr)   { data = vld1q_f32((float *)ptr); return *this; }
	f32x4& Load64(const void *ptr) { data = vcombine_f32(vld1_f32((float *)ptr), vdup_n_f32(0)); return *this; }
	f32x4& Load32(const void *ptr) { data = vsetq_lane_f32(*(float *)ptr, vdupq_n_f32(0), 0); return *this; }

	void   Store(void *ptr)        { vst1q_f32((float32_t *)ptr, data); }
	void   Store64(void *ptr)      { vst1_f32((float32_t *)ptr, vget_low_f32(data)); }
	void   Store32(void *ptr)      { *(float32_t *)ptr = vgetq_lane_f32(data, 0); }
	
	f32x4()                      {}
	f32x4(const void *ptr)       { Load(ptr); }
	f32x4(float32x4_t d)         { data = d; }

	f32x4(double f)              { data = vsetq_lane_f32((float)f, vdupq_n_f32(0), 0); }
	f32x4(float f)               { data = vsetq_lane_f32((float)f, vdupq_n_f32(0), 0); }
	f32x4(int f)                 { data = vsetq_lane_f32((float)f, vdupq_n_f32(0), 0); }
	f32x4(double a, double b, double c, double d) {
		float __attribute__((aligned(16))) w[4] = { (float)d, (float)c, (float)b, (float)a };
		data = vld1q_f32(w);
	}
	
	operator float32x4_t()       { return data; }
};

force_inline int GetMaskf32x4(f32x4 a)
{
    uint32x4_t shifted = vshrq_n_u32(vreinterpretq_u32_f32(a.data), 31);
    return (vgetq_lane_u32(shifted, 0) << 0) |
           (vgetq_lane_u32(shifted, 1) << 1) |
           (vgetq_lane_u32(shifted, 2) << 2) |
           (vgetq_lane_u32(shifted, 3) << 3);
}

force_inline f32x4  f32all(double f) { return vdupq_n_f32((float)f); }

force_inline f32x4  operator+(f32x4 a, f32x4 b)   { return vaddq_f32(a, b); }
force_inline f32x4& operator+=(f32x4& a, f32x4 b) { return a = a + b; }
force_inline f32x4  operator-(f32x4 a, f32x4 b)   { return vsubq_f32(a, b); }
force_inline f32x4& operator-=(f32x4& a, f32x4 b) { return a = a - b; }
force_inline f32x4  operator*(f32x4 a, f32x4 b)   { return vmulq_f32(a, b); }
force_inline f32x4& operator*=(f32x4& a, f32x4 b) { return a = a * b; }

force_inline f32x4  operator/(f32x4 a, f32x4 b)   {
	float32x4_t reciprocal = vrecpeq_f32(b);
	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);
	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);
	return vmulq_f32(a,reciprocal);
}

force_inline f32x4& operator/=(f32x4& a, f32x4 b) { return a = a / b; }

force_inline f32x4  operator==(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vceqq_f32(a, b)); }
force_inline f32x4  operator!=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(a, b))); }
force_inline f32x4  operator<(f32x4 a, f32x4 b)   { return vreinterpretq_f32_u32(vcltq_f32(a, b)); }
force_inline f32x4  operator>(f32x4 a, f32x4 b)   { return vreinterpretq_f32_u32(vcgtq_f32(a, b)); }
force_inline f32x4  operator<=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vcleq_f32(a, b)); }
force_inline f32x4  operator>=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vcgeq_f32(a, b)); }

force_inline bool   AllTrue(uint32x4_t v) {
    uint32x2_t tmp = vand_u32(vget_low_u32(v), vget_high_u32(v));
    return vget_lane_u32(vpmin_u32(tmp, tmp), 0) == 0xffffffff;;
}

force_inline bool   AllTrue(f32x4 a) {
	return AllTrue(vreinterpretq_u32_f32(a));
}

force_inline f32x4 min(f32x4 a, f32x4 b)          { return vminq_f32(a, b); }
force_inline f32x4 max(f32x4 a, f32x4 b)          { return vmaxq_f32(a, b); }

force_inline f32x4 Broadcast0(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 0)); }
force_inline f32x4 Broadcast1(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 1)); }
force_inline f32x4 Broadcast2(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 2)); }
force_inline f32x4 Broadcast3(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 3)); }

struct i16x8 { // 8xint16
	int16x8_t data;

	i16x8& Load(const void *ptr)   { data = vld1q_s16((int16_t *)ptr); return *this; }
	i16x8& Load64(const void *ptr) { data = vcombine_s16(vld1_s16((int16_t *)ptr), vdup_n_s16(0)); return *this; }
	i16x8& Load32(const void *ptr) { data = vreinterpretq_s16_s32(vsetq_lane_s32(*(int32_t *)ptr, vdupq_n_s32(0), 0)); return *this; }

	void   Store(void *ptr)      { vst1q_s16((int16_t *)ptr, data); }
	void   Store64(void *ptr)    { vst1_s16((int16_t *)ptr, vget_low_s16(data)); }
	void   Store32(void *ptr)    { *(int32_t *)ptr = vgetq_lane_s32(vreinterpretq_s32_s16(data), 0); }

	i16x8()                      {}
	i16x8(const void *ptr)       { Load(ptr); }
	i16x8(int16x8_t d)           { data = d; }
	i16x8(int8x16_t d)           { data = vreinterpretq_s16_s8(d); }
	i16x8(int32x4_t d)           { data = vreinterpretq_s16_s32(d); }
	i16x8(int v)                 { data = vsetq_lane_s16(v, vdupq_n_s16(0), 0); }
	i16x8(int a, int b, int c, int d, int e, int f, int g, int h) {
		int16_t __attribute__((aligned(16))) val[8] = { (int16_t)h, (int16_t)g, (int16_t)f, (int16_t)e, (int16_t)d, (int16_t)c, (int16_t)b, (int16_t)a };
		data = vld1q_s16(val);
	}

	operator int16x8_t()         { return data; }
};

force_inline int GetMaski16x8(i16x8 a)
{
    uint16x8_t shifted = vshrq_n_u16(vreinterpretq_u16_s16(a.data), 15);
    return (vgetq_lane_u16(shifted, 0) << 0) |
           (vgetq_lane_u16(shifted, 1) << 1) |
           (vgetq_lane_u16(shifted, 2) << 2) |
           (vgetq_lane_u16(shifted, 3) << 3) |
           (vgetq_lane_u16(shifted, 4) << 4) |
           (vgetq_lane_u16(shifted, 5) << 5) |
           (vgetq_lane_u16(shifted, 6) << 6) |
           (vgetq_lane_u16(shifted, 7) << 7);
}

force_inline i16x8  i16all(int v)                  { return vdupq_n_s16(v); }

force_inline i16x8  operator+(i16x8 a, i16x8 b)    { return vaddq_s16(a, b); }
force_inline i16x8& operator+=(i16x8& a, i16x8 b)  { return a = a + b; }
force_inline i16x8  operator-(i16x8 a, i16x8 b)    { return vsubq_s16(a, b); }
force_inline i16x8& operator-=(i16x8& a, i16x8 b)  { return a = a - b; }
force_inline i16x8  operator*(i16x8 a, i16x8 b)    { return vmulq_s16(a, b); }
force_inline i16x8& operator*=(i16x8& a, i16x8 b)  { return a = a * b; }

force_inline i16x8  operator&(i16x8 a, i16x8 b)    { return vandq_s16(a, b); }
force_inline i16x8& operator&=(i16x8& a, i16x8 b)  { return a = a & b; }
force_inline i16x8  operator|(i16x8 a, i16x8 b)    { return vorrq_s16(a, b); }
force_inline i16x8& operator|=(i16x8& a, i16x8 b)  { return a = a | b; }
force_inline i16x8  operator^(i16x8 a, i16x8 b)    { return veorq_s16(a, b); }
force_inline i16x8& operator^=(i16x8& a, i16x8 b)  { return a = a ^ b; }
force_inline i16x8  operator~(i16x8 a)             { return vmvnq_s16(a); }

force_inline i16x8  operator>>(i16x8 a, int b)     { return vreinterpretq_s16_u16(vshlq_u16(vreinterpretq_u16_s16(a), vdupq_n_s16(-b))); }
force_inline i16x8& operator>>=(i16x8& a, int b)   { return a = a >> b; }
force_inline i16x8  operator<<(i16x8 a, int b)     { return vshlq_s16(a, vdupq_n_s16(b)); }
force_inline i16x8& operator<<=(i16x8& a, int b)   { return a = a << b; }

force_inline i16x8  operator==(i16x8 a, i16x8 b)  { return vreinterpretq_s16_u16(vceqq_s16(a, b)); }
force_inline i16x8  operator<(i16x8 a, i16x8 b)   { return vreinterpretq_s16_u16(vcltq_s16(a, b)); }
force_inline i16x8  operator>(i16x8 a, i16x8 b)   { return vreinterpretq_s16_u16(vcgtq_s16(a, b)); }

force_inline bool   AllTrue(i16x8 a) {
	return AllTrue(vreinterpretq_u32_s16(a));
}

struct i32x4 { // 4xint32
	int32x4_t data;

	i32x4& Load(const void *ptr)   { data = vld1q_s32((int32_t *)ptr); return *this; }
	i32x4& Load64(const void *ptr) { data = vcombine_s32(vld1_s32((int32_t *)ptr), vdup_n_s32(0)); return *this; }
	i32x4& Load32(const void *ptr) { data = vsetq_lane_s32(*(int32_t *)ptr, vdupq_n_s32(0), 0); return *this; }

	void   Store(void *ptr)      { vst1q_s32((int32_t *)ptr, data); }
	void   Store64(void *ptr)    { vst1_s32((int32_t *)ptr, vget_low_s32(data)); }
	void   Store32(void *ptr)    { *(int32_t *)ptr = vgetq_lane_s32(data, 0); }

	i32x4()                      {}
	i32x4(const void *ptr)       { Load(ptr); }
	i32x4(int32x4_t d)           { data = d; }
	i32x4(int8x16_t d)           { data = vreinterpretq_s32_s8(d); }
	i32x4(int16x8_t d)           { data = vreinterpretq_s32_s16(d); }
	i32x4(int v)                 { data = vsetq_lane_s32(v, vdupq_n_s32(0), 0); }
	i32x4(int a, int b, int c, int d)  {
		int32_t __attribute__((aligned(16))) val[4] = { (int32_t)d, (int32_t)c, (int32_t)b, (int32_t)a };
		data = vld1q_s32(val);
	}
	operator int32x4_t()         { return data; }
	operator int()               { return vgetq_lane_s32(data, 0); }
	operator i16x8() const       { return i16x8(data); }
};

force_inline int GetMaski32x4(i32x4 a)
{
    uint32x4_t shifted = vshrq_n_u32(vreinterpretq_u32_s32(a.data), 31);
    return (vgetq_lane_u32(shifted, 0) << 0) |
           (vgetq_lane_u32(shifted, 1) << 1) |
           (vgetq_lane_u32(shifted, 2) << 2) |
           (vgetq_lane_u32(shifted, 3) << 3);
}

force_inline i32x4  i32all(int v)                 { return vdupq_n_s32(v); }

force_inline i32x4  operator+(i32x4 a, i32x4 b)   { return vaddq_s32(a, b); }
force_inline i32x4& operator+=(i32x4& a, i32x4 b) { return a = a + b; }
force_inline i32x4  operator-(i32x4 a, i32x4 b)   { return vsubq_s32(a, b); }
force_inline i32x4& operator-=(i32x4& a, i32x4 b) { return a = a - b; }

force_inline i32x4  operator&(i32x4 a, i32x4 b)    { return vandq_s32(a, b); }
force_inline i32x4& operator&=(i32x4& a, i32x4 b)  { return a = a & b; }
force_inline i32x4  operator|(i32x4 a, i32x4 b)    { return vorrq_s32(a, b); }
force_inline i32x4& operator|=(i32x4& a, i32x4 b)  { return a = a | b; }
force_inline i32x4  operator^(i32x4 a, i32x4 b)    { return veorq_s32(a, b); }
force_inline i32x4& operator^=(i32x4& a, i32x4 b)  { return a = a ^ b; }
force_inline i32x4  operator~(i32x4 a)             { return vmvnq_s32(a); }

force_inline i32x4  operator>>(i32x4 a, int b)     { return vshlq_s32(a, vdupq_n_s32(-b)); }
force_inline i32x4& operator>>=(i32x4& a, int b)   { return a = a >> b; }
force_inline i32x4  operator<<(i32x4 a, int b)     { return vshlq_s32(a, vdupq_n_s32(b)); }
force_inline i32x4& operator<<=(i32x4& a, int b)   { return a = a << b; }

force_inline i32x4  operator==(i32x4 a, i32x4 b)  { return vreinterpretq_s32_u32(vceqq_s32(a, b)); }
force_inline i32x4  operator<(i32x4 a, i32x4 b)   { return vreinterpretq_s32_u32(vcltq_s32(a, b)); }
force_inline i32x4  operator>(i32x4 a, i32x4 b)   { return vreinterpretq_s32_u32(vcgtq_s32(a, b)); }

force_inline bool   AllTrue(i32x4 a) {
	return AllTrue(vreinterpretq_u32_s32(a));
}

struct i8x16 { // 16*int8
	int8x16_t data;

	i8x16& Load(const void *ptr)   { data = vld1q_s8((int8_t *)ptr); return *this; }
	i8x16& Load64(const void *ptr) { data = vcombine_s8(vld1_s8((int8_t *)ptr), vdup_n_s8(0)); return *this; }
	i8x16& Load32(const void *ptr) { data = vreinterpretq_s8_s32(vsetq_lane_s32(*(int32_t *)ptr, vdupq_n_s32(0), 0)); return *this; }

	void   Store(void *ptr)      { vst1q_s8((int8_t *)ptr, data); }
	void   Store64(void *ptr)    { vst1_s32((int32_t *)ptr, vget_low_s32(vreinterpretq_s32_s8(data))); }
	void   Store32(void *ptr)    { *(int32_t *)ptr = vgetq_lane_s32(vreinterpretq_s32_s8(data), 0); }

	i8x16()                      {}
	i8x16(const void *ptr)       { Load(ptr); }
	i8x16(int8x16_t d)           { data = d; }
	i8x16(int16x8_t d)           { data = vreinterpretq_s8_s16(d); }
	i8x16(int32x4_t d)           { data = vreinterpretq_s8_s32(d); }
	i8x16(int v)                 { data = vsetq_lane_s8(v, vdupq_n_s8(0), 0); }
	i8x16(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p)
	{
		int8_t __attribute__((aligned(16))) val[16] = {
			(int8_t)p, (int8_t)o, (int8_t)n, (int8_t)m,
			(int8_t)l, (int8_t)k, (int8_t)j, (int8_t)i,
			(int8_t)h, (int8_t)g, (int8_t)f, (int8_t)e,
			(int8_t)d, (int8_t)c, (int8_t)b, (int8_t)a,
		};
		data = vld1q_s8(val);
	}
	operator int8x16_t() const   { return data; }
	operator i16x8() const       { return i16x8(data); }
};

force_inline int GetMaski8x16(i8x16 a)
{
    uint8x16_t shifted = vshrq_n_u8(vreinterpretq_u8_s8(a.data), 7);
    return (vgetq_lane_u8(shifted, 0) << 0)   |
           (vgetq_lane_u8(shifted, 1) << 1)   |
           (vgetq_lane_u8(shifted, 2) << 2)   |
           (vgetq_lane_u8(shifted, 3) << 3)   |
           (vgetq_lane_u8(shifted, 4) << 4)   |
           (vgetq_lane_u8(shifted, 5) << 5)   |
           (vgetq_lane_u8(shifted, 6) << 6)   |
           (vgetq_lane_u8(shifted, 7) << 7)   |
           (vgetq_lane_u8(shifted, 8) << 8)   |
           (vgetq_lane_u8(shifted, 9) << 9)   |
           (vgetq_lane_u8(shifted, 10) << 10) |
           (vgetq_lane_u8(shifted, 11) << 11) |
           (vgetq_lane_u8(shifted, 12) << 12) |
           (vgetq_lane_u8(shifted, 13) << 13) |
           (vgetq_lane_u8(shifted, 14) << 14) |
           (vgetq_lane_u8(shifted, 15) << 15);
}

force_inline i8x16  i8all(int v)                  { return vdupq_n_s8(v); }

force_inline i8x16  operator+(i8x16 a, i8x16 b)   { return vaddq_s8(a, b); }
force_inline i8x16& operator+=(i8x16& a, i8x16 b) { return a = a + b; }
force_inline i8x16  operator-(i8x16 a, i8x16 b)   { return vsubq_s8(a, b); }
force_inline i8x16& operator-=(i8x16& a, i8x16 b) { return a = a - b; }

force_inline i8x16  operator&(i8x16 a, i8x16 b)    { return vandq_s8(a, b); }
force_inline i8x16& operator&=(i8x16& a, i8x16 b)  { return a = a & b; }
force_inline i8x16  operator|(i8x16 a, i8x16 b)    { return vorrq_s8(a, b); }
force_inline i8x16& operator|=(i8x16& a, i8x16 b)  { return a = a | b; }
force_inline i8x16  operator^(i8x16 a, i8x16 b)    { return veorq_s8(a, b); }
force_inline i8x16& operator^=(i8x16& a, i8x16 b)  { return a = a ^ b; }
force_inline i8x16  operator~(i8x16 a)             { return vmvnq_s8(a); }

force_inline f32x4 ToFloat(i32x4 a)               { return vcvtq_f32_s32(a); }
force_inline i32x4 Truncate(f32x4 a)              { return vcvtq_s32_f32(a); }

force_inline i16x8 Unpack8L(i16x8 a)              { return vzipq_s8(vreinterpretq_s8_s16(a), vdupq_n_s8(0)).val[0]; }
force_inline i16x8 Unpack8H(i16x8 a)              { return vzipq_s8(vreinterpretq_s8_s16(a), vdupq_n_s8(0)).val[1]; }

force_inline i32x4 Unpack16L(i16x8 a)             { return vzipq_s16(a, vdupq_n_s16(0)).val[0]; }
force_inline i32x4 Unpack16H(i16x8 a)             { return vzipq_s16(a, vdupq_n_s16(0)).val[1]; }

force_inline i8x16 Pack16(i16x8 l, i16x8 h)       { return vreinterpretq_s8_u8(vcombine_u8(vqmovun_s16(l), vqmovun_s16(h))); }
force_inline i8x16 Pack16(i16x8 l)                { return vreinterpretq_s8_u8(vcombine_u8(vqmovun_s16(l), vdup_n_u8(0))); }

force_inline i16x8 Pack32(i32x4 a)                { return vcombine_s16(vqmovn_s32(a), vdup_n_s16(0)); }

force_inline i16x8 BroadcastLH0(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 0)), vdup_n_s16(vgetq_lane_s16(a, 4)));
}

force_inline i16x8 BroadcastLH1(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 1)), vdup_n_s16(vgetq_lane_s16(a, 5)));
}

force_inline i16x8 BroadcastLH2(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 2)), vdup_n_s16(vgetq_lane_s16(a, 6)));
}

force_inline i16x8 BroadcastLH3(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 3)), vdup_n_s16(vgetq_lane_s16(a, 7)));
}

force_inline i16x8 i64all(qword data)             { return vreinterpretq_s16_u64(vdupq_n_u64(data)); }
