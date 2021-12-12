#include "TypeReader.h"

#define LLOG(x)   // LOG(x)
#define LDUMP(x)  // LOG(#x << " = " << x);

int FontTypeReader::Error()
{
	throw Fail(); return 0;
}

int    FontTypeReader::Peek8(const char *s)
{
	return s + 1 < data.end() ? (byte)*s : Error();
}

int    FontTypeReader::Peek16(const char *s)
{
	return s + 2 < data.end() ? Peek16be(s) : Error();
}

int    FontTypeReader::Peek32(const char *s)
{
	return s + 4 < data.end() ? Peek32be(s) : Error();
}

int    FontTypeReader::Peek8(const char *s, int i)
{
	return Peek8(s + i);
}

int    FontTypeReader::Peek16(const char *s, int i)
{
	return Peek16(s + 2 * i);
}

int    FontTypeReader::Peek32(const char *s, int i)
{
	return Peek32(s + 4 * i);
}

int    FontTypeReader::Read8(const char *&s)
{
	int q = byte(*s);
	s++;
	return q;
}

int    FontTypeReader::Read16(const char *&s)
{
	int q = Peek16(s);
	s += 2;
	return q;
}

int    FontTypeReader::Read32(const char *&s)
{
	int q = Peek32(s);
	s += 4;
	return q;
}

bool FontTypeReader::Open(Font font, bool symbol, bool justcheck)
{
//	DDUMP(fnt.GetCount());
	try {
		int i;
#if 0
		for(i = 0; i < table.GetCount(); i++)
			LLOG("table: " << table.GetKey(i) << " offset: " << table[i].offset << " length: " << table[i].length);

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
#endif

		data = font.GetData("OS/2");
		panose.Clear();
		if(data.GetCount() > 32 + 10)
			panose = data.Mid(32, 10);

		bool found = false;
		data = font.GetData("cmap");
		for(int pass = 0; pass < 2; pass++) {
			const char *p = data;
//			LOGHEXDUMP(p, 256);
			p += 2;
			int n = Read16(p);
//			DDUMP(n);
			while(n--) {
				int pid = Read16(p);
				int psid = Read16(p);
				int offset = Read32(p);
				if(offset < 0 || offset > data.GetCount())
					Error();
				int format = Peek16(~data + offset);
				LLOG("cmap pid: " << pid << " psid: " << psid << " format: " << format);
				//Test with Symbol font !!!; improve - Unicode first, 256 bytes later..., symbol...
				if(symbol) {
					if(pid == 1 && psid == 0 && Peek16(~data + offset) == 0) {
						LLOG("Reading symbol table");
						p = ~data + offset + 6;
						for(int i = 0; i < 256; i++);
							//SetGlyph(i, (byte)p[i]);
						goto done;
					}
				}
				else
				if((pid == 3 && psid == 10) || (pid == 0 && psid == 4) && format == 12 && pass == 0) {
					const char *p = ~data + offset;
					int ngroups = Peek32(p + 12);
					p += 16; // pointer to groups table
					for(int i = 0; i < ngroups; i++) {
						LLOG(Peek32(p) << " - " << Peek32(p + 4) << ", glyphi: " << Peek32(p + 8));
						ranges.Add(MakeTuple(Peek32(p), Peek32(p + 4)));
						p += 12;
					}
					goto done;
				}
				else
				if((pid == 3 && psid == 1) || (pid == 0 && psid == 3) && format == 4 && pass == 1) {
					const char *p = ~data + offset;
					int n = Peek16(p + 6) >> 1;
					LLOG("Found UNICODE encoding, offset " << offset << ", segments " << n);
					const char *seg_end = p + 14;
					const char *seg_start = seg_end + 2 * n + 2;
					const char *idDelta = seg_start + 2 * n;
					const char *idRangeOffset = idDelta + 2 * n;
					for(int i = 0; i < n; i++) {
						int start = Peek16(seg_start, i);
						int end = Peek16(seg_end, i);
						ranges.Add(MakeTuple(start, end));
					#if 0
						int delta = Peek16(idDelta, i);
						int ro = Peek16(idRangeOffset, i);
					    if (ro && delta == 0) {
					        LOG(start << " - " << end);
					        LLOG("RangeOffset start: " << start << ", end: " << end << ", delta: " << (int16)delta);
					        LLOG("ro " << ro);
					//		const char *q = idRangeOffset + 2 * i + ro;
					//		for(int c = start; c <= end; c++) {
					//			SetGlyph(c, (word)Read16(q));
					//		}
					    }
					    else {
					        LOG(start << " - " << end);
					        LLOG("Delta start: " << start << ", end: " << end << ", delta: " << (int16)delta);
					//		for(int c = start; c <= end; c++);
					//			SetGlyph(c, c + delta);
					    }
					#endif
					}
					break;
				}
			}
		}
	done:;
	#if 0
		String names = font.GetData("name");
		if(names.GetCount()) {
			const char *strings = names;
			const char *s = strings + 2;
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
		}
		DDUMP(ps_name);
	#endif
	}
	catch(Fail) {
		LLOG("ERROR");
		return false;
	}

	return true;
}
