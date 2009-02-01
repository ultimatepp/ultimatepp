#ifndef _ScanLine_ScanLine_h_
#define _ScanLine_ScanLine_h_

#include <CtrlLib/CtrlLib.h>

#define RASTERIZER2

using namespace Upp;

#define PAINTER_TIMING(x)  //  RTIMING(x)

inline RGBA Mul8(const RGBA& s, int mul)
{
	RGBA t;
	t.r = (mul * s.r) >> 8;
	t.g = (mul * s.g) >> 8;
	t.b = (mul * s.b) >> 8;
	t.a = (mul * s.a) >> 8;
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

inline void AlphaBlendCover8(RGBA& t, const RGBA& c, int cover)
{
	int a = c.a * cover >> 8;
	int alpha = 256 - (a + (a >> 7));
	t.r = (c.r * cover >> 8) + (alpha * t.r >> 8);
	t.g = (c.g * cover >> 8) + (alpha * t.g >> 8);
	t.b = (c.b * cover >> 8) + (alpha * t.b >> 8);
	t.a = a + (alpha * t.a >> 8);
}

struct ScanLine {
	int          xmin, xmax;
	Buffer<byte> data;
	int          datalen;

	bool IsFull();
	bool IsEmpty();

	String ToString() const;
};

#ifdef RASTERIZER2

#include "Rasterizer2.h"

#elif defined(RASTERIZER3)

#include "Rasterizer3.h"

#else

#include "Rasterizer.h"

#endif

void     Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color);

struct VertexTarget {
	virtual void Move(Pointf p) = 0;
	virtual void Line(Pointf p) = 0;
	virtual void End() = 0;
};

struct VertexProcessor : VertexTarget {
	VertexTarget *target;
};

class Stroker : VertexProcessor {
public:
	virtual void Move(double x, double y);
	virtual void Line(double x, double y);
	virtual void End();
};

class Dasher : VertexProcessor {
public:
	virtual void Move(double x, double y);
	virtual void Line(double x, double y);
	virtual void End();
};

class Transformer : VertexProcessor {
public:
	virtual void Move(double x, double y);
	virtual void Line(double x, double y);
	virtual void End();
};

struct LineSegment {
	byte   kind;
	Pointf x;
};

struct QuadraticSegment {
	byte   kind;
	Pointf x;
	Pointf x1;
};

struct CubicSegment {
	byte   kind;
	Pointf x;
	Pointf x1;
	Pointf x2;
};

void Quadratic(VertexTarget& t, Pointf x0, Pointf x1, Pointf x, double epsilon);
void Cubic(VertexTarget& t, Pointf x0, Pointf x1, Pointf x2, Pointf x, double epsilon);

class Painter {
	Vector<byte> path;
	
	template <class T>
	T& PathAdd() {
		int q = path.GetCount();
		path.SetCount(q + sizeof(T));
		return *(T *)&path[q];
	}

	void Move(Pointf t) {
	}
};


#endif
