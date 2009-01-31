#ifndef _ScanLine_ScanLine_h_
#define _ScanLine_ScanLine_h_

#include <CtrlLib/CtrlLib.h>

#define RASTERIZER2

using namespace Upp;

#define PAINTER_TIMING(x)  //  RTIMING(x)

inline RGBA Mul7(const RGBA& s, int mul)
{
	RGBA t;
	t.r = (mul * s.r) >> 7;
	t.g = (mul * s.g) >> 7;
	t.b = (mul * s.b) >> 7;
	t.a = (mul * s.a) >> 7;
	return t;
}

inline void AlphaBlend(RGBA& t, const RGBA& c)
{
	int alpha = 256 - (c.a + (c.a >> 7));
	t.r = c.r + (alpha * t.r >> 8);
	t.g = c.g + (alpha * t.g >> 8);
	t.b = c.b + (alpha * t.b >> 8);
	t.a = c.a + (alpha * t.a >> 8);
}

inline void AlphaBlendCover7(RGBA& t, const RGBA& c, byte cover)
{
#if 0
	AlphaBlend(t, Mul7(c, cover));
#else
	int a = c.a * cover >> 7;
	int alpha = 256 - (a + (a >> 7));
	t.r = (c.r * cover >> 7) + (alpha * t.r >> 8);
	t.g = (c.g * cover >> 7) + (alpha * t.g >> 8);
	t.b = (c.b * cover >> 7) + (alpha * t.b >> 8);
	t.a = a + (alpha * t.a >> 8);
#endif
}

struct ScanLine {
	int x, len;
	String data;
	
	String ToString() const;
};

void     Apply(RGBA *t, int len, const RGBA& color, const ScanLine& s);
ScanLine And(const ScanLine& a, const ScanLine& b);
ScanLine Pack(int x, const byte *data, int len);

#ifdef RASTERIZER2

#include "Rasterizer2.h"

#else

#include "Rasterizer.h"

#endif

void     Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color);

#endif
