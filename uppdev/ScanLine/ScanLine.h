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

void Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color, bool evenodd);

double SquareDist(const Pointf& p1, const Pointf& p2);
Pointf Mid(const Pointf& a, const Pointf& b);
Pointf Ortogonal(const Pointf& p);
double SquareLength(const Pointf& p);
double Length(const Pointf& p);
double Bearing(const Pointf& p);
double Distance(const Pointf& p1, const Pointf& p2);
double SquareDistance(const Pointf& p1, const Pointf& p2);
Pointf PolarPointf(double a);
Pointf Polar(const Pointf& p, double r, double a);

struct VertexTarget {
	virtual void Move(const Pointf& p) = 0;
	virtual void Line(const Pointf& p) = 0;
	virtual void End() = 0;
};

struct VertexProcessor : VertexTarget {
	VertexTarget *target;
	
	void PutMove(const Pointf& p) { target->Move(p); }
	void PutLine(const Pointf& p) { target->Line(p); }
	void PutEnd()                 { target->End(); }
	
	VertexProcessor& operator|(VertexProcessor& b) { target = &b; return b; }
	void             operator|(VertexTarget& b)    { target = &b; }
};

enum {
	LINECAP_BUTT,
	LINECAP_SQUARE,
	LINECAP_ROUND,

	LINEJOIN_MITER,
	LINEJOIN_ROUND,
	LINEJOIN_BEVEL,
};

class Stroker : public VertexProcessor {
	double w2;
	double qmiter;
	double fid;

	Pointf p0, v0, o0, a0, b0;
	Pointf p1, v1, o1, a1, b1;
	Pointf p2;
	int    linecap;
	int    linejoin;
	
	void   Finish();
	void   Round(const Pointf& p, const Pointf& v1, const Pointf& v2, double r);
	void   Cap(const Pointf& p0, const Pointf& v0, const Pointf& o0,
	           const Pointf& a0, const Pointf& b0);

public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);
	virtual void End();
	
	Stroker(double width, double miterlimit, double tolerance, int linecap, int linejoin);
};

class Transformer : VertexProcessor {
public:
	virtual void Move(double x, double y);
	virtual void Line(double x, double y);
	virtual void End();
};

void ApproximateQuadratic(VertexTarget& t, const Pointf& p1, const Pointf& p2, const Pointf& p3, double tolerance);
void ApproximateCubic(VertexTarget& t, const Pointf& x0, const Pointf& x1, const Pointf& x2, const Pointf& x, double tolerance);

#define Painter NewPainter

class Painter {
	enum {
		MOVE, LINE, QUADRATIC, CUBIC
	};
	struct Segment {
		byte   kind;
		Pointf p;
	};	
	struct QuadraticSegment : Segment{
		Pointf p1;
	};	
	struct CubicSegment : QuadraticSegment {
		Pointf p2;
	};
	
	Vector<byte> path;
	
	template <class T>
	T& PathAdd() {
		int q = path.GetCount();
		path.SetCount(q + sizeof(T));
		return *(T *)&path[q];
	}
	
public:
	void Move(Pointf p);
	void Line(Pointf p);
	void Quadratic(Pointf p1, Pointf p);
	void Cubic(Pointf p1, Pointf p2, Pointf p);
};

#endif
