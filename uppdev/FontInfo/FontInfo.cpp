#include "FontInfo.h"

struct CharEntry {
	GlyphInfo info;
	word      chr;
	Font      font;
};

CharEntry fc_cache_global[4096];

CharEntry GetGlyphEntry(Font font, int chr)
{
	DrawLock __;
	unsigned hash = CombineHash(font.GetHashValue(), chr) & 511;
	CharEntry& e = fc_cache_global[hash];
	if(e.font != font || e.chr != chr) {
		e.font = font;
		e.chr = chr;
		e.info = GetGlyphInfoSys(font, chr);
		if(!e.info.IsNormal()) {
			ComposedGlyph cg;
			Font rfnt;
			if(Compose(font, chr, cg)) {
				e.info.lspc = -1;
				e.info.rspc = cg.basic_char;
			}
			else
			if(Replace(font, chr, rfnt))
				e.info.lspc = rfnt.GetFace();
			else
				e.info.lspc = -2;
		}
	}
	return e;
}

thread__ CharEntry fc_cache[512];

GlyphInfo GetGlyphInfo(Font font, int chr)
{
	unsigned hash = CombineHash(font.GetHashValue(), chr) & 511;
	CharEntry& e = fc_cache[hash];
	if(e.font != font || e.chr != chr)
		e = GetGlyphEntry(font, chr);
	return e.info;
}

void GlyphMetrics(GlyphInfo& f, Font font, int chr)
{
	if(f.IsReplaced())
		f = GetGlyphInfo(font().Face(f.lspc), chr);
	if(f.IsComposed()) {
		f = GetGlyphInfo(font, f.rspc);
		if(f.IsComposedLM())
			f.rspc += f.width / 2;
	}
}

GlyphInfo GetGlyphMetrics(Font font, int chr)
{
	GlyphInfo f = GetGlyphInfo(font, chr);
	if(f.IsMissing())
		f = GetGlyphInfo(font, '?');
	GlyphMetrics(f, font, chr);
	return f;
}

struct FontEntry {
	CommonFontInfo info;
	Font           font;
};

thread__ FontEntry fi_cache[64];

const CommonFontInfo& GetFontInfo(Font font)
{
	unsigned hash = font.GetHashValue() & 63;
	FontEntry& e = fi_cache[hash];
	if(e.font != font) {
		DrawLock __;
		e.font = font;
		e.info = GetFontInfoSys(font);
	}
	return e.info;
}

void DoDrawText(Draw& w, int x, int y, int angle, const wchar *text, Font font, Color ink,
                int n, const int *dx)
{
	for(int i = 0; i < n; i++) {
		wchar chr = text[i];
		GlyphInfo gi = GetGlyphInfo(font, chr);
		if(gi.IsNormal())
			w.DrawTextOp(x, y, angle, &chr, font, ink, 1, NULL);
		else
		if(gi.IsReplaced()) {
			FontInfo fi = font.Info();
			Font fnt = font;
			fnt.Face(gi.lspc);
			FontInfo fi2 = fnt.Info();
			w.DrawTextOp(x, y + fi.GetAscent() - fi2.GetAscent(), angle, &chr, fnt, ink, 1, NULL);
			GlyphMetrics(gi, font, chr);
		}
		else
		if(gi.IsComposed()) {
			ComposedGlyph cg;
			Compose(font, chr, cg);
			w.DrawTextOp(x, y, angle, &cg.basic_char, font, ink, 1, NULL);
			w.DrawTextOp(x + cg.mark_pos.x, y + cg.mark_pos.y, angle, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			GlyphMetrics(gi, font, chr);
		}
		if(dx)
			x += *dx++;
		else
			x += gi.width;
	}
}
