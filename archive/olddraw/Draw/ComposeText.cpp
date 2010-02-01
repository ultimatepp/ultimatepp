#include "Draw.h"

NAMESPACE_UPP

enum {
	CG_NONE,
	CG_CAPITAL,
	CG_SMALL
};

enum {
	CG_GRAVE = 0x60,
	CG_ACUTE = 0xb4,
	CG_CEDILLA = 0xb8,
	CG_MACRON = 175,
	CG_CIRCUMFLEX = 0x2c6,
	CG_TILDE = 0x2dc,
	CG_DOT_ABOVE = 0x2d9,
	CG_OGONEK = 0x2db,
	CG_STROKE = '-',
	CG_BREVE = 0x2d8,
	CG_CARON = 0x2c7,
	CG_MIDDLE_DOT = 0xb7,
	CG_DOUBLE_ACUTE = 0x2dd,
	CG_DIAERESIS = 0xa8,
	CG_RING_ABOVE = 0x2da,
	CG_COMMA_T = ',',
	CG_COMMA_UR = 1,
	CG_COMMA_URI,
};

struct CGInfo {
	byte  type;
	char  ascii;
	wchar mark;
}
gc_info[128] = {
	{ CG_CAPITAL, 'A', CG_MACRON },
	{ CG_SMALL, 'a', CG_MACRON },
	{ CG_CAPITAL, 'A', CG_BREVE },
	{ CG_SMALL, 'a', CG_BREVE },
	{ CG_CAPITAL, 'A', CG_OGONEK },
	{ CG_SMALL, 'a', CG_OGONEK },
	{ CG_CAPITAL, 'C', CG_ACUTE },
	{ CG_SMALL, 'c', CG_ACUTE },
	{ CG_CAPITAL, 'C', CG_CIRCUMFLEX },
	{ CG_SMALL, 'c', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'C', CG_DOT_ABOVE },
	{ CG_SMALL, 'c', CG_DOT_ABOVE },
	{ CG_CAPITAL, 'C', CG_CARON },
	{ CG_SMALL, 'c', CG_CARON },
	{ CG_CAPITAL, 'D', CG_CARON },
	{ CG_SMALL, 'd', CG_COMMA_UR },
	{ CG_CAPITAL, 'D', CG_STROKE },
	{ CG_SMALL, 'd', CG_STROKE },
	{ CG_CAPITAL, 'E', CG_MACRON },
	{ CG_SMALL, 'e', CG_MACRON },
	{ CG_CAPITAL, 'E', CG_BREVE },
	{ CG_SMALL, 'e', CG_BREVE },
	{ CG_CAPITAL, 'E', CG_DOT_ABOVE },
	{ CG_SMALL, 'e', CG_DOT_ABOVE },
	{ CG_CAPITAL, 'E', CG_OGONEK },
	{ CG_SMALL, 'e', CG_OGONEK },
	{ CG_CAPITAL, 'E', CG_CARON },
	{ CG_SMALL, 'e', CG_CARON },
	{ CG_CAPITAL, 'G', CG_CIRCUMFLEX },
	{ CG_SMALL, 'g', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'G', CG_BREVE },
	{ CG_SMALL, 'g', CG_BREVE },
	{ CG_CAPITAL, 'G', CG_DOT_ABOVE },
	{ CG_SMALL, 'g', CG_DOT_ABOVE },
	{ CG_CAPITAL, 'G', CG_CEDILLA },
	{ CG_SMALL, 'g', CG_CEDILLA },
	{ CG_CAPITAL, 'H', CG_CIRCUMFLEX },
	{ CG_SMALL, 'h', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'H', CG_STROKE },
	{ CG_SMALL, 'h', CG_STROKE },
	{ CG_CAPITAL, 'I', CG_TILDE },
	{ CG_SMALL, 'i', CG_TILDE },
	{ CG_CAPITAL, 'I', CG_MACRON },
	{ CG_SMALL, 'i', CG_MACRON },
	{ CG_CAPITAL, 'I', CG_BREVE },
	{ CG_SMALL, 'i', CG_BREVE },
	{ CG_CAPITAL, 'I', CG_OGONEK },
	{ CG_SMALL, 'i', CG_OGONEK },
	{ CG_CAPITAL, 'I', CG_DOT_ABOVE },
	{ CG_NONE, 0, 0 }, // , CG_SMALL, 'DOTLESS I
	{ CG_NONE, 0, 0 }, // LATIN CAPITAL LIGATURE IJ
	{ CG_NONE, 0, 0 }, // LATIN SMALL LIGATURE IJ
	{ CG_CAPITAL, 'J', CG_CIRCUMFLEX },
	{ CG_SMALL, 'j', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'K', CG_CEDILLA },
	{ CG_SMALL, 'k', CG_CEDILLA },
	{ CG_NONE, 0, 0 }, // CG_SMALL, 'KRA
	{ CG_CAPITAL, 'L', CG_ACUTE },
	{ CG_SMALL, 'l', CG_ACUTE },
	{ CG_CAPITAL, 'L', CG_CEDILLA },
	{ CG_SMALL, 'l', CG_CEDILLA },
	{ CG_CAPITAL, 'L', CG_COMMA_URI },
	{ CG_SMALL, 'l', CG_COMMA_UR },
	{ CG_CAPITAL, 'L', CG_MIDDLE_DOT },
	{ CG_SMALL, 'l', CG_MIDDLE_DOT },
	{ CG_CAPITAL, 'L', CG_STROKE },
	{ CG_SMALL, 'l', CG_STROKE },
	{ CG_CAPITAL, 'N', CG_ACUTE },
	{ CG_SMALL, 'n', CG_ACUTE },
	{ CG_CAPITAL, 'N', CG_CEDILLA },
	{ CG_SMALL, 'n', CG_CEDILLA },
	{ CG_CAPITAL, 'N', CG_CARON },
	{ CG_SMALL, 'n', CG_CARON },
	{ CG_NONE, 0, 0 }, // CG_SMALL, 'N PRECEDED BY APOSTROPHE
	{ CG_NONE, 0, 0 }, //CG_CAPITAL, 'ENG
	{ CG_NONE, 0, 0 }, //CG_SMALL, 'ENG
	{ CG_CAPITAL, 'O', CG_MACRON },
	{ CG_SMALL, 'o', CG_MACRON },
	{ CG_CAPITAL, 'O', CG_BREVE },
	{ CG_SMALL, 'o', CG_BREVE },
	{ CG_CAPITAL, 'O', CG_DOUBLE_ACUTE },
	{ CG_SMALL, 'o', CG_DOUBLE_ACUTE },
	{ CG_NONE, 0, 0 }, // LATIN CAPITAL LIGATURE OE
	{ CG_NONE, 0, 0 }, // LATIN SMALL LIGATURE OE
	{ CG_CAPITAL, 'R', CG_ACUTE },
	{ CG_SMALL, 'r', CG_ACUTE },
	{ CG_CAPITAL, 'R', CG_CEDILLA },
	{ CG_SMALL, 'r', CG_CEDILLA },
	{ CG_CAPITAL, 'R', CG_CARON },
	{ CG_SMALL, 'r', CG_CARON },
	{ CG_CAPITAL, 'S', CG_ACUTE },
	{ CG_SMALL, 's', CG_ACUTE },
	{ CG_CAPITAL, 'S', CG_CIRCUMFLEX },
	{ CG_SMALL, 's', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'S', CG_CEDILLA },
	{ CG_SMALL, 's', CG_CEDILLA },
	{ CG_CAPITAL, 'S', CG_CARON },
	{ CG_SMALL, 's', CG_CARON },
	{ CG_CAPITAL, 'T', CG_CEDILLA },
	{ CG_SMALL, 't', CG_CEDILLA },
	{ CG_CAPITAL, 'T', CG_CARON },
	{ CG_SMALL, 't', CG_COMMA_T },
	{ CG_CAPITAL, 'T', CG_STROKE },
	{ CG_SMALL, 't', CG_STROKE },
	{ CG_CAPITAL, 'U', CG_TILDE },
	{ CG_SMALL, 'u', CG_TILDE },
	{ CG_CAPITAL, 'U', CG_MACRON },
	{ CG_SMALL, 'u', CG_MACRON },
	{ CG_CAPITAL, 'U', CG_BREVE },
	{ CG_SMALL, 'u', CG_BREVE },
	{ CG_CAPITAL, 'U', CG_RING_ABOVE },
	{ CG_SMALL, 'u', CG_RING_ABOVE },
	{ CG_CAPITAL, 'U', CG_DOUBLE_ACUTE },
	{ CG_SMALL, 'u', CG_DOUBLE_ACUTE },
	{ CG_CAPITAL, 'U', CG_OGONEK },
	{ CG_SMALL, 'u', CG_OGONEK },
	{ CG_CAPITAL, 'W', CG_CIRCUMFLEX },
	{ CG_SMALL, 'w', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'Y', CG_CIRCUMFLEX },
	{ CG_SMALL, 'y', CG_CIRCUMFLEX },
	{ CG_CAPITAL, 'Y', CG_DIAERESIS },
	{ CG_CAPITAL, 'Z', CG_ACUTE },
	{ CG_SMALL, 'z', CG_ACUTE },
	{ CG_CAPITAL, 'Z', CG_DOT_ABOVE },
	{ CG_SMALL, 'z', CG_DOT_ABOVE },
	{ CG_CAPITAL, 'Z', CG_CARON },
	{ CG_SMALL, 'z', CG_CARON },
	{ CG_NONE, 0, 0 } // CG_SMALL, 'LONG S
};

