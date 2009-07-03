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
	
	bool IsMissing() const { return (word)width == (word)0x8000; }
};

GlyphInfo      GetGlyphInfoSys(Font font, int chr);

struct FaceInfo : Moveable<FaceInfo> {
	String name;
	bool   scalable;
	bool   fixed;
};

Vector<FaceInfo> GetAllFacesSys();

#endif
