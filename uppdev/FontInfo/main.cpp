#include "FontInfo.h"

using namespace Upp;

struct CharEntry {
	GlyphInfo info;
	word      chr;
	Font      font;
};

CharEntry fc_cache_global[4096];

CharEntry GetGlyphEntry(Font font, int chr)
{
	DrawLock __;
	unsigned hash = CombineHash(font.GetHashValue(), chr) & 511;
	CharEntry& e = fc_cache_global[hash];
	if(e.font != font || e.chr != chr) {
		e.font = font;
		e.chr = chr;
		e.info = GetGlyphInfoSys(font, chr);
	}
	return e;	
}

thread__ CharEntry fc_cache[512];

const GlyphInfo& GetGlyphInfo(Font font, int chr)
{
	unsigned hash = CombineHash(font.GetHashValue(), chr) & 511;
	CharEntry& e = fc_cache[hash];
	if(e.font != font || e.chr != chr)
		e = GetGlyphEntry(font, chr);
	return e.info;
}

struct FontEntry {
	CommonFontInfo info;
	Font           font;
};

thread__ FontEntry fi_cache[64];

const CommonFontInfo& GetFontInfo(Font font)
{
	unsigned hash = font.GetHashValue() & 63;
	FontEntry& e = fi_cache[hash];
	if(e.font != font) {
		DrawLock __;
		e.font = font;
		e.info = GetFontInfoSys(font);
	}
	return e.info;
}

Size GetTextSizeNew(const wchar *text, Font font, int n)
{
	int cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		cx += GetGlyphInfo(font, *wtext++).width;
		n--;
	}
	return Size(cx, GetFontInfo(font).height);
}

GUI_APP_MAIN
{
	WString x = "Just a little test!";
	RDUMP(GetTextSize(x, Arial(20), x.GetCount()));
	RDUMP(GetTextSizeNew(x, Arial(20), x.GetCount()));
	RDUMP(sizeof(GlyphInfo));
	RDUMP(sizeof(CharEntry));
	RDUMP(sizeof(CommonFontInfo));
	#ifndef _DEBUG
	int cx = 0;
	{
		RTIMING("Old");
		for(int i = 0; i < 1000000; i++) {
			cx += GetTextSize(x, Arial(20), x.GetCount()).cx;
		}
	}
	{
		RTIMING("New");
		for(int i = 0; i < 1000000; i++) {
			cx += GetTextSizeNew(x, Arial(20), x.GetCount()).cx;
		}
	}
	#endif
}
