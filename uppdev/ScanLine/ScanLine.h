#ifndef _ScanLine_ScanLine_h_
#define _ScanLine_ScanLine_h_

#include <CtrlLib/CtrlLib.h>

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

struct LinearPathConsumer {
	virtual void Move(const Pointf& p) = 0;
	virtual void Line(const Pointf& p) = 0;
	virtual void End();
};

struct LinearPathFilter : LinearPathConsumer {
	virtual void End();

	LinearPathConsumer *target;

	void PutMove(const Pointf& p)               { target->Move(p); }
	void PutLine(const Pointf& p)               { target->Line(p); }
	void PutEnd()                               { target->End(); }
	
	template <class T> T& operator|(T& b)       { target = &b; return b; }
};

enum {
	LINECAP_BUTT,
	LINECAP_SQUARE,
	LINECAP_ROUND,

	LINEJOIN_MITER,
	LINEJOIN_ROUND,
	LINEJOIN_BEVEL,
};

class Stroker : public LinearPathFilter {
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

struct Dasher : public LinearPathFilter {
	Vector<double> pattern;
	int            patterni;
	double         rem;
	bool           flag;
	Pointf         p0;

	void    Put(const Pointf& p);

public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);
	
	Dasher(double width, const Vector<double>& pattern, double distance);
};

void ApproximateQuadratic(LinearPathConsumer& t, const Pointf& p1, const Pointf& p2, const Pointf& p3, double tolerance);
void ApproximateCubic(LinearPathConsumer& t, const Pointf& x0, const Pointf& x1, const Pointf& x2, const Pointf& x, double tolerance);

class Rasterizer : public LinearPathConsumer {
public:
	virtual void Move(const Pointf& p);
	virtual void Line(const Pointf& p);

private:
	struct Cell : MoveableWithSwap<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };

	Rectf                   cliprect;
	Pointf                  p0;
	Buffer< Vector<Cell> >  cell;
	int                     xmax, ymax;
	int                     min_y;
	int                     max_y;
	Size                    sz;

	void  Init();
	Cell *AddCells(int y, int n);
	void  RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void  LineClip(double x1, double y1, double x2, double y2);
	int   CvX(double x);
	int   CvY(double y);
	void  CvLine(double x1, double y1, double x2, double y2);
	bool  BeginRender(int y, const Cell *&c, const Cell *&e);

public:
	struct Target {
		virtual void Start(int x, int len) = 0;
		virtual void Render(int val) = 0;
		virtual void Render(int val, int len) = 0;
	};

	void LineRaw(int x1, int y1, int x2, int y2);
	
	void SetClip(const Rectf& rect);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	void Render(int y, Target& g, bool evenodd);

	void Reset();
	
	Rasterizer(int cx, int cy);
};

void Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color, bool evenodd);

struct Xform2D {
	Pointf x, y, t;
	
	Pointf GetScaleXY() const;
	double GetScale() const;
	bool   IsRegular() const;
	Pointf Transform(const Pointf& f) const;
	
	static Xform2D Identity();
	static Xform2D Translation(double x, double y);
	static Xform2D Scale(double sx, double sy);
	static Xform2D Scale(double scale);
	static Xform2D Rotation(double fi);
	static Xform2D Sheer(double fi);	
};

Xform2D operator*(const Xform2D& a, const Xform2D& b);
Xform2D Inverse(const Xform2D& m);

class BufferPainter {
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
