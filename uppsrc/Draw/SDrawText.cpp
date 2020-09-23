#include "Draw.h"

#define LTIMING(x)  // RTIMING(x)

namespace Upp {

Image RenderGlyphByPainter(Point at, int angle, int chr, Font fnt, Color color, Size sz);

Image SDraw::RenderGlyph(Point at, int angle, int chr, Font fnt, Color color, Size sz)
{
	ASSERT(HasPainter());
	return RenderGlyphByPainter(at, angle, chr, fnt, color, sz);
}

struct sMakeTextGlyph : public ImageMaker
{
	int    chr;
	Font   font;
	int    angle;
	Color  color;
	SDraw *draw;
	int    yy;

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		RawCat(h, yy);
		return String(h);
	}

	virtual Image Make() const {
		LTIMING("Render glyph");
		Point at(font[chr], font.GetLineHeight());
		if(IsNull(yy)) {
			int n = 2 * (at.x + at.y);
			at.x = max(at.x, at.y);
			at.y = max(at.x, at.y);
			return AutoCrop(WithHotSpot(draw->RenderGlyph(at, angle, chr, font, color, Size(n, n)), at.x, at.y), RGBAZero());
		}
		else {
			int n = at.x + at.y;
			Size bandsz(2 * n, 32);
			return AutoCrop(WithHotSpot(draw->RenderGlyph(Point(0, -yy), angle, chr, font, color, bandsz), 0, 0), RGBAZero());
		}
	}
};

void SDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	sMakeTextGlyph g;
	g.font = font;
	g.color = White();
	g.angle = angle;
	g.draw = this;
	for(int i = 0; i < n; i++) {
		g.chr = text[i];
		LTIMING("Paint glyph");
		if(font.GetHeight() > 200) {
			int bn = font[g.chr] + font.GetLineHeight();
			for(g.yy = 0; g.yy < bn; g.yy += 32) {
				Image m;
				if(paintonly)
					m = MakeImagePaintOnly(g);
				else
					m = MakeImage(g);
				Point h = m.GetHotSpot();
				SysDrawImageOp(x - h.x, y + g.yy - h.y, m, m.GetSize(), ink);
			}
		}
		else {
			g.yy = Null;
			Image m;
			if(paintonly)
				m = MakeImagePaintOnly(g);
			else
				m = MakeImage(g);
			Point h = m.GetHotSpot();
			SysDrawImageOp(x - h.x, y - h.y, m, m.GetSize(), ink);
		}
		x += dx ? *dx++ : font[g.chr];
	}
}

}
