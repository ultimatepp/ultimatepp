#ifndef _Draw_FontInt_h_
#define _Draw_FontInt_h_

// Internal header!

struct FaceInfo : Moveable<FaceInfo> {
	String name;
	dword  info;
};

struct CommonFontInfo {
	int  ascent;
	int  descent;
	int  external;
	int  internal;
	int  height;
	int  lineheight;
	int  overhang;
	int  avewidth;
	int  maxwidth;
	int  firstchar;
	int  charcount;
	int  default_char;
	int  spacebefore;
	int  spaceafter;
	bool fixedpitch;
	bool scaleable;

#ifdef PLATFORM_POSIX
	char path[256];
#endif
};

class Font;

struct GlyphInfo {
	int16 width;
	int16 lspc;
	int16 rspc;
	
	bool IsNormal() const     { return (word)width != 0x8000; }
	bool IsComposed() const   { return !IsNormal() && (lspc == -1 || lspc == -11); }
	bool IsComposedLM() const { return !IsNormal() && lspc == -11; }
	bool IsReplaced() const   { return !IsNormal() && lspc >= 0; }
	bool IsMissing() const    { return !IsNormal() && lspc == -2; }
};

void      Std(Font& font);
GlyphInfo GetGlyphInfo(Font font, int chr);
const     CommonFontInfo& GetFontInfo(Font font);
bool      IsNormal(Font font, int chr);
void      GlyphMetrics(GlyphInfo& f, Font font, int chr);

#endif
