#include <Core/Core.h>

using namespace Upp;

struct SymbolRange {
	int         from;
	int         to;
	const char *text;
} symbol_ranges[] = {
	0x2190, 0x21FF, "Arrows",
	0x2200, 0x22FF, "Mathematical Operators",
	0x2300, 0x23FF, "Miscellaneous Technical .Mathematical",
//	0x2400, 0x243F, "Control Pictures",
//	0x2440, 0x245F, "Optical Character Recognition",
//	0x2460, 0x24FF, "Enclosed Alphanumerics",
//	0x2500, 0x257F, "Box Drawing",
//	0x2580, 0x259F, "Block Elements",
	0x25A0, 0x25FF, "Geometric Shapes",
	0x2600, 0x26FF, "Miscellaneous Symbols",
	0x2700, 0x27BF, "Dingbats",
	0x27C0, 0x27EF, "Miscellaneous Mathematical Symbols-A",
	0x27F0, 0x27FF, "Supplemental Arrows-A",
//	0x2800, 0x28FF, "Braille Patterns",
	0x2900, 0x297F, "Supplemental Arrows-B",
	0x2980, 0x29FF, "Miscellaneous Mathematical Symbols-B",
	0x2A00, 0x2AFF, "Supplemental Mathematical Operators",
	0x2B00, 0x2BFF, "Miscellaneous Symbols and Arrows",
	0x1F300, 0x1F5FF, "Miscellaneous Symbols and Pictographs",
	0x1F600, 0x1F64F, "Emoticons .Symbols",
	0x1F650, 0x1F67F, "Ornamental Dingbats",
	0x1F680, 0x1F6FF, "Transport and Map Symbols",
	0x1F700, 0x1F77F, "Alchemical Symbols",
	0x1F780, 0x1F7FF, "Geometric Shapes Extended",
	0x1F800, 0x1F8FF, "Supplemental Arrows-C",
	0x1F900, 0x1F9FF, "Supplemental Symbols and Pictographs",
//	0x1FA00, 0x1FA6F, "Chess Symbols",
	0x1FA70, 0x1FAFF, "Symbols and Pictographs Extended-A",
//	0x1F000, 0x1F02F, "Mahjong Tiles",
//	0x1F030, 0x1F09F, "Domino Tiles",
//	0x1F0A0, 0x1F0FF, "Playing Cards",
};

CONSOLE_APP_MAIN
{
	FileIn in(GetDataFile("UnicodeData.txt"));
	Vector<String> group = { "symbols", "arrows", "shapes", "dingbats", "mathematical" };
	VectorMap<String, Vector<Tuple<int, String>>> out;
	Vector<Tuple<int, String>> out2;
	for(String s : group)
		out.GetAdd(s);
	while(!in.IsEof()) {
		Vector<String> h = Split(in.GetLine(), ';');
		if(h.GetCount() > 2) {
			int codepoint = stou(~h[0], NULL, 16);
			String text = h[1];
			for(SymbolRange r : symbol_ranges) {
				out2 << MakeTuple(codepoint , ToLower(text));
				if(codepoint >= r.from && codepoint <= r.to) {
					String g = ToLower(r.text);
					for(String s : group) {
						if(g.Find(s) >= 0)
							g = s;
					}
					out.GetAdd(g) << MakeTuple(codepoint , ToLower(text));
				}
			}
		}
	}
	DDUMP(StoreAsString(out).GetCount());
	DDUMP(ZCompress(StoreAsString(out)).GetCount());
	DDUMP(FastCompress(StoreAsString(out)).GetCount());
	DLOG("==========");
	DDUMP(StoreAsString(out2).GetCount());
	DDUMP(ZCompress(StoreAsString(out2)).GetCount());
	DDUMP(FastCompress(StoreAsString(out2)).GetCount());
	
	String h = ZCompress(StoreAsString(out));
	String r;
	for(int i = 0; i < h.GetCount(); i++) {
		r << (int)(byte)h[i] << ",";
		if((i & 31) == 31)
			r << "\n";
	}
	LOG("=========================");
	LOG(r);
/*
	for(int i = 0; i < out.GetCount(); i++) {
		LOG("///////////////");
		LOG("0, " << AsCString(out.GetKey(i)) << ",");
		for(String s : out[i])
			LOG(s);
	}
*/
}
