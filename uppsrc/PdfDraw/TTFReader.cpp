#include "PdfDraw.h"

NAMESPACE_UPP

#define LLOG(x)   // LOG(x)
#define LDUMP(x)  // LLOG(#x << " = " << x);

int    TTFReader::Peek8(const char *s)
{
	if(s + 1 > font.End())
		Error();
	return (byte)*s;
}

int    TTFReader::Peek16(const char *s)
{
	if(s + 2 > font.End())
		Error();
	return ((byte)s[0] << 8) | (byte)s[1];
}

int    TTFReader::Peek32(const char *s)
{
	if(s + 4 > font.End())
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
	if(s + n > font.End())
		Error();
	String q(s, n);
	s += n;
	return q;
}

void TTFReader::Reset()
{
	memset(zero, 0, sizeof(zero));
	for(int i = 0; i < 256; i++)
		cmap[i] = zero;
}

void TTFReader::Free()
{
	for(int i = 0; i < 256; i++)
		if(cmap[i] != zero)
			delete[] cmap[i];
}

void TTFReader::SetGlyph(wchar chr, word glyph)
{
	int h = HIBYTE(chr);
	if(cmap[h] == zero)
		memset(cmap[h] = new word[256], 0, 256 * sizeof(word));
	cmap[h][LOBYTE(chr)] = glyph;
}

const char *TTFReader::Seek(const char *tab, int& len)
{
	ASSERT(strlen(tab) == 4);
	int q = table.Find(tab);
	if(q < 0)
		Error();
	len = table[q].length;
	return ~font + table[q].offset;
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
	int q = table.Find(tab);
	if(q < 0)
		return Null;
	return String(~font + table[q].offset, table[q].length);
}

bool TTFReader::Open(const String& fnt, bool symbol, bool justcheck)
{
	try {
		int i;
		Free();
		Reset();
		table.Clear();
		glyphinfo.Clear();
		font = fnt;
		const char *s = fnt;
		int q = Read32(s);
		if(q != 0x74727565 && q != 0x00010000)
			Error();
		int n = Read16(s);
		s += 6;
		while(n--) {
			Table& t = table.Add(Read(s, 4));
			s += 4;
			t.offset = Read32(s);
			t.length = Read32(s);
		}
		for(i = 0; i < table.GetCount(); i++)
			LLOG("table: " << table.GetKey(i) << " offset: " << table[i].offset << " length: " << table[i].length);

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

		if(justcheck)
			return true;

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

		s = Seek("hmtx");
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

		s = Seek("cmap");
		const char *p = s;
		p += 2;
		n = Read16(p);
		while(n--) {
			int pid = Read16(p);
			int psid = Read16(p);
			int offset = Read32(p);
			LLOG("cmap pid: " << pid << " psid: " << psid << " format: " << Peek16(s + offset));
			//Test with Symbol font !!!; improve - Unicode first, 256 bytes later..., symbol...
			if(symbol) {
				if(pid == 1 && psid == 0 && Peek16(s + offset) == 0) {
					LLOG("Reading symbol table");
					p = s + offset + 6;
					for(int i = 0; i < 256; i++)
						SetGlyph(i, (byte)p[i]);
					break;
				}
			}
			else
			if(pid == 3 && psid == 1) {
				p = s + offset;
				n = Peek16(p + 6) >> 1;
				LLOG("Found UNICODE encoding, offset " << offset << ", segments " << n);
				const char *seg_end = p + 14;
				const char *seg_start = seg_end + 2 * n + 2;
				const char *idDelta = seg_start + 2 * n;
				const char *idRangeOffset = idDelta + 2 * n;
				for(int i = 0; i < n; i++) {
					int start = Peek16(seg_start, i);
					int end = Peek16(seg_end, i);
					int delta = Peek16(idDelta, i);
					int ro = Peek16(idRangeOffset, i);
				    if (ro && delta == 0) {
				        LLOG("RangeOffset start: " << start << ", end: " << end << ", delta: " << (int16)delta);
				        LLOG("ro " << ro);
						const char *q = idRangeOffset + 2 * i + ro;
						for(int c = start; c <= end; c++) {
							SetGlyph(c, (word)Read16(q));
						}
				    }
				    else {
				        LLOG("start: " << start << ", end: " << end << ", delta: " << (int16)delta);
						for(int c = start; c <= end; c++)
							SetGlyph(c, c + delta);
				    }
				}
				break;
			}
		}


		const char *strings = Seek("name");
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

TTFReader::~TTFReader()
{
	Free();
}

END_UPP_NAMESPACE
