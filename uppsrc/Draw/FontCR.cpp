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

bool Compose(Font font, int chr, ComposedGlyph& cg)
{
	if(chr < 256 || chr > 256 + 128)
		return false;
	CGInfo f = gc_info[chr - 256];
	if(f.type == CG_NONE)
		return false;
	GlyphInfo gi = GetGlyphInfo(font, f.ascii);
	if(!gi.IsNormal())
		return false;
	int cw = gi.width;
	CommonFontInfo fi = GetFontInfo(font);
	gi = GetGlyphInfo(font, f.mark);
	if(!gi.IsNormal())
		return false;
	int mw = gi.width;
	cg.mark_font = font;
	cg.mark_pos.x = cg.mark_pos.y = 0;
	cg.basic_char = f.ascii;
	cg.mark_char = f.mark;
	if(cg.mark_char == CG_COMMA_UR && fi.fixedpitch)
		cg.mark_char = CG_CARON;
	if(cg.mark_char == CG_COMMA_T) {
		cg.mark_pos.y -= 3 * font.GetHeight() / 4;
		cg.mark_pos.x += 4 * cw / 10;
		if(font.IsItalic())
			cg.mark_pos.x += mw / 2;
	}
	else
	if(cg.mark_char == CG_COMMA_UR) {
		cg.mark_pos.y -= 2 * font.GetHeight() / 3;
		cg.mark_pos.x += cw - mw / 4;
		cg.mark_char = ',';
		if(font.IsItalic())
			cg.mark_pos.x += mw / 3;
	}
	else
	if(cg.mark_char == CG_COMMA_URI) {
		cg.mark_pos.y -= 2 * font.GetHeight() / 3;
		cg.mark_pos.x += cw - mw / 2;
		cg.mark_char = ',';
		if(font.IsItalic())
			cg.mark_pos.x += mw / 3;
	}
	else
	if(cg.mark_char != CG_STROKE) {
		if(cg.mark_char != CG_OGONEK && cg.mark_char != CG_CEDILLA && f.type == CG_CAPITAL) {
			cg.mark_font = font(9 * font.GetHeight() / 10);
			mw = GetGlyphInfo(cg.mark_font, f.mark).width;
			cg.mark_pos.y -= cg.mark_char == CG_RING_ABOVE ? font.GetHeight() / 19
			                            : font.GetHeight() / 10;
		}
		cg.mark_pos.x += (cw - mw) / 2;
		if(font.IsItalic())
			cg.mark_pos.x += mw / 5;
	}
	return true;
}

struct sRFace {
	const char *name;
	dword l, h;
} sFontReplacements[] = {
	{ "sans-serif", 0xffee0008, 0xdc000801 },
	{ "Arial", 0xfffe0000, 0x9c000801 },
	{"\346\226\260\345\256\213\344\275\223", 0xfd800000, 0x9ffff00d },//SimSun (or New Song Ti)
	{"SimSun", 0xfd800000, 0x9ffff00d },//SimSun (or New Song Ti)
	{"\345\256\213\344\275\223", 0xfd800000, 0x9ffff00d }, // Song Ti
	{"\345\276\256\350\275\257\351\233\205\351\273\221", 0xfd800000, 0x9ffff00f }, //MS Ya Hei
	{"Microsoft YaHei", 0xfd800000, 0x9ffff00f }, //MS Ya Hei
//	{"\351\273\221\344\275\223", 0xfd800000, 0x09ffff00 },  // Hei Ti
//	{"\346\226\207\346\263\211\351\251\277\346\255\243\351\273\221", 0xfd800000, 0x09ffff00 }, //WenQuanYi Zheng Hi
//	{"\346\226\207\346\263\211\351\251\277\347\255\211\345\256\275\345\276\256\347\261\263\351\273\221", 0xfd800000, 0x09ffff00 },//WenQuanYi Wei Hei
//	{"\344\273\277\345\256\213", 0xfd800000, 0x09ffff00 }, //Fang Song
//	{"\346\245\267\344\275\223", 0xfd800000, 0x09ffff00 }, // Kai Ti
	{ "Arial Unicode MS", 0xfffc3fef, 0xfa7ff7ef },
	{ "MS UI Gothic", 0xffc01008, 0xfffff001 },
	{ "MS Mincho", 0xffc01008, 0xfffff001 },
	{ "VL Gothic", 0xfd800000, 0x9a7ff80f },
	{ "VL PGothic", 0xffe00008, 0xde7ff80f },
	{ "UnDotum", 0xe5800000, 0xaa7ff7ef },
	{ "UnBatang", 0xe5800000, 0xaa7ff7ef },
	{ "DejaVu Sans Mono", 0xffec0004, 0xfc00080f },
	{ "DejaVu Sans", 0xfffd000c, 0xfc40080f },
	{ "AlArabiyaFreeSerif", 0xffdc0008, 0xd800000f },
	{ "Kochi Mincho", 0xffdc0008, 0xd800000f },
	{ "Kochi Gothic", 0xffdc0008, 0xd800000f },
	{ "Sazanami Mincho", 0xffdc0008, 0xd800000f },
	{ "Sazanami Gothic", 0xffdc0008, 0xd800000f },
	{ "Gulim", 0xf7c00000, 0xba7ff7e1 },
	{ "PMingLiU", 0xff800000, 0x9ffff001 }, // <--- SHOULD MOVE UP
	{ "FreeSans", 0xfff23d00, 0xfc00000f },
	{ "FreeSerif", 0xfffd3938, 0xfc00080f },
	{ "Symbol", 0xe4000000, 0x8800000f },
};

struct sFontMetricsReplacement {
	Font src;
	Font dst;
	Font mdst;
};

bool Replace(Font fnt, int chr, Font& rfnt)
{
	static Vector<int> rface;
	static Vector<dword> l, h;
	ONCELOCK {
		for(int i = 0; i < __countof(sFontReplacements) && rface.GetCount() < 20; i++) {
			int q = Font::FindFaceNameIndex(sFontReplacements[i].name);
			if(q > 0) {
				rface.Add(q);
				l.Add(sFontReplacements[i].l);
				h.Add(sFontReplacements[i].h);
			}
		}
	}

	Font f = fnt;
//	dword tl = chr < 4096 ? 0x80000000 >> (chr >> 7) : 0;
//	dword th = 0x80000000 >> ((dword)chr >> 11);
	for(int i = 0; i < rface.GetCount(); i++) {
		if(/*((l[i] & tl) || (h[i] & th)) && */IsNormal(f.Face(rface[i]), chr)) {
			int a = fnt.GetAscent();
			int d = fnt.GetDescent();
			if(f.GetAscent() > a || f.GetDescent() > d) {
				static sFontMetricsReplacement cache[256];
				int q = CombineHash(fnt, f) & 255;
				if(cache[q].src != fnt || cache[q].dst != f) {
					cache[q].src = fnt;
					cache[q].dst = f;
					while((f.GetAscent() > a || f.GetDescent() > d) && f.GetHeight() > 1) {
						f.Height(max(1, min(f.GetHeight() - 1, f.GetHeight() * 9 / 10)));
					}
					cache[q].mdst = f;
				}
				else
					f = cache[q].mdst;
			}
			rfnt = f;
			return true;
		}
	}
	return false;
}

END_UPP_NAMESPACE
