#ifndef _GridCtrl_GridUtils_h_
#define _GridCtrl_GridUtils_h_

#define BIT(x) (1 << x)

inline bool IsSet(int s, int b)
{
	return s & b;
}

inline void BitSet(dword &k, dword v, bool s)
{
	if(s) k |= v; else  k &= ~v;
}

inline bool BitInverse(dword &k, dword v)
{
	bool s = k & v;
	BitSet(k, v, !s);
	return s;
}

inline int Distance(const Point &p0, const Point &p1)
{
	return max(abs(p0.x - p1.x), abs(p0.y - p1.y));
}

inline int32 Round(double a)
{
#if defined(COMPILER_MSC) && defined(CPU_32)
	int32 retval;
	__asm fld a
	__asm fistp retval
	return retval;
#else
	return (int32) a;
#endif
}

#ifdef flagDEBUG
#define LG LogGui
#else
inline void LG_Dummy_Nop__(int level, const char *fmt, ...) {} // needed to silence CLANG warnings
inline void LG_Dummy_Nop__(const char *fmt, ...) {} // about unused value
#define LG LG_Dummy_Nop__
#endif
#define LGR LogGui

extern LineEdit *dlog;
extern int       dlev;

void LogGui(const char *fmt, ...);
void LogGui(int level, const char *fmt, ...);

void LogCon(const char *fmt, ...);
void LogCon(int level, const char *fmt, ...);

#endif
