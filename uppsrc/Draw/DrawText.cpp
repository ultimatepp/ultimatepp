#include "Draw.h"

NAMESPACE_UPP


#define LLOG(x)    // LOG(x)
#define LTIMING(x) // TIMING(x)

WString TextUnicode(const char *s, int n, byte cs, Font font)
{
	if(n < 0)
		n = (int)strlen(s);
#ifdef PLATFORM_WIN32
	if(font.GetFace() == Font::SYMBOL) {
		WStringBuffer b(n);
		wchar *t = b;
		while(n > 0) {
			*t++ = *s++;
			n--;
		}
		return b;
	}
#endif
	return ToUnicode(s, n, cs);
}

void Draw::DrawText(int x, int y, int angle, const wchar *text, Font font,
		            Color ink, int n, const int *dx)
{
	if(IsNull(ink)) return;
	if(n < 0)
		n = wstrlen(text);
	Std(font);
	double sina;
	double cosa;
	int    d = 0;
	if(angle)
		Draw::SinCos(angle, sina, cosa);
	for(int i = 0; i < n; i++) {
		wchar chr = text[i];
		GlyphInfo gi = GetGlyphInfo(font, chr);
		if(gi.IsNormal())
			if(angle)
				DrawTextOp(int(x + cosa * d), int(y - sina * d), angle, &chr, font, ink, 1, NULL);
			else {
				int c = 1;
				int dd = 0;
				while(i + c < n) {
					GlyphInfo gi2 = GetGlyphInfo(font, text[i + c]);
					if(!gi2.IsNormal())
						break;
					dd += dx ? dx[c] : gi.width;
					c++;
					gi = gi2;
				}
				DrawTextOp(x + d, y, 0, text + i, font, ink, c, dx);
				d += dd;
				i += c - 1;
				if(dx)
					dx += c - 1;
			}
		else
		if(gi.IsReplaced()) {
			Font fnt = font;
			fnt.Face(gi.lspc);
			fnt.Height(gi.rspc);
			if(angle)
				DrawTextOp(int(x + cosa * d), int(y - sina * (font.GetAscent() - fnt.GetAscent() + d)),
				             angle, &chr, fnt, ink, 1, NULL);
			else
				DrawTextOp(x + d, y + font.GetAscent() - fnt.GetAscent(), 0, &chr, fnt, ink, 1, NULL);
			GlyphMetrics(gi, font, chr);
		}
		else
		if(gi.IsComposed()) {
			ComposedGlyph cg;
			Compose(font, chr, cg);
			if(angle) {
				DrawTextOp(int(x + cosa * d), int(y - sina * d), angle, &cg.basic_char, font, ink, 1, NULL);
				DrawTextOp(int(x + cosa * (d + cg.mark_pos.x)), int(y - sina * (cg.mark_pos.y + d)), angle, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			else {
				DrawTextOp(x + d, y, 0, &cg.basic_char, font, ink, 1, NULL);
				DrawTextOp(x + cg.mark_pos.x + d, y + cg.mark_pos.y, 0, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			GlyphMetrics(gi, font, chr);
		}
		d += dx ? *dx++ : gi.width;
	}
}

// ----------------------------

void Draw::DrawText(int x, int y, const wchar *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const WString& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, ~text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const WString& text, Font font, Color ink, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, TextUnicode(text, n, charset, font), font, ink, dx);
}

void Draw::DrawText(int x, int y, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, charset, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const char *text,
                    Font font, Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, text, CHARSET_DEFAULT, font, ink, n, dx);
}

void Draw::DrawText(int x, int y, const char *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, text, CHARSET_DEFAULT, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const String& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const String& text, Font font, Color ink, const int *dx)
{
	WString h = TextUnicode(text, text.GetLength(), CHARSET_DEFAULT, font);
	DrawText(x, y, h, font, ink, h.GetLength(), dx);
}

// --------------------------

Size GetTextSize(const wchar *text, Font font, int n)
{
	FontInfo fi = font.Info();
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		sz.cx += fi[*wtext++];
		n--;
	}
	sz.cy = fi.GetHeight();
	return sz;
}

Size GetTextSize(const WString& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

Size GetTextSize(const char *text, byte charset, Font font, int n)
{
	return GetTextSize(TextUnicode(text, n, charset, font), font);
}

Size GetTextSize(const char *text, Font font, int n)
{
	return GetTextSize(text, CHARSET_DEFAULT, font, n);
}

Size GetTextSize(const String& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

END_UPP_NAMESPACE
