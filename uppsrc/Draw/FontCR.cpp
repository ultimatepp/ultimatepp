#include "Draw.h"

#define LLOG(x) // LOG(x)

namespace Upp {

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
	byte panose[10];
	dword coverage[8];
} sFontReplacements[] = {
	#include "Fonts.i"
};

bool ReadCmap(const char *ptr, int count, Event<int, int, int> range, dword flags)
{
	auto Get8 = [&](int i) { return i >= 0 && i + 1 <= count ? (byte)ptr[i] : 0; };
	auto Get16 = [&](int i) { return i >= 0 && i + 2 <= count ? Peek16be(ptr + i) : 0; };
	auto Get32 = [&](int i) { return i >= 0 && i + 4 <= count ? Peek32be(ptr + i) : 0; };
	for(int pass = 0; pass < (flags & CMAP_ALLOW_SYMBOL ? 3 : 2); pass++) {
		int p = 0;
		p += 2;
		int n = Get16(p);
		p += 2;
		while(n-- && p < count) {
			int pid = Get16(p); p += 2;
			int psid = Get16(p); p += 2;
			int offset = Get32(p); p += 4;
			if(offset < 0 || offset > count)
				return false;
			int format = Get16(offset);
			LLOG("cmap pid: " << pid << " psid: " << psid << " format: " << format);
			if((pid == 3 && psid == 10) || (pid == 0 && psid == 4) && format == 12 && pass == 0) {
				int p = offset;
				int ngroups = Get32(p + 12);
				p += 16; // pointer to groups table
				for(int i = 0; i < ngroups; i++) {
					int start = Get32(p);
					int end = Get32(p + 4);
					range(start, end, Get32(p + 8));
					p += 12;
				}
				return true;
			}
			if(((pid == 3 && psid == 1) || (pid == 0 && psid == 3) && format == 4) && pass == 1) {
				int p = offset;
				int n = Get16(p + 6) >> 1;
				int seg_end = p + 14;
				int seg_start = seg_end + 2 * n + 2;
				int idDelta = seg_start + 2 * n;
				int idRangeOffset = idDelta + 2 * n;
				for(int i = 0; i < n; i++) {
					int start = Get16(seg_start + 2 * i);
					int end = Get16(seg_end + 2 * i);
					int delta = Get16(idDelta + 2 * i);
					int ro = Get16(idRangeOffset + 2 * i);
					if(flags & CMAP_GLYPHS) {
					    if (ro && delta == 0) {
					        LLOG("RangeOffset start: " << start << ", end: " << end << ", delta: " << (int16)delta);
							int q = idRangeOffset + 2 * i + ro;
							for(int c = start; c <= end; c++) {
								range(c, c, (word)Get16(q));
								q += 2;
							}
					    }
					    else {
					        LLOG("Delta start: " << start << ", end: " << end << ", delta: " << (int16)delta);
					        range(start, end, start + delta);
					    }
					}
					else
						range(start, end, 0);
				}
				return true;
			}
			if(pid == 1 && psid == 0 && Get16(offset) == 0 && pass == 2) {
				LLOG("Reading symbol cmap");
				for(int i = 0; i < 256; i++)
					range(i, i, Get8(offset + 6 + i));
				return true;
			}
		}
	}
	return false;
}

bool ReadCmap(Font font, Event<int, int, int> range, dword flags)
{
	String h = font.GetData("cmap");
	return ReadCmap(h, h.GetCount(), range, flags);
}

bool GetPanoseNumber(Font font, byte *panose)
{
	memset(panose, 0, 10);
	String data = font.GetData("OS/2", 32, 10);
	if(data.GetCount() != 10)
		return false;
	memcpy(panose, data, data.GetCount());
	return true;
}

