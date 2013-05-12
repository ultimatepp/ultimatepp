#ifndef _Glyph_glyph_h_
#define _Glyph_glyph_h_

#include <Draw/Draw.h>
#include <Painter/Painter.h>

using namespace Upp;

class MiniRenderer {
	struct Segment : Moveable<Segment> {
		int l;
		int h;
		
		bool operator<(const Segment& b) const { return l < b.l; }
	};
	
	struct Segments;
	
	int           cy;
	int           diry;
	int           dirx;
	One<Segments> pseg;
	Point         p0, p1;
	int           width;

	void AHorz(int x, int y, int cx);
	void AVert(int x, int y, int cy);
	void DoLine(Point p1, Point p2);

public:
	void FatLine(Point p1, Point p2, int n);

	virtual void PutHorz(int x, int y, int cx) = 0;
	virtual void PutVert(int x, int y, int cy) = 0;
	
	int  GetDirx() const                      { return dirx; }
	int  GetDiry() const                      { return diry; }
	
	MiniRenderer& Move(Point p);
	MiniRenderer& Line(Point p);
	MiniRenderer& Close();
	
	MiniRenderer& Polygon();
	MiniRenderer& Fill();
	
	MiniRenderer& Ellipse(Point center, Size radius);
	
	MiniRenderer& Width(int width_)           { width = width_; return *this; }

	void Cy(int cy_)                          { cy = cy_; }
	
	MiniRenderer();
	~MiniRenderer();
};


Image  AutoCrop(const Image& m, RGBA c);
Image  RenderGlyph(Font fnt, int chr);
String CompressGlyph(const Image& m);
Image  DecompressGlyph(const String& g, Color c);

#endif