Draw& ScreenInfo();

void FontInfo::ComposeMetrics(Font fnt, CharMetrics *m, int page) const
{
	if(fnt.GetFaceInfo() & Font::COMPOSED) {
		FontInfo fi = ScreenInfo().GetFontInfoW(fnt);
		for(int i = 0; i < 32; i++) {
			int c = gc_info[i + (page - 8) * 32].ascii;
			m[i].lspc = fi.GetLeftSpace(c);
			m[i].width = fi[c];
			m[i].rspc = fi.GetRightSpace(c);
			if(gc_info[i].type == CG_COMMA_UR && !fi.IsFixedPitch())
				m[i].rspc += m[i].width / 2;
		}
	}
}

void Draw::ComposeText(int x, int y, int angle, const wchar *text, Font font, Color ink,
                       int n, const int *dx) {
	if(font.GetFaceInfo() & Font::COMPOSED) {
		for(int i = 0; i < n; i++)
			if(text[i] >= 256 && text[i] < 256 + 128) {
				FontInfo fi = GetFontInfoW(font);
				Clip(x, y, 9999, fi.GetHeight());
				Buffer<wchar> ntext(n);
				double sina;
				double cosa;
				Size   offset;
				if(angle)
					SinCos(angle, sina, cosa); //TODO global sin tables!
				int xp = 0;
				for(int i = 0; i < n; i++) {
					wchar chr = text[i];
					ntext[i] = chr;
					if(chr >= 256 && chr < 256 + 128) {
						CGInfo f = gc_info[chr - 256];
						if(f.type != CG_NONE) {
							ntext[i] = chr = f.ascii;
							Font mfnt = font;
							FontInfo mfi = fi;
							int my = 0;
							int mx = 0;
							int cw = fi[f.ascii];
							int mw = mfi[f.mark];
							wchar mark = f.mark;
							if(mark == CG_COMMA_UR && fi.IsFixedPitch())
								mark = CG_CARON;
							if(mark == CG_COMMA_T) {
								my -= 3 * font.GetHeight() / 4;
								mx += 4 * cw / 10;
								if(font.IsItalic())
									mx += mw / 2;
							}
							else
							if(mark == CG_COMMA_UR) {
								my -= 2 * font.GetHeight() / 3;
								mx += cw - mw / 4;
								mark = ',';
								if(font.IsItalic())
									mx += mw / 3;
							}
							else
							if(mark == CG_COMMA_URI) {
								my -= 2 * font.GetHeight() / 3;
								mx += cw - mw / 2;
								mark = ',';
								if(font.IsItalic())
									mx += mw / 3;
							}
							else
							if(mark != CG_STROKE) {
								if(f.mark != CG_OGONEK && f.mark != CG_CEDILLA && f.type == CG_CAPITAL) {
									mfnt = font(9 * font.GetHeight() / 10);
									mfi = GetFontInfoW(mfnt);
									my -= mark == CG_RING_ABOVE ? font.GetHeight() / 19
									                            : font.GetHeight() / 13;
								}
								mw = mfi[f.mark];
								mx += (cw - mw) / 2;
								if(font.IsItalic())
									mx += mw / 5;
							}
							if(angle)
								DrawText(int(x + cosa * (xp + mx) + sina * my),
								         int(y - sina * (xp + mx) + cosa * my),
								         angle, &mark, mfnt, ink, 1);
							else
								DrawText(x + xp + mx, y + my, &mark, mfnt, ink, 1);
						}
					}
					if(angle)
						DrawTextOp(int(x + cosa * xp), int(y - sina * xp),
						           angle, &chr, font, ink, 1, dx);
					if(dx)
						xp += dx[i];
					else
						xp += fi[chr];
				}
				if(!angle)
					DrawTextOp(x, y, angle, ntext, font, ink, n, dx);
				End();
				return;
			}
	}
	DrawTextOp(x, y, angle, text, font, ink, n, dx);
}

END_UPP_NAMESPACE
