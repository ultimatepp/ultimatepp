#ifndef _Glyph_glyph_h_
#define _Glyph_glyph_h_

#include <Draw/Draw.h>
#include <Painter/Painter.h>

using namespace Upp;


Image  RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz);

String CompressGlyph(const Image& m);
Image  DecompressGlyph(const String& g, Color c);

struct TestDraw : SDraw {
	Draw *draw;

	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
};


struct SImageDraw : SDraw {
	ImageBuffer b;

	virtual void  PutImage(Point p, const Image& m, const Rect& src);
	virtual void  PutRect(const Rect& r, Color color);
	
	Image PickResult()       { return b; }

	SImageDraw(Size sz);
	SImageDraw(int cx, int cy);
	~SImageDraw();
};


#endif
