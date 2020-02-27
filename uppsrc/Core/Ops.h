#if defined(CPU_UNALIGNED) && defined(CPU_LE) || __ARM_ARCH_7A__
NOUBSAN inline int    Peek16le(const void *ptr)  { return *(const word *)ptr; }
NOUBSAN inline int    Peek32le(const void *ptr)  { return *(const dword *)ptr; }
NOUBSAN inline int64  Peek64le(const void *ptr)  { return *(const int64 *)ptr; }

NOUBSAN inline void   Poke16le(const void *ptr, int val)    { *(word *)ptr = val; }
NOUBSAN inline void   Poke32le(const void *ptr, int val)    { *(dword *)ptr = val; }
NOUBSAN inline void   Poke64le(const void *ptr, int64 val)  { *(int64 *)ptr = val; }
#else
inline int    Peek16le(const void *ptr)  { return MAKEWORD(((byte *)ptr)[0], ((byte *)ptr)[1]); }
inline int    Peek32le(const void *ptr)  { return MAKELONG(Peek16le(ptr), Peek16le((byte *)ptr + 2)); }
inline int64  Peek64le(const void *ptr)  { return MAKEQWORD(Peek32le(ptr), Peek32le((byte *)ptr + 4)); }

inline void   Poke16le(const void *ptr, int val)    { ((byte *)ptr)[0] = LOBYTE(val); ((byte *)ptr)[1] = HIBYTE(val); }
inline void   Poke32le(const void *ptr, int val)    { Poke16le(ptr, LOWORD(val)); Poke16le((byte *)ptr + 2, HIWORD(val)); }
inline void   Poke64le(const void *ptr, int64 val)  { Poke32le(ptr, LODWORD(val)); Poke32le((byte *)ptr + 4, HIDWORD(val)); }
#endif

inline int    Peek16be(const void *ptr)  { return MAKEWORD(((byte *)ptr)[1], ((byte *)ptr)[0]); }
inline int    Peek32be(const void *ptr)  { return MAKELONG(Peek16be((byte *)ptr + 2), Peek16be(ptr)); }
inline int64  Peek64be(const void *ptr)  { return MAKEQWORD(Peek32be((byte *)ptr + 4), Peek32be(ptr)); }

inline void   Poke16be(const void *ptr, int val)    { ((byte *)ptr)[1] = LOBYTE(val); ((byte *)ptr)[0] = HIBYTE(val); }
inline void   Poke32be(const void *ptr, int val)    { Poke16be(ptr, HIWORD(val)); Poke16be((byte *)ptr + 2, LOWORD(val)); }
inline void   Poke64be(const void *ptr, int64 val)  { Poke32be(ptr, HIDWORD(val)); Poke32be((byte *)ptr + 4, LODWORD(val)); }

#ifdef CPU_LITTLE_ENDIAN
#define MAKE2B(b0, b1)                  MAKEWORD(b0, b1);
#define MAKE4B(b0, b1, b2, b3)          MAKELONG(MAKEWORD(b0, b1), MAKEWORD(b2, b3))
#else
#define MAKE2B(b0, b1)                  MAKEWORD(b1, b0);
#define MAKE4B(b0, b1, b2, b3)          MAKELONG(MAKEWORD(b2, b3), MAKEWORD(b0, b1))
#endif

#if defined(CPU_X86) && (defined(COMPILER_GCC) || defined(COMPILER_MSC))
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

#ifdef COMPILE_GCC

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

void EndianSwap(word *v, size_t count);
void EndianSwap(int16 *v, size_t count);
void EndianSwap(dword *v, size_t count);
void EndianSwap(int *v, size_t count);
void EndianSwap(int64 *v, size_t count);
void EndianSwap(uint64 *v, size_t count);

#define SVO_MEMCPY(tgt, src, len) \
do {  \
	const char *s__ = (const char *)(src); \
	char *t__ = (char *)(tgt); \
	switch(len) { \
	case 11: t__[10] = s__[10]; \
	case 10: t__[9] = s__[9]; \
	case  9: t__[8] = s__[8]; \
	case  8: t__[7] = s__[7]; \
	case  7: t__[6] = s__[6]; \
	case  6: t__[5] = s__[5]; \
	case  5: t__[4] = s__[4]; \
	case  4: t__[3] = s__[3]; \
	case  3: t__[2] = s__[2]; \
	case  2: t__[1] = s__[1]; \
	case  1: t__[0] = s__[0]; \
	case  0: \
		break; \
	default: \
		memcpy(t__, s__, len); \
	} \
} while(false)

#define SVO_MEMSET(tgt, val, len) \
do { \
	char *t__ = (char *)(tgt); \
	switch(len) { \
	case 11: t__[10] = val; \
	case 10: t__[9] = val; \
	case  9: t__[8] = val; \
	case  8: t__[7] = val; \
	case  7: t__[6] = val; \
	case  6: t__[5] = val; \
	case  5: t__[4] = val; \
	case  4: t__[3] = val; \
	case  3: t__[2] = val; \
	case  2: t__[1] = val; \
	case  1: t__[0] = val; \
	case  0: \
		break; \
	default: \
		memset(t__, val, len); \
	} \
} while(false)

