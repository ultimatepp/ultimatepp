#ifndef _Glyph_glyph_h_
#define _Glyph_glyph_h_

#include <Draw/Draw.h>

using namespace Upp;

struct MiniRenderer {
	virtual void PutHorz(int x, int y, int cx) = 0;
	virtual void PutVert(int x, int y, int cy) = 0;
	
	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	
	void Line(Point p1, Point p2);

	struct Segment : Moveable<Segment> {
		int x;
		int cx;
		
		bool operator<(const Segment& b) const { return x < b.x; }
	};
	
	struct Segments;
	
	int         cy;
	int         diry;
	int         dirx;
	Segments   *pseg;
	Point       p0, p1;

	void Move(Point p);
	void Line(Point p);
	void Close();
	void Render();
	
	void Ellipse(Point center, Size radius);
	
	MiniRenderer(int cy);
	~MiniRenderer();
};

#endif
