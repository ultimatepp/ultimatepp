#ifndef CPU_LE
#error Only little endian CPUs supported
#endif

#if defined(CPU_X86) && defined(COMPILER_MSC)

#ifdef COMPILER_GCC
#ifdef CPU_64
inline word   SwapEndian16(word v)    { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
inline int16  SwapEndian16(int16 v)   { __asm__("xchgb %b0,%h0" : "=Q" (v) :  "0" (v)); return v; }
#else
inline word   SwapEndian16(word v)    { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
inline int16  SwapEndian16(int16 v)   { __asm__("xchgb %b0,%h0" : "=q" (v) :  "0" (v)); return v; }
#endif
inline dword  SwapEndian32(dword v)   { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
inline int    SwapEndian32(int v)     { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif

#ifdef COMPILER_MSC
#pragma intrinsic (_byteswap_ushort, _byteswap_ulong, _byteswap_uint64, strlen)

inline word   SwapEndian16(word v)    { return _byteswap_ushort(v); }
inline int16  SwapEndian16(int16 v)   { return _byteswap_ushort(v); }
inline dword  SwapEndian32(dword v)   { return _byteswap_ulong(v); }
inline int    SwapEndian32(int v)     { return _byteswap_ulong(v); }
#endif

inline void   EndianSwap(word& v)     { v = SwapEndian16(v); }
inline void   EndianSwap(int16& v)    { v = SwapEndian16(v); }
inline void   EndianSwap(dword& v)    { v = SwapEndian32(v); }
inline void   EndianSwap(int& v)      { v = SwapEndian32(v); }

#else

#ifdef COMPILER_GCC

inline dword  SwapEndian32(dword v)   { return __builtin_bswap32(v); }
inline int    SwapEndian32(int v)     { return __builtin_bswap32(v); }

inline word   SwapEndian16(word v)    { return SwapEndian32(v) >> 16; } // GCC bug workaround
inline int16  SwapEndian16(int16 v)   { return SwapEndian32(v) >> 16; }

inline void   EndianSwap(word& v)     { v = SwapEndian16(v); }
inline void   EndianSwap(int16& v)    { v = SwapEndian16(v); }
inline void   EndianSwap(dword& v)    { v = SwapEndian32(v); }
inline void   EndianSwap(int& v)      { v = SwapEndian32(v); }

#else

inline void   EndianSwap(word& v)     { byte *x = (byte *)(&v); Swap(x[0], x[1]); }
inline void   EndianSwap(int16& v)    { EndianSwap(*(word *)&v); }
inline void   EndianSwap(dword& v)    { byte *x = (byte *)&v; Swap(x[0], x[3]); Swap(x[1], x[2]); }
inline void   EndianSwap(int& v)      { EndianSwap(*(dword *)&v); }
inline word   SwapEndian16(word v)    { EndianSwap(v); return v; }
inline int16  SwapEndian16(int16 v)   { EndianSwap(v); return v; }
inline dword  SwapEndian32(dword v)   { EndianSwap(v); return v; }
inline int    SwapEndian32(int v)     { EndianSwap(v); return v; }

#endif

#endif

#if defined(CPU_AMD64) && (defined(COMPILER_GCC) || defined(COMPILER_MSC))
#ifdef COMPILER_GCC
inline uint64  SwapEndian64(uint64 v) { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
inline int64   SwapEndian64(int64 v)  { __asm__("bswap %0" : "=r" (v) : "0" (v)); return v; }
#endif
#ifdef COMPILER_MSC
inline uint64  SwapEndian64(uint64 v) { return _byteswap_uint64(v); }
inline int64   SwapEndian64(int64 v)  { return _byteswap_uint64(v); }
#endif

inline void   EndianSwap(int64& v)    { v = SwapEndian64(v); }
inline void   EndianSwap(uint64& v)   { v = SwapEndian64(v); }

#else

#ifdef COMPILER_GCC

inline uint64  SwapEndian64(uint64 v) { return __builtin_bswap64(v); }
inline int64   SwapEndian64(int64 v)  { return __builtin_bswap64(v); }

inline void    EndianSwap(int64& v)   { v = SwapEndian64(v); }
inline void    EndianSwap(uint64& v)  { v = SwapEndian64(v); }

#else
inline void   EndianSwap(int64& v)    { byte *x = (byte *)&v; Swap(x[0], x[7]); Swap(x[1], x[6]); Swap(x[2], x[5]); Swap(x[3], x[4]); }
inline void   EndianSwap(uint64& v)   { EndianSwap(*(int64 *)&v); }
inline int64  SwapEndian64(int64 v)   { EndianSwap(v); return v; }
inline uint64 SwapEndian64(uint64 v)  { EndianSwap(v); return v; }
#endif
#endif

inline word   SwapEndian16(int w)     { return SwapEndian16((word)w); }
inline word   SwapEndian16(dword w)   { return SwapEndian16((word)w); }

void EndianSwap(word *v, size_t count);
void EndianSwap(int16 *v, size_t count);
void EndianSwap(dword *v, size_t count);
void EndianSwap(int *v, size_t count);
void EndianSwap(int64 *v, size_t count);
void EndianSwap(uint64 *v, size_t count);

// unligned access - memcpy converts to simple load/store with normal compilers

inline int    Peek16(const void *ptr)        { word x; memcpy(&x, ptr, 2); return x; }
inline int    Peek32(const void *ptr)        { dword x; memcpy(&x, ptr, 4); return x; }
inline int64  Peek64(const void *ptr)        { uint64 x; memcpy(&x, ptr, 8); return x; }

inline void   Poke16(void *ptr, word val)    { memcpy(ptr, &val, 2); }
inline void   Poke32(void *ptr, dword val)   { memcpy(ptr, &val, 4); }
inline void   Poke64(void *ptr, int64 val)   { memcpy(ptr, &val, 8); }

inline int    Peek16le(const void *ptr)      { return Peek16(ptr); }
inline int    Peek32le(const void *ptr)      { return Peek32(ptr); }
inline int64  Peek64le(const void *ptr)      { return Peek64(ptr); }

inline void   Poke16le(void *ptr, word val)   { Poke16(ptr, val); }
inline void   Poke32le(void *ptr, dword val)   { Poke32(ptr, val); }
inline void   Poke64le(void *ptr, int64 val) { Poke64(ptr, val); }

inline int    Peek16be(const void *ptr)      { return SwapEndian16(Peek16(ptr)); }
inline int    Peek32be(const void *ptr)      { return SwapEndian32(Peek32(ptr)); }
inline int64  Peek64be(const void *ptr)      { return SwapEndian64(Peek64(ptr)); }

inline void   Poke16be(void *ptr, word val)  { Poke16(ptr, SwapEndian16(val)); }
inline void   Poke32be(void *ptr, dword val) { Poke32(ptr, SwapEndian32(val)); }
inline void   Poke64be(void *ptr, int64 val) { Poke64(ptr, SwapEndian64(val)); }

#define MAKE2B(b0, b1)                            MAKEWORD(b0, b1)
#define MAKE4B(b0, b1, b2, b3)                    MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#define MAKE8B(b0, b1, b2, b3, b4, b5, b6, b7)    MAKEQWORD(MAKE4B(b0, b1, b2, b3), MAKE4B(b4, b5, b6, b7))

#ifdef CPU_64

#define HASH64

#define HASH_CONST1 I64(0xf7c21089bee7c0a5)
#define HASH_CONST2 I64(0xc85abc8da7534a4d)
#define HASH_CONST3 I64(0x8642b0fe3e86671b)

typedef qword hash_t;

inline dword FoldHash(qword h)
{
	return (dword)SwapEndian64(HASH_CONST3 * h);
}

#else

#define HASH_CONST1 0xbee7c0a5
#define HASH_CONST2 0xa7534a4d
#define HASH_CONST3 0x8e86671b

typedef dword hash_t;

inline dword FoldHash(dword h)
{
	return SwapEndian32(HASH_CONST3 * h);
}

#endif

inline byte Saturate255(int x)             { return byte(~(x >> 24) & (x | (-(x >> 8) >> 24)) & 0xff); }

force_inline
int SignificantBits(dword x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
	DWORD index;
	return _BitScanReverse(&index, x) ? index + 1 : 0;
#else
	return x ? 32 - __builtin_clz(x) : 0;
#endif
}

force_inline
int SignificantBits64(uint64 x)
{ // basically log2(x) + 1 except that for 0 this is 0, number of significant bits of x
#ifdef COMPILER_MSC
#ifdef CPU_64
	DWORD index;
	return _BitScanReverse64(&index, x) ? index + 1 : 0;
#else
	if(x & 0xffffffff00000000)
		return SignificantBits(HIDWORD(x)) + 32;
	else
		return SignificantBits((DWORD)x);
#endif
#else
	return x ? 64 - __builtin_clzll(x) : 0;
#endif
}

inline bool FitsInInt64(double x)
{
	return x >= -9223372036854775808.0 && x < 9223372036854775808.0;
}

#if defined(__SIZEOF_INT128__) && (__GNUC__ > 5 || __clang_major__ >= 5)

#ifdef CPU_X86

inline
byte addc64(uint64& result, const uint64& value, byte carry) {
	return _addcarry_u64(carry, result, value, &result);
}

#else

force_inline
byte addc64(uint64& r, uint64 a, byte carry)
{
	r += a + carry;
	return carry ? r <= a : r < a;
}

#endif

inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	unsigned __int128 prod =  (unsigned __int128)a * b;
	hi = prod >> 64;
	return prod;
}

#elif defined(COMPILER_MSC) && defined(CPU_64)

inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	return _umul128(a, b, &hi);
}

inline
byte addc64(uint64& result, const uint64& value, byte carry) {
	return _addcarry_u64(carry, result, value, &result);
}

#else

force_inline
byte addc64(uint64& r, uint64 a, byte carry)
{
	r += a + carry;
	return carry ? r <= a : r < a;
}

force_inline
uint64 mul64(uint64 a, uint64 b, uint64& hi)
{
	uint64 lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
	uint64 hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
	uint64 lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
	uint64 hi_hi = (a >> 32)        * (b >> 32);
	
	uint64 cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
	hi = (hi_lo >> 32) + (cross >> 32)        + hi_hi;

	return (cross << 32) | (lo_lo & 0xFFFFFFFF);
}

#endif
