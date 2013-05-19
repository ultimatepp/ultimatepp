#include "Draw.h"

#define LTIMING(x)  RTIMING(x)

NAMESPACE_UPP

struct sMakeTextGlyph : public ImageMaker
{
	int    chr;
	Font   font;
	int    angle;
	Color  color;
	SDraw *draw;

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		return h;
	}

	virtual Image Make() const {
		LTIMING("Render glyph");
		Point at(font[chr], font.GetLineHeight());
		int n = 2 * (at.x + at.y);
		return AutoCrop(WithHotSpot(draw->RenderGlyph(at, angle, chr, font, color, Size(n, n)), at.x, at.y), RGBAZero());
	}
};

void SDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	sMakeTextGlyph g;
	g.font = font;
	g.color = ink;
	g.angle = angle;
	g.draw = this;
	for(int i = 0; i < n; i++) {
		g.chr = text[i];
		LTIMING("Paint glyph");
		if(font.GetHeight() > 200) {
			Point at(font[g.chr], font.GetLineHeight());
			int n = at.x + at.y;
			Size bandsz(2 * n, 32);
			for(int yy = 0; yy < n; yy += bandsz.cy) {
				Image m = RenderGlyph(Point(0, -yy), angle, g.chr, font, ink, bandsz);
				SysDrawImageOp(x, y + yy, m, m.GetSize(), Null);
			}
		}
		else {
			Image m = MakeImage(g);
			Point h = m.GetHotSpot();
			SysDrawImageOp(x - h.x, y - h.y, m, m.GetSize(), Null);
		}
		x += dx ? *dx++ : font[g.chr];
	}
}

END_UPP_NAMESPACE
