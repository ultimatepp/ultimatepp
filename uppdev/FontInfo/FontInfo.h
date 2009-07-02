#ifndef _FontInfo_FontInfo_h_
#define _FontInfo_FontInfo_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

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
	int    underline_thickness;
	int    underline_position;
	String path;
#endif
};

struct GlyphInfo {
	int16 width;
	int16 lspc;
	int16 rspc;
};

GlyphInfo      GetGlyphInfoSys(Font font, int chr);
CommonFontInfo GetFontInfoSys(Font font);

#endif
