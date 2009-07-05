#ifndef _FontInfo_FontInfo_h_
#define _FontInfo_FontInfo_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct CommonFontInfo : Moveable<CommonFontInfo> {
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
	int    underline_thickness;
	int    underline_position;
	String path;
#endif
};

CommonFontInfo GetFontInfoSys(Font font);

struct GlyphInfo : Moveable<GlyphInfo> {
	int16 width;
	int16 lspc;
	int16 rspc;
	
	bool IsNormal() const     { return (word)width != 0x8000; }
	bool IsComposed() const   { return !IsNormal() && (lspc == -1 || lspc == -11); }
	bool IsComposedLM() const { return !IsNormal() && lspc == -11; }
	bool IsReplaced() const   { return !IsNormal() && lspc >= 0; }
	bool IsMissing() const    { return !IsNormal() && lspc == -2; }
};

GlyphInfo      GetGlyphInfoSys(Font font, int chr);

struct FaceInfo : Moveable<FaceInfo> {
	String name;
	dword  info;
};

Vector<FaceInfo> GetAllFacesSys();

void Std(Font& font);

bool      IsNormal(Font font, int chr);

GlyphInfo GetGlyphInfo(Font font, int chr);
void      GlyphMetrics(GlyphInfo& f, Font font, int chr);
GlyphInfo GetGlyphMetrics(Font font, int chr);

const CommonFontInfo& GetFontInfo(Font font);

struct ComposedGlyph {
	wchar  basic_char;
	Point  mark_pos;
	wchar  mark_char;
	Font   mark_font;
};

bool Compose(Font fnt, int chr, ComposedGlyph& cs);
bool Replace(Font fnt, int chr, Font& rfnt);

void DoDrawText(Draw& w, int x, int y, int angle, const wchar *text, Font font, Color ink,
                int n, const int *dx);

#endif