bool HasCodepoint(Font font, int c)
{
	Value v = MakeValue(
		[&] {
			StringBuffer h;
			RawCat(h, font);
			return (String)h;
		},
		[&](Value& v) {
			Vector<Tuple<int, int>>& ranges = CreateRawValue<Vector<Tuple<int, int>>>(v);
			ReadCmap(font, [&](int start, int end, int) {
				if(ranges.GetCount() && ranges.Top().b + 1 == start) // often we get sequence like { 1, 2 }, { 3, 4 } - optimize by joining
					ranges.Top().b = end;
				else
					ranges.Add({ start, end });
			});
			ranges.Shrink();
			return int(ranges.GetCount() * sizeof(Tuple<int, int>));
		}
	);
    const Vector<Tuple<int, int>>& ranges = v.To<Vector<Tuple<int, int>>>();
	for(Tuple<int, int> r : ranges)
		if(c >= r.a && c <= r.b)
			return true;
	return false;
}

struct sFontMetricsReplacement {
	Font src;
	Font dst;
	Font mdst;
};

int  PanoseDistance(byte *a, byte *b)
{
	int distance = 0;
	auto pval = [&](int val, int def) {
		if(val) return val;
		distance += 5;
		return def;
	};
	if(pval(a[0], 2) != pval(b[0], 2)) return INT_MAX;
	
	auto Add = [&](int i, int def) {
		int q = pval(a[i], def) - pval(b[i], def);
		distance += q * q;
	};
	Add(1, 6);
	Add(2, 6);
	Add(3, 4);
	return distance;
}

