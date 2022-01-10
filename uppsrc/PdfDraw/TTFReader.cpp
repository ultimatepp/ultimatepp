#include "PdfDraw.h"

namespace Upp {

#define LLOG(x)   // LOG(x)
#define LDUMP(x)  // LLOG(#x << " = " << x);

int    TTFReader::Peek8(const char *s)
{
	if(s + 1 > current_table.End())
		Error();
	return (byte)*s;
}

int    TTFReader::Peek16(const char *s)
{
	if(s + 2 > current_table.End())
		Error();
	return ((byte)s[0] << 8) | (byte)s[1];
}

int    TTFReader::Peek32(const char *s)
{
	if(s + 4 > current_table.End())
		Error();
	return ((byte)s[0] << 24) | ((byte)s[1] << 16) | ((byte)s[2] << 8) | (byte)s[3];
}

int    TTFReader::Peek8(const char *s, int i)
{
	return Peek16(s + i);
}

int    TTFReader::Peek16(const char *s, int i)
{
	return Peek16(s + 2 * i);
}

int    TTFReader::Peek32(const char *s, int i)
{
	return Peek32(s + 4 * i);
}

int    TTFReader::Read8(const char *&s)
{
	int q = byte(*s); s++; return q;
}

int    TTFReader::Read16(const char *&s)
{
	int q = Peek16(s); s += 2; return q;
}

int    TTFReader::Read32(const char *&s)
{
	int q = Peek32(s); s += 4; return q;
}

String TTFReader::Read(const char *&s, int n)
{
	if(s + n > current_table.End())
		Error();
	String q(s, n);
	s += n;
	return q;
}

void TTFReader::Reset()
{
	glyph_map.Clear();
}

void TTFReader::SetGlyph(wchar chr, word glyph)
{
	glyph_map.GetAdd(chr) = glyph;
}

const char *TTFReader::Seek(const char *tab, int& len)
{
	ASSERT(strlen(tab) == 4);
	current_table = font.GetData(tab);
	len = current_table.GetCount();
	return current_table;
}

const char *TTFReader::Seek(const char *tab)
{
	int dummy;
	return Seek(tab, dummy);
}

void TTFReader::Seek(const char *tab, TTFStreamIn& s)
{
	int len;
	s.beg = s.s = Seek(tab, len);
	s.lim = s.s + len;
}

String TTFReader::GetTable(const char *tab)
{
	ASSERT(strlen(tab) == 4);
	return font.GetData(tab);
}

bool TTFReader::Open(const Font& fnt, bool symbol, bool justcheck)
{
	try {
		int i;
		Reset();
		table.Clear();
		glyphinfo.Clear();
		font = fnt;

		TTFStreamIn is;
		Seek("head", is);
		head.Serialize(is);
		if(head.magicNumber != 0x5F0F3CF5)
			Error();

		LDUMP(head.unitsPerEm);
		LDUMP(head.xMin);
		LDUMP(head.yMin);
		LDUMP(head.xMax);
		LDUMP(head.yMax);
		LDUMP(head.indexToLocFormat);

		Seek("maxp", is);
		maxp.Serialize(is);
		LDUMP(maxp.numGlyphs);

		Seek("post", is);
		post.Serialize(is);
//		LLOGHEXLDUMP(is.s, is.lim - is.s);
		LDUMP((post.format >> 16));
		LDUMP(post.italicAngle);
		LDUMP(post.italicAngle);
		LDUMP(post.underlinePosition);
		LDUMP(post.underlineThickness);
		LDUMP(post.italicAngle);

		Seek("hhea", is);
		hhea.Serialize(is);
		LDUMP(hhea.ascent);
		LDUMP(hhea.descent);
		LDUMP(hhea.lineGap);
		LDUMP(hhea.advanceWidthMax);
		LDUMP(hhea.numOfLongHorMetrics);

		if(hhea.numOfLongHorMetrics > maxp.numGlyphs)
			Error();

		glyphinfo.SetCount(maxp.numGlyphs);

		if(justcheck)
			return ReadCmap(fnt, [&](int, int, int) {}, CMAP_ALLOW_SYMBOL);

		const char *s = Seek("hmtx");
		if(!s) Error();
		int aw = 0;
		for(i = 0; i < hhea.numOfLongHorMetrics; i++) {
			aw = glyphinfo[i].advanceWidth = (uint16)Read16(s);
			glyphinfo[i].leftSideBearing = (int16)Read16(s);
		}
		for(; i < maxp.numGlyphs; i++) {
			glyphinfo[i].advanceWidth = aw;
			glyphinfo[i].leftSideBearing = (int16)Read16(s);
		}

		s = Seek("loca");
		if(!s) Error();
		for(i = 0; i < maxp.numGlyphs; i++)
			if(head.indexToLocFormat) {
				glyphinfo[i].offset = Peek32(s, i);
				glyphinfo[i].size = Peek32(s, i + 1) - glyphinfo[i].offset;
			}
			else {
				glyphinfo[i].offset = 2 * (word)Peek16(s, i);
				glyphinfo[i].size = 2 * (word)Peek16(s, i + 1) - glyphinfo[i].offset;
			}

		for(i = 0; i < maxp.numGlyphs; i++)
			LLOG(i << " advance: " << glyphinfo[i].advanceWidth << ", left: " << glyphinfo[i].leftSideBearing
			      << ", offset: " << glyphinfo[i].offset << ", size: " << glyphinfo[i].size);

		ReadCmap(fnt, [&](int start, int end, int glyph) {
			for(int ch = start; ch <= end; ch++)
				SetGlyph(ch, glyph++);
		}, CMAP_GLYPHS|CMAP_ALLOW_SYMBOL);

		const char *strings = Seek("name");
		if(!strings) Error();
		s = strings + 2;
		int count = Read16(s);
		strings += (word)Read16(s);
		for(int i = 0; i < count; i++) {
			int platform = Read16(s);
			s += 4;
			if(Read16(s) == 6) {
				int len = Read16(s);
				int offset = Read16(s);
				if(platform == 1)
					ps_name = String(strings + offset, len);
				else {
					s = strings + offset;
					len >>= 1;
					while(len--)
						ps_name.Cat(Read16(s));
				}
				break;
			}
			s += 4;
		}
		LDUMP(ps_name);
	}
	catch(Fail) {
		return false;
	}
	catch(TTFStream::Fail) {
		return false;
	}

	return true;
}

TTFReader::TTFReader()
{
	Reset();
}

}
