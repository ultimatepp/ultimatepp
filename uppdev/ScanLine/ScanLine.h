#ifndef _ScanLine_ScanLine_h_
#define _ScanLine_ScanLine_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define PAINTER_TIMING(x)   // RTIMING(x)

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
	AlphaBlend(t, Mul7(c, cover));
}

struct ScanLine {
	int x, len;
	String data;
	
	String ToString() const;
};

void     Apply(RGBA *t, int len, const RGBA& color, const ScanLine& s);
ScanLine And(const ScanLine& a, const ScanLine& b);
ScanLine Pack(int x, const byte *data, int len);

#define YLINK

class Rasterizer {
	struct Cell : Moveable<Cell> {
		int16 x;
		int16 cover;
		int   area;

		bool operator<(const Cell& b) const { return x < b.x; }
    };

#ifdef YLINK
	struct CellY : Cell, Moveable<CellY> {
		int   ylink;
	};
#endif

	Rectf                   cliprect;
	double                  x1, y1;
#ifdef YLINK
	Buffer<int>             ylink;
	Vector<CellY>           cell;
	Vector<Cell>            line;
#else
	Buffer< Vector<Cell> >  cell;
#endif
	int                     current_y;
	Cell                    current;
	int                     xmax, ymax;
	int                     min_y;
	int                     max_y;
	Size                    sz;
	bool                    finish;
	
	void AddCurrent();
	void SetCurrent(int x, int y);
	void RenderHLine(int ey, int x1, int y1, int x2, int y2);
	void LineClip(double x1, double y1, double x2, double y2);
	int  CvX(double x);
	int  CvY(double y);
	void CvLine(double x1, double y1, double x2, double y2);
	bool BeginRender(int y, const Cell *&c, const Cell *&e);

public:
	void LineRaw(int x1, int y1, int x2, int y2);
	
	void Move(double x, double y);
	void Line(double x, double y);

	int  MinY() const                         { return min_y; }
	int  MaxY() const                         { return max_y; }
	
	void SetClip(const Rectf& rect);

	template <class Target>
	void Render(int y, Target& g);
	
	void Reset();
	
	Rasterizer(int cx, int cy);
};

#include "Rasterizer.hpp"

void     Render(ImageBuffer& ib, Rasterizer& r, const RGBA& color);

#endif
