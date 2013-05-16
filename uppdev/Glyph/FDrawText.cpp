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
		if(font.GetHeight() > 200) {
			Size sz(font[g.chr], font.GetLineHeight());
			int w = 2 * (sz.cx + sz.cy);
			Size bandsz(w, 32);
			for(int yy = 0; yy < w; yy += bandsz.cy) {
				Image m = RenderGlyph(font, g.chr, ink, angle, bandsz, Point(0, -yy));
				if(yy & 1)
					SysDrawImageOp(x - (sz.cx + sz.cy), y + yy - (sz.cx + sz.cy), m, m.GetSize(), Null);
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
