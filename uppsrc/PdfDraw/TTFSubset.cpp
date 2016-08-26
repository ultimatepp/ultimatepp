#include "PdfDraw.h"

namespace Upp {

dword sCheckSum(const String& data)
{
	const byte *s = (byte *)~data;
	const byte *lim = s + (data.GetCount() & ~3);
	dword sum = 0;
	while(s < lim) {
		sum += (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
		s += 4;
	}
	byte h[4];
	h[0] = h[1] = h[2] = h[3] = 0;
	byte *t = h;
	while(s < (byte *)data.End())
		*t++ = *s++;
	sum += (h[0] << 24) | (h[1] << 16) | (h[2] << 8) | h[3];
	return sum;
}

void TTFReader::Set(NewTable& m, const String& data)
{
	m.data = data;
	m.checksum = sCheckSum(data);
}

void TTFReader::Add(Array<NewTable>& t, const char *name, const String& data)
{
	if(IsNull(data))
		return;
	NewTable& m = t.Add();
	m.name = name;
	ASSERT(m.name.GetCount() == 4);
	Set(m, data);
	m.checksum = sCheckSum(m.data);
}

String TTFReader::CreateHeader(const Array<NewTable>& t)
{
	TTFStreamOut out;
	int i = 1;
	int k = 0;
	while(i <= t.GetCount()) {
		i <<= 1;
		k++;
	}
	out.Put32(0x00010000);
	out.Put16(t.GetCount());
	out.Put16(i << 3);
	out.Put16(k - 1);
	out.Put16((t.GetCount() << 4) - (i << 3));
	int offset = 16 * t.GetCount() + 12;
	for(int i = 0; i < t.GetCount(); i++) {
		const NewTable& m = t[i];
		out.Put(m.name);
		out.Put32(m.checksum);
		out.Put32(offset);
		out.Put32(m.data.GetLength());
		offset += (m.data.GetLength() + 3) & ~3;
	}
	return out;
}

String TTFReader::Subset(const Vector<wchar>& chars, int first, bool os2)
{
	try {
		ASSERT(chars.GetCount() + first < 256);

		TTFStreamOut cmap;
		cmap.Put16(0);
		cmap.Put16(1);
		cmap.Put16(1);
		cmap.Put16(0);
		cmap.Put32(cmap.out.GetLength() + 4);
		cmap.Put16(0);
		cmap.Put16(262);
		cmap.Put16(0);

		Index<int> glyph;
		glyph.Add(0);
		for(int i = 0; i < first; i++)
			cmap.Put8(0);
		for(int i = 0; i < chars.GetCount(); i++) {
			int q = GetGlyph(chars[i]);
			if(q) {
				cmap.Put8(glyph.GetCount());
				glyph.Add(q);
			}
			else
				cmap.Put8(0);
		}
		for(int i = chars.GetCount(); i < 256; i++)
			cmap.Put8(0);

		TTFStreamOut loca;
		TTFStreamIn  in;
		TTFStreamOut glyf;
		TTFStreamOut hmtx;
		Seek("glyf", in);
		for(int i = 0; i < glyph.GetCount(); i++) {
			int gi = glyph[i];
			if(gi >= glyphinfo.GetCount())
				throw Fail();
			const GlyphInfo &gf = glyphinfo[gi];
			loca.Put32(glyf.out.GetLength());
			hmtx.Put16(gf.advanceWidth);
			hmtx.Put16(gf.leftSideBearing);
			if(gf.size) {
				if(gf.size < 10)
					Error();
				in.Seek(gf.offset);
				int c = (int16)Copy16(glyf, in);
				Copy(glyf, in, 8);
				if(c < 0) {
					int flags;
					do {
						flags = Copy16(glyf, in);
						glyf.Put16(glyph.FindAdd(in.Get16()));
						if(flags & ARG_1_AND_2_ARE_WORDS)
							Copy(glyf, in, 4);
						else
							Copy(glyf, in, 2);
						if(flags & WE_HAVE_A_SCALE)
							Copy16(glyf, in);
						else
						if(flags & WE_HAVE_AN_X_AND_Y_SCALE)
							Copy(glyf, in, 4);
						else
						if(flags & WE_HAVE_A_TWO_BY_TWO)
							Copy(glyf, in, 8);
					}
					while(flags & MORE_COMPONENTS);
					if(flags & WE_HAVE_INSTRUCTIONS)
						Copy(glyf, in, Copy16(glyf, in));
				}
				else
					Copy(glyf, in, gf.size - 10);
			}
		}
		loca.Put32(glyf.out.GetLength());

		Hhea nhhea = hhea;
		nhhea.numOfLongHorMetrics = glyph.GetCount();
		TTFStreamOut hhea;
		nhhea.Serialize(hhea);

		Maxp nmaxp = maxp;
		nmaxp.numGlyphs = glyph.GetCount();
		TTFStreamOut maxp;
		nmaxp.Serialize(maxp);

		Post npost = post;
		npost.format = 0x00030000;
		TTFStreamOut post;
		npost.Serialize(post);

		Head nhead = head;
		nhead.checkSumAdjustment = 0;
		nhead.indexToLocFormat = 1;
		TTFStreamOut head;
		nhead.Serialize(head);

		Array<NewTable> tab;
		if(os2)
			Add(tab, "OS/2", GetTable("OS/2"));
		Add(tab, "cmap", cmap);
		Add(tab, "cvt ", GetTable("cvt "));
		Add(tab, "fpgm", GetTable("fpgm"));
		Add(tab, "glyf", glyf);
		int headi = tab.GetCount();
		Add(tab, "head", head);
		Add(tab, "hhea", hhea);
		Add(tab, "hmtx", hmtx);
		Add(tab, "loca", loca);
		Add(tab, "maxp", maxp);
		Add(tab, "name", GetTable("name"));
		Add(tab, "post", post);
		Add(tab, "prep", GetTable("prep"));

		dword chksum = sCheckSum(CreateHeader(tab));
		for(int i = 0; i < tab.GetCount(); i++)
			chksum += tab[i].checksum;

		nhead.checkSumAdjustment = 0xB1B0AFBA - chksum;
		head.out.Clear();
		nhead.Serialize(head);
		Set(tab[headi], head);

		String result = CreateHeader(tab);
		for(int i = 0; i < tab.GetCount(); i++) {
			result.Cat(tab[i].data);
			result.Cat(0, (4 - result.GetCount()) & 3);
		}

		return result;
	}
	catch(Fail) {
		return String::GetVoid();
	}
	catch(TTFStream::Fail) {
		return String::GetVoid();
	}
}

}
