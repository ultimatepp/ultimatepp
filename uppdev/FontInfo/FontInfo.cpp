#include "FontInfo.h"

void Std(Font& font)
{
	if(IsNull(font))
		font = StdFont();
	if(font.GetFace() == 0)
		font.Face(GetStdFont.GetFace());
	if(font.GetHeight() == 0)
		font.Height(GetStdFont.GetHeight());
}

struct CharEntry {
	GlyphInfo info;
	word      chr;
	Font      font;
};

CharEntry fc_cache_global[4093];

bool IsNormal(Font font, int chr)
{
	DrawLock __;
	CharEntry& e = fc_cache_global[CombineHash(font.GetHashValue(), chr) % 4093];
	if(e.font == font || e.chr == chr)
		return e.info.IsNormal();
	return GetGlyphInfoSys(font, chr).IsNormal();
}

CharEntry GetGlyphEntry(Font font, int chr, unsigned hash)
{
	DrawLock __;
	CharEntry& e = fc_cache_global[hash % 4093];
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
	Std(font);
	unsigned hash = CombineHash(font.GetHashValue(), chr);
	CharEntry& e = fc_cache[hash & 511];
	if(e.font != font || e.chr != chr)
		e = GetGlyphEntry(font, chr, hash);
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
	Std(font);
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
	Std(font);
	double sina;
	double cosa;
	int    d = 0;
	if(angle)
		Draw::SinCos(angle, sina, cosa); //TODO global sin tables!
	for(int i = 0; i < n; i++) {
		wchar chr = text[i];
		GlyphInfo gi = GetGlyphInfo(font, chr);
		if(gi.IsNormal())
			if(angle)
				w.DrawTextOp(int(x + cosa * d), int(y - sina * d), angle, &chr, font, ink, 1, NULL);
			else {
				int c = 1;
				int dd = 0;
				if(!dx)
					while(c < n) {
						GlyphInfo gi2 = GetGlyphInfo(font, text[i + c]);
						if(!gi2.IsNormal())
							break;
						c++;
						dd += gi.width;
						gi = gi2;
					}
				w.DrawTextOp(x + d, y, 0, text + i, font, ink, c, NULL);
				i += c - 1;
				d += dd;
			}
		else
		if(gi.IsReplaced()) {
			FontInfo fi = font.Info();
			Font fnt = font;
			fnt.Face(gi.lspc);
			FontInfo fi2 = fnt.Info();
			if(angle)
				w.DrawTextOp(int(x + cosa * d), int(y - sina * (fi.GetAscent() - fi2.GetAscent() + d)),
				             angle, &chr, fnt, ink, 1, NULL);
			else
				w.DrawTextOp(x + d, y + fi.GetAscent() - fi2.GetAscent(), 0, &chr, fnt, ink, 1, NULL);
			GlyphMetrics(gi, font, chr);
		}
		else
		if(gi.IsComposed()) {
			ComposedGlyph cg;
			Compose(font, chr, cg);
			if(angle) {
				w.DrawTextOp(int(x + cosa * d), int(y - sina * d), angle, &cg.basic_char, font, ink, 1, NULL);
				w.DrawTextOp(int(x + cosa * (d + cg.mark_pos.x)), int(y - sina * (cg.mark_pos.y + d)), angle, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			else {
				w.DrawTextOp(x + d, y, angle, &cg.basic_char, font, ink, 1, NULL);
				w.DrawTextOp(x + cg.mark_pos.x + d, y + cg.mark_pos.y, angle, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			GlyphMetrics(gi, font, chr);
		}
		if(dx)
			d += *dx++;
		else
			d += gi.width;
	}
}