bool Replace(Font fnt, int chr, Font& rfnt)
{
#ifdef PLATFORM_COCOA
	static WString apple_kbd = "⌘⌃⇧⌥"; // these need special handling on macos to look nice
	if(apple_kbd.Find(chr) >= 0)
		fnt.Face(Font::SANSSERIF); // otherwise devangari font is used, which looks off
#endif

	bool prefer_color = PreferColorEmoji(chr) && !fnt.IsNoColor();
	static VectorMap<int, sRFace *> rface[2]; // face index to font info
	static Vector<int> color[2]; // colorimg faces
	static bool all_loaded;
	if(rface[0].GetCount() == 0) {
		for(int i = 0; i < __countof(sFontReplacements); i++) {
			int q = Font::FindFaceNameIndex(sFontReplacements[i].name);
			if(q > 0) {
				rface[0].Add(q) = &sFontReplacements[i];
			}
		}
		for(int i = 0; i < Font::GetFaceCount(); i++)
			if(Font::GetFaceInfo(i) & Font::COLORIMG)
				color[0].Add(i);
	}

	int face = fnt.GetFace();
	byte *panose;
	int q = rface[0].Find(face); // if we have this font in database, we do not need to load panose
	if(q >= 0)
		panose = rface[0][q]->panose;
	else {
		int q = rface[1].Find(face); // if we have this font in database, we do not need to load panose
		if(q >= 0)
			panose = rface[1][q]->panose;
		else {
			struct Panose : Moveable<Panose> {
				byte panose[10];
			};
			static ArrayMap<int, Panose> cache;
			int q = cache.Find(face);
			if(q < 0) {
				q = cache.GetCount();
				Font f2(face, 20);
				GetPanoseNumber(f2, cache.Add(face).panose);
			}
			panose = cache[q].panose;
		}
	}
	
	int wi;
	dword bit;

	auto ChrBit = [&](int c) {
		wi = 0;
		c = max(c, 0);
		if(c < 2048)
			bit = 0x80000000 >> (c >> 6);
		else {
			int bi = clamp(c - 2048, 0, 7*32*1024 - 1) >> 10;
			ASSERT((bi >> 5) + 1 < 8);
			wi = (bi >> 5) + 1;
			bit = 0x80000000 >> (bi & 31);
		}
	};
	
	for(int pass = 0; pass < 2; pass++) {
		Font f = fnt;
		Vector<int> distance;
		Vector<int> candidate;
		ChrBit(chr);
		for(int i = 0; i < rface[pass].GetCount(); i++)
			if(rface[pass][i]->coverage[wi] & bit) {
				int fi = rface[pass].GetKey(i);
				if(!fnt.IsNoColor() || !(Font::GetFaceInfo(fi) & Font::COLORIMG)) {
					distance.Add(PanoseDistance(rface[pass][i]->panose, panose));
					candidate.Add(fi);
				}
			}
		if(prefer_color)
			for(int fi : color[pass]) {
				distance.Add(-1);
				candidate.Add(fi);
			}
		StableIndexSort(distance, candidate);
		for(int fi : candidate) {
			f.Face(fi);
			if(HasCodepoint(f, chr)) {
				int a = fnt.GetAscent();
				int d = fnt.GetDescent();
				static WString apple_kbd = "⌘⌃⇧⌥"; // do not make these smaller it looks ugly...
				LLOG("Original font: " << fnt << " " << fnt.GetAscent() << " " << f.GetDescent() <<
				     ", replacement " << f << " " << f.GetAscent() << " " << f.GetDescent());
				if((f.GetAscent() > a || f.GetDescent() > d)
			#ifdef PLATFORM_COCOA
				 && apple_kbd.Find(chr) < 0
			#endif
				) {
					static sFontMetricsReplacement cache[256];
					int q = CombineHash(fnt, f) & 255;
					if(cache[q].src != fnt || cache[q].dst != f) {
						cache[q].src = fnt;
						cache[q].dst = f;
						double h = f.GetHeight();
						f.Height((int)min(h * a / max(1, f.GetAscent()), h * d / max(1, f.GetDescent())) + 1);
						while((f.GetAscent() > a || f.GetDescent() > d) && f.GetHeight() > 1) {
							f.Height(max(1, f.GetHeight() - 1/*max(1, f.GetHeight() / 20)*/));
							LLOG("Original font: " << fnt << " " << fnt.GetAscent() << " " << f.GetDescent() <<
							     ", downsized " << f << " " << f.GetAscent() << " " << f.GetDescent());
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
		if(!all_loaded) {
			all_loaded = true;
			for(int i = 1; i < Font::GetFaceCount(); i++) {
				dword fi = Font::GetFaceInfo(i);
				if(rface[1].Find(i) < 0 && !(fi & Font::SPECIAL) && (fi & Font::SCALEABLE)) {
					Font fnt(i, 40);
					sRFace fi;
					memset(&fi, 0, sizeof(fi));
					bool hascmap = ReadCmap(fnt, [&](int start, int end, int) {
						for(int c = start; c <= end; c = (c + 64) & ~63) {
							ChrBit(c);
							fi.coverage[wi] |= bit;
						}
					});
					if(hascmap && GetPanoseNumber(fnt, fi.panose)) {
						static Array<sRFace> xface; // for those additionally loaded
						sRFace& n = xface.Add();
						n = fi;
						rface[1].Add(i, &n);
					}
				}
				if((Font::GetFaceInfo(i) & Font::COLORIMG) && FindIndex(color[0], i) < 0)
					color[1].Add(i);
			}
		}
	}
	return false;
}

String GetFontDataSysSys(Stream& in, int fonti, const char *table, int offset, int size)
{ // read truetype or opentype table from file - common implementation
	int q = in.Get32be();
	if(q == 0x74746366) { // font collection
		in.Get32(); // skip major/minor version
		int nfonts = in.Get32be();
		if(fonti >= nfonts)
			return Null;
		in.SeekCur(fonti * 4);
		int offset = in.Get32be();
		if(offset < 0 || offset >= in.GetSize())
			return Null;
		in.Seek(offset);
		q = in.Get32be();
	}
	if(q != 0x74727565 && q != 0x00010000 && q != 0x4f54544f) // 0x4f54544f means CCF font!
		return Null;
	int n = in.Get16be();
	in.Get32();
	in.Get16();
	while(n--) {
		if(in.IsError() || in.IsEof()) return Null;
		String tab = in.Get(4);
		in.Get32();
		int off = in.Get32be();
		int len = in.Get32be();
		if(tab == table) {
			if(off < 0 || len < 0 || off + len > in.GetSize())
				return Null;
			len = min(len - offset, size);
			if(len < 0)
				return Null;
			in.Seek(off + offset);
			return in.Get(len);
		}
	}
	return Null;
}

}
