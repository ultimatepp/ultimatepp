#include "FontInfo.h"

using namespace Upp;

#if 0

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
#endif

GUI_APP_MAIN
{
	{
		for(int i = 0; i < 65536; i++) {
			GlyphInfo g = GetGlyphInfoSys(Arial(100).Bold(), i);
			if(g.IsMissing())
				LOG(i);
		}
	}


	Vector<FaceInfo> fa = GetAllFacesSys();
	for(int i = 0; i < fa.GetCount(); i++)
		LOG(fa[i].name << ": scalable: " << fa[i].scalable << ", fixed: " << fa[i].fixed);
	CommonFontInfo f = GetFontInfoSys(Arial(100).Bold());
	DDUMP(f.ascent);
	DDUMP(f.descent);
	DDUMP(f.lineheight);
	DDUMP(f.maxwidth);
	DDUMP(f.path);
	
	FontInfo fi = Arial(100).Bold().Info();
	DDUMP(fi.GetAscent());
	DDUMP(fi.GetDescent());
	DDUMP(fi.GetMaxWidth());
	
	for(int c = '0'; c < 'z'; c++) {
		LOG("-------------------------");
		DDUMP((char)c);
		GlyphInfo g = GetGlyphInfoSys(Arial(100).Bold(), c);
		DDUMP(g.width);
		DDUMP(fi[c]);
		DDUMP(g.lspc);
		DDUMP(fi.GetLeftSpace(c));
		DDUMP(g.rspc);
		DDUMP(fi.GetRightSpace(c));
	}
	
	
#if 0
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
#endif
}
