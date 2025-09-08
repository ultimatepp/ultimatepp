#include "Draw.h"

namespace Upp {

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
		return WString(b);
	}
#endif
	if(cs == CHARSET_UTF8)
		return ToUtf32(s, n);
	return ToUnicode(s, n, cs);
}

void Draw::DrawText(int x, int y, int angle, const wchar *text, Font font,
		            Color ink, int n, const int *dx)
{
	if(IsNull(ink)) return;
	ink = ResolveInk(ink);
	if(n < 0)
		n = strlen__(text);
	Std(font);
	double sina = 0;
	double cosa = 1;
	int    posx = 0;
	if(angle)
		Draw::SinCos(angle, sina, cosa);
	Font font0 = font;
	if(GetInfo() & DRAWTEXTLINES)
		font.Underline(false).Strikeout(false);
	for(int i = 0; i < n; i++) {
		wchar chr = text[i];
		GlyphInfo gi = GetGlyphInfo(font, chr);
		if(gi.IsNormal())
			if(angle || chr >= 0x10000)
				DrawTextOp(int(x + cosa * posx), int(y - sina * posx), angle, &chr, font, ink, 1, NULL);
			else {
				int c = 1;
				int dd = 0;
				while(i + c < n && c < 1000) {
					GlyphInfo gi2 = GetGlyphInfo(font, text[i + c]);
					if(!gi2.IsNormal())
						break;
					dd += dx ? dx[c - 1] : gi.width;
					c++;
					gi = gi2;
				}
				DrawTextOp(x + posx, y, 0, text + i, font, ink, c, dx);
				posx += dd;
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
				DrawTextOp(int(x + cosa * posx), int(y - sina * (font.GetAscent() - fnt.GetAscent() + posx)),
				             angle, &chr, fnt, ink, 1, NULL);
			else
				DrawTextOp(x + posx, y + font.GetAscent() - fnt.GetAscent(), 0, &chr, fnt, ink, 1, NULL);
			GlyphMetrics(gi, font, chr);
		}
		else
		if(gi.IsComposed()) {
			ComposedGlyph cg;
			Compose(font, chr, cg);
			if(angle) {
				DrawTextOp(int(x + cosa * posx), int(y - sina * posx), angle, &cg.basic_char, font, ink, 1, NULL);
				DrawTextOp(int(x + cosa * (posx + cg.mark_pos.x)), int(y - sina * (cg.mark_pos.y + posx)), angle, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			else {
				DrawTextOp(x + posx, y, 0, &cg.basic_char, font, ink, 1, NULL);
				DrawTextOp(x + cg.mark_pos.x + posx, y + cg.mark_pos.y, 0, &cg.mark_char, cg.mark_font, ink, 1, NULL);
			}
			GlyphMetrics(gi, font, chr);
		}
		else {
			Font fnt = Arial(font.GetHeight());
			wchar chr = 0x25a1;
			gi = GetGlyphInfo(fnt, chr);
			if(!gi.IsNormal()) {
				chr = ' ';
				gi = GetGlyphInfo(fnt, chr);
			}
			if(angle)
				DrawTextOp(int(x + cosa * posx), int(y - sina * posx), angle, &chr, fnt, ink, 1, NULL);
			else
				DrawTextOp(x + posx, y, 0, &chr, fnt, ink, 1, NULL);
		}
		posx += dx ? *dx++ : gi.width;
	}

	if((GetInfo() & DRAWTEXTLINES) && (font0.IsUnderline() || font0.IsStrikeout())) {
		int hg = abs(font0.GetCy());
		if(hg == 0) hg = 10;
		int thickness = max(font.IsBold() ? hg / 10 : hg / 20, 1);

		int ascent = font0.GetAscent();
		Size offset = Point(0, ascent);
		if(angle) {
			offset.cx = fround(ascent * sina);
			offset.cy = fround(ascent * cosa);
		}

		x += offset.cx;
		y += offset.cy;
		int p;
		auto DoLine = [&] {
			if(angle == 0)
				DrawRect(x, y + p, posx, thickness, ink);
			else
				DrawLine(
					int(x + p * sina),
					int(y + p * cosa),
					int(x + posx * cosa + p * sina),
					int(y - posx * sina + p * cosa),
					thickness,
					ink
				);
		};
		if(font0.IsUnderline()) {
			int descent = font0.GetDescent();
			p = min(descent - thickness, max(descent > 0 ? descent / 2 : hg / 15, int(descent > 0)));
			DoLine();
		}
		if(font0.IsStrikeout()) {
			p = -ascent / 3;
			DoLine();
		}
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
	if(n < 0)
		n = strlen__(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		if(*wtext >= ' ')
			sz.cx += font[*wtext];
		n--;
		wtext++;
	}
	sz.cy = font.GetCy();
	return sz;
}

Size GetTextSize(const WString& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

Size GetTextSize(const char *text, byte charset, Font font, int n)
{
		return GetTextSize(TextUnicode(text, n, charset, font), font);

	if(ResolveCharset(charset) == CHARSET_UTF8
#ifdef PLATFORM_WIN32
	   && font.GetFace() != Font::SYMBOL
#endif
	) {
		Size sz;
		sz.cx = 0;
		const char *lim = text + (n < 0 ? strlen(text) : n);
		while(text < lim) {
			int c = FetchUtf8(text, lim);
			if(c >= ' ')
				sz.cx += font[c];
			n--;
		}
		sz.cy = font.GetCy();
		return sz;
	}
	else
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

}
