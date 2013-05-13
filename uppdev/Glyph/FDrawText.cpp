#include "glyph.h"

struct sMakeGlyph : public ImageMaker
{
	int    chr;
	Font   font;
	int    angle;
	Color  color;

	virtual String Key() const {
		StringBuffer h;
		RawCat(h, chr);
		RawCat(h, font);
		RawCat(h, angle);
		RawCat(h, color);
		return h;
	}

	virtual Image Make() const {
		RTIMING("Render glyph");
		return RenderGlyph(font, chr, color, angle);
	}
};

void FDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink, int n, const int *dx)
{
	sMakeGlyph g;
	g.font = font;
	g.color = ink;
	g.angle = angle; // TODO!
	for(int i = 0; i < n; i++) {
		g.chr = text[i];
		RTIMING("Paint glyph");
		Image m = MakeImage(g);
		Point h = m.GetHotSpot();
		SysDrawImageOp(x - h.x, y - h.y, m, m.GetSize(), Null);
		x += dx ? *dx++ : font[g.chr];
	}
}
