#ifndef _Glyph_glyph_h_
#define _Glyph_glyph_h_

#include <Draw/Draw.h>

using namespace Upp;

struct MiniRenderer {
	virtual void PutHorz(int x, int y, int cx) = 0;
	virtual void PutVert(int x, int y, int cy) = 0;
	
	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	
	void DrawLine(Point p2);
	
	struct Segments;
	
	Segments *pseg;
	Point     p0, p1;
	int       cy;
	int       diry;

	void Move(Point p)          { p0 = p1 = p; }
	void Line(Point p);
	void Close();
	void Render();
	
	void Ellipse(Point center, Size radius);
	
	MiniRenderer(int cy);
	~MiniRenderer();
};

#endif