template <class tchar>
force_inline bool svo_memeq(const tchar *a, const tchar *b, int len)
{
	if(len > 11)
		return memcmp(a, b, len * sizeof(tchar)) == 0;
	switch(len) {
	case 11:
		if(a[10] != b[10]) return false;
	case 10:
		if(a[9] != b[9]) return false;
	case 9:
		if(a[8] != b[8]) return false;
	case 8:
		if(a[7] != b[7]) return false;
	case 7:
		if(a[6] != b[6]) return false;
	case 6:
		if(a[5] != b[5]) return false;
	case 5:
		if(a[4] != b[4]) return false;
	case 4:
		if(a[3] != b[3]) return false;
	case 3:
		if(a[2] != b[2]) return false;
	case 2:
		if(a[1] != b[1]) return false;
	case 1:
		if(a[0] != b[0]) return false;
	}
	return true;
}

force_inline bool fast_equal128(const void *a, const void *b)
{
	uint32 *aa = (uint32 *)a;
	uint32 *bb = (uint32 *)b;
	return ((aa[0] ^ bb[0]) | (aa[1] ^ bb[1]) | (aa[2] ^ bb[2]) | (aa[3] ^ bb[3])) == 0;
}

force_inline void fast_zero128(void *t)
{
	uint32 *tt = (uint32 *)t;
	tt[0] = tt[1] = tt[2] = tt[3] = 0;
}

force_inline void fast_copy128(void *t, const void *s)
{
	uint32 *tt = (uint32 *)t;
	uint32 *ss = (uint32 *)s;
	tt[0] = ss[0];
	tt[1] = ss[1];
	tt[2] = ss[2];
	tt[3] = ss[3];
}

#if defined(CPU_UNALIGNED) && defined(CPU_LE) && (defined(COMPILER_MSC) || defined(COMPILER_GCC))
#define FAST_STRING_COMPARE
#endif

#ifdef FAST_STRING_COMPARE
force_inline
int fast_memcmp(const char *a, const char *b, size_t len)
{
#ifdef CPU_64
	while(len >= 8) {
		uint64 a64 = *(uint64 *)a;
		uint64 b64 = *(uint64 *)b;
		if(a64 != b64)
			return SwapEndian64(a64) < SwapEndian64(b64) ? -1 : 1;
		a += 8;
		b += 8;
		len -= 8;
	}
	if(len & 4) {
		uint32 a32 = *(uint32 *)a;
		uint32 b32 = *(uint32 *)b;
		if(a32 != b32)
			return SwapEndian32(a32) < SwapEndian32(b32) ? -1 : 1;
		a += 4;
		b += 4;
	}
#else
	while(len >= 4) {
		uint32 a32 = *(uint32 *)a;
		uint32 b32 = *(uint32 *)b;
		if(a32 != b32)
			return SwapEndian32(a32) < SwapEndian32(b32) ? -1 : 1;
		a += 4;
		b += 4;
		len -= 4;
	}
#endif
	if(len & 2) {
		uint16 a16 = *(uint16 *)a;
		uint16 b16 = *(uint16 *)b;
		if(a16 != b16)
			return SwapEndian16(a16) < SwapEndian16(b16) ? -1 : 1;
		a += 2;
		b += 2;
	}
	if((len & 1) != 0 && *a != *b)
		return (byte)*a < (byte)*b ? -1 : 1;
	return 0;
}
#else
inline
int fast_memcmp(const char *a, const char *b, size_t len)
{
	return memcmp(a, b, len);
}
#endif

inline dword FoldHash(dword h)
{
	return SwapEndian32(0xa3613c16 * h);
}

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
	return x ? 64 - __builtin_clzl(x) : 0;
#endif
}

inline bool FitsInInt64(double x)
{
	return x >= -9223372036854775808.0 && x < 9223372036854775808.0;
}

#if defined(COMPILER_MINGW) && !defined(COMPILER_CLANG)

#define MINGW_TLS_PATCH
// This is hopefully a temporary fix for abysmal MINGW thread_local implementation
// ALSO IMPORTANT: There are some mingw/lld issues that prevent TLS stuff to be used in inlines

int FastMingwTlsAlloc();

template <class T>
struct FastMingwTls {
	int ndx = -1;

	struct TEB_ {
	  PVOID Reserved1[12];
	  PVOID ProcessEnvironmentBlock;
	  PVOID Reserved2[399];
	  BYTE  Reserved3[1952];
	  PVOID TlsSlots[64];
	  BYTE  Reserved4[8];
	  PVOID Reserved5[26];
	  PVOID ReservedForOle;
	  PVOID Reserved6[4];
	  PVOID TlsExpansionSlots;
	};

	force_inline
	PVOID& Slot() const {
	#ifdef CPU_64
		TEB_ *teb = (TEB_ *)__readgsqword(0x30);
	#else
		TEB_ *teb = (TEB_ *)__readfsdword(0x18);
	#endif
		return teb->TlsSlots[ndx];
	}

public:
	void operator=(T x)                { Slot() = (PVOID)(uintptr_t)x; }
	T operator->()                     { return (T)(uintptr_t)Slot(); }
	const T operator->() const         { return (T)(uintptr_t)Slot(); }
	operator T() const                 { return (T)(uintptr_t)Slot(); }
	
	FastMingwTls()                     { ndx = FastMingwTlsAlloc(); }
	FastMingwTls(T x) : FastMingwTls() { operator=(x); }
};

#endif