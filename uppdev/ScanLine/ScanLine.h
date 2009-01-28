#ifndef _ScanLine_ScanLine_h_
#define _ScanLine_ScanLine_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

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

class Rasterizer {
	struct Cell : Moveable<Cell> {
		int x;
		int cover;
		int area;

		void Init()                         { x = 0x7FFFFFFF; cover = area  = 0; }
		bool operator<(const Cell& b) const { return x < b.x; }
    };

	struct SortedY {
		unsigned start;
		unsigned num;
	};
	
	Buffer< Vector<Cell> >  cell;
	int                     current_y;
	Cell                    current;
	int                     min_y;
	int                     max_y;
	bool                    finish;
	
	void AddCurrent();
	void SetCurrent(int x, int y);
	void RenderHLine(int ey, int x1, int y1, int x2, int y2);

public:
	void Line(int x1, int y1, int x2, int y2);

	int  MinY() const { return min_y; }
	int  MaxY() const { return max_y; }

	ScanLine Get(int y);
	
	Rasterizer(int cy);
};

#endif
